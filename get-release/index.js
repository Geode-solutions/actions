import core from "@actions/core";
import github from "@actions/github";
import { Octokit } from "@octokit/rest";
import fs from "fs";
import path from "path";
import https from "https";
import http from "http";
import * as tar from "tar";
import unzipper from "unzipper";
import { execSync } from "child_process";

const download_file = (url, dest, token) => {
  return new Promise((resolve, reject) => {
    const options = {
      headers: {
        accept: "application/octet-stream",
        Authorization: "Bearer " + token,
        "User-Agent": "get-release-action",
      },
    };

    const handleResponse = (res) => {
      console.log("Response status:", res.statusCode);
      console.log("Response URL:", res.headers.location || url);

      if (res.statusCode === 302 || res.statusCode === 301) {
        const redirectUrl = res.headers.location;
        console.log("Redirecting to:", redirectUrl);
        const redirectModule = redirectUrl.startsWith("https") ? https : http;
        redirectModule.get(redirectUrl, handleResponse).on("error", reject);
        return;
      }
      if (res.statusCode !== 200) {
        reject(new Error(`Download failed with status ${res.statusCode}`));
        return;
      }
      let bytes = 0;
      const writer = fs.createWriteStream(dest);
      res.on("data", (chunk) => {
        bytes += chunk.length;
      });
      res.pipe(writer);
      writer.on("finish", () => {
        console.log(`Downloaded ${bytes} bytes to ${dest}`);
        const stat = fs.statSync(dest);
        console.log(`File size on disk: ${stat.size} bytes`);
        // Print first 4 bytes - should be 50 4B 03 04 for a valid ZIP
        const fd = fs.openSync(dest, "r");
        const buf = Buffer.alloc(4);
        fs.readSync(fd, buf, 0, 4, 0);
        fs.closeSync(fd);
        console.log("First 4 bytes (should be 50 4B 03 04 for ZIP):", buf.toString("hex"));
        writer.close(resolve);
      });
      writer.on("error", reject);
    };

    https.get(url, options, handleResponse).on("error", reject);
  });
};

const download_asset = async (asset, token) => {
  await download_file(asset.url, asset.name, token);

  const extension = asset.name.split(".").pop();
  if (extension == "zip") {
    console.log("Unzipping", asset.name);
    const destPath = process.env.GITHUB_WORKSPACE;

    if (process.platform == "win32") {
      execSync(
        `powershell -Command "Expand-Archive -Force -Path '${asset.name}' -DestinationPath '${destPath}'"`,
        { stdio: "inherit" },
      );
    } else {
      execSync(`unzip -o "${asset.name}" -d "${destPath}"`, { stdio: "inherit" });
      // Restore executable permissions on linux
      let extract_name = asset.name.slice(0, -4);
      if (extract_name.endsWith("-private")) extract_name = extract_name.slice(0, -8);
      const extract_dir = path.join(destPath, extract_name);
      if (fs.existsSync(extract_dir)) {
        execSync(`chmod -R 755 "${extract_dir}"`);
      }
    }

    let extract_name = asset.name.slice(0, -4);
    if (extract_name.endsWith("-private")) extract_name = extract_name.slice(0, -8);
    const result = path.join(destPath, extract_name);
    console.log("Unzip to:", extract_name);
    console.log("Result:", result);
    fs.unlinkSync(asset.name);
    return result;
  } else if (extension == "gz") {
    console.log("Untaring", asset.name);
    await new Promise((resolve, reject) => {
      fs.createReadStream(asset.name).pipe(tar.x()).on("close", resolve).on("error", reject);
    });
    let extract_name = asset.name.slice(0, -7);
    if (extract_name.endsWith("-private")) {
      extract_name = extract_name.slice(0, -8);
    }
    const result = path.join(process.env.GITHUB_WORKSPACE, extract_name);
    console.log("Untar to:", extract_name);
    console.log("Result:", result);
    fs.unlinkSync(asset.name);
    return result;
  } else {
    console.log("Downloading", asset.name);
    return path.join(process.env.GITHUB_WORKSPACE, asset.name);
  }
};

const main = async () => {
  try {
    const repos = core.getInput("repository");
    if (repos.length) {
      const file = core.getInput("file", { required: true });
      const token = core.getInput("token");
      const branch = core.getInput("branch");
      const base = core.getInput("base");
      const octokit = new Octokit({ auth: token });

      const results = [];

      for (const owner_repo of repos.split(";")) {
        if (!owner_repo.length) continue;

        const owner_repo_array = owner_repo.split("/");
        let owner = "Geode-solutions";
        let repo = owner_repo_array[0];
        if (owner_repo_array.length == 2) {
          owner = owner_repo_array[0];
          repo = owner_repo_array[1];
        }

        console.log("Looking for repository:", repo);

        const release_id = await (branch.includes("master")
          ? octokit.repos.getLatestRelease({ owner, repo }).then((r) => r.data.id)
          : octokit.repos.listReleases({ owner, repo, per_page: 100 }).then((releases) => {
              if (github.context.payload.pull_request) {
                const head_release = releases.data.find(
                  (r) => r.name === github.context.payload.pull_request.head.ref,
                );
                if (head_release) {
                  console.log("Found head release:", head_release.name, " for ", repo);
                  return head_release.id;
                }
                const base_release = releases.data.find(
                  (r) => r.name === github.context.payload.pull_request.base.ref,
                );
                if (base_release) {
                  console.log("Found base release:", base_release.name, " for ", repo);
                  return base_release.id;
                }
              }
              const branch_release = releases.data.find((r) => r.name === branch);
              if (branch_release) {
                console.log("Found branch release:", branch_release.name, " for ", repo);
                return branch_release.id;
              }
              const base_release = releases.data.find((r) => r.name === base);
              if (base_release) {
                console.log("Found base release:", base_release.name, " for ", repo);
                return base_release.id;
              }
              const release = releases.data.find(
                (r) => r.name.startsWith("v") && r.name.includes("-rc."),
              );
              if (release) {
                console.log("Found release:", release.name, " for ", repo);
                return release.id;
              }
              console.log("Found default release:", releases.data[0].name, " for ", repo);
              return releases.data[0].id;
            }));

        const assets = await octokit.repos.listReleaseAssets({ owner, repo, release_id });
        const filtered_assets = assets.data.filter((asset) => asset.name.includes(file));
        const repo_results = [];
        for (const asset of filtered_assets) {
          console.log("Asset name:", asset.name);
          const result = await download_asset(asset, token);
          repo_results.push(result);
        }
        results.push(repo_results);
      }

      let result = "";
      results.forEach((output) => {
        output.forEach((f) => {
          result += f + ";";
        });
      });
      result = result.slice(0, -1);
      core.setOutput("path", result);
      console.log("Final result:", result);
    }
  } catch (error) {
    core.setFailed(error.message);
  }
};

main();
