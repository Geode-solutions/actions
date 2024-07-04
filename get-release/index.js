import core from "@actions/core"
import github from "@actions/github"
import { Octokit } from "@octokit/rest"
import fs from "fs"
import path from "path"
import request from "request"
import * as tar from "tar"
import AdmZip from "adm-zip"

const download_asset = async (asset, token) => {
  return new Promise((resolve) => {
    request({
      url: asset.url,
      method: "GET",
      headers: {
        accept: "application/octet-stream",
        Authorization: "Bearer " + token,
        "User-Agent": "",
      },
    })
      .pipe(fs.createWriteStream(asset.name))
      .on("finish", function () {
        const extension = asset.name.split(".").pop()
        if (extension == "zip") {
          console.log("Unzipping", asset.name)
          const zip = new AdmZip(asset.name)
          zip.extractAllTo(process.env.GITHUB_WORKSPACE)
          let extract_name = asset.name.slice(0, -4)
          if (extract_name.endsWith("-private")) {
            extract_name = extract_name.slice(0, -8)
          }
          console.log("Unzip to:", extract_name)
          const result = path.join(process.env.GITHUB_WORKSPACE, extract_name)
          console.log("Result:", result)
          resolve(result)
        } else if (extension == "gz") {
          console.log("Untaring", asset.name)
          fs.createReadStream(asset.name)
            .pipe(tar.x())
            .on("close", function () {
              let extract_name = asset.name.slice(0, -7)
              if (extract_name.endsWith("-private")) {
                extract_name = extract_name.slice(0, -8)
              }
              console.log("Untar to:", extract_name)
              const result = path.join(
                process.env.GITHUB_WORKSPACE,
                extract_name
              )
              console.log("Result:", result)
              resolve(result)
            })
        } else if (extension == "whl") {
          console.log("Skipping", asset.name)
          const result = path.join(process.env.GITHUB_WORKSPACE, asset.name)
          resolve(result)
        }
      })
  })
}

const main = async () => {
  try {
    const repos = core.getInput("repository")
    if (repos.length) {
      let promises = []
      const file = core.getInput("file", { required: true })
      const token = core.getInput("token")
      const branch = core.getInput("branch")
      const octokit = new Octokit({ auth: token })
      repos.split(";").forEach((owner_repo) => {
        if (!owner_repo.length) {
          return
        }
        const owner_repo_array = owner_repo.split("/")
        let owner = "Geode-solutions"
        let repo = owner_repo_array[0]
        if (owner_repo_array.length == 2) {
          owner = owner_repo_array[0]
          repo = owner_repo_array[1]
        }
        let promise = new Promise(function (resolve) {
          console.log("Looking for repository:", repo)
          const query = branch.includes("master")
            ? octokit.repos
                .getLatestRelease({ owner, repo })
                .then((release) => release.data.id)
            : octokit.repos.listReleases({ owner, repo }).then((releases) => {
                if (github.context.payload.pull_request) {
                  const head_release = releases.data.find(
                    (r) =>
                      r.name === github.context.payload.pull_request.head.ref
                  )
                  if (head_release) {
                    return head_release.id
                  }
                  const base_release = releases.data.find(
                    (r) =>
                      r.name === github.context.payload.pull_request.base.ref
                  )
                  if (base_release) {
                    return base_release.id
                  }
                }
                const branch_release = releases.data.find(
                  (r) => r.name === branch
                )
                if (branch_release) {
                  return branch_release.id
                }
                const release = releases.data.find(
                  (r) => r.name.startsWith("v") && r.name.includes("-rc.")
                )
                if (release) {
                  return release.id
                }
                return releases.data[0].id
              })
          query.then((release_id) => {
            octokit.repos
              .listReleaseAssets({ owner, repo, release_id })
              .then(async (assets) => {
                const filtered_assets = assets.data.filter((asset) =>
                  asset.name.includes(file)
                )
                let results = []
                for (let i = 0; i < filtered_assets.length; i++) {
                  console.log("Asset name:", filtered_assets[i].name)
                  const result = await download_asset(filtered_assets[i], token)
                  results.push(result)
                }
                resolve(results)
              })
          })
        })
        promises.push(promise)
      })
      Promise.all(promises).then((outputs) => {
        let result = ""
        outputs.forEach((output) => {
          console.log("Output:", output)
          output.forEach((file) => {
            result += file + ";"
          })
        })
        result = result.slice(0, -1)
        core.setOutput("path", result)
        console.log("Final result:", result)
      })
    }
  } catch (error) {
    core.setFailed(error.message)
  }
}

// Call the main function to run the action
main()
