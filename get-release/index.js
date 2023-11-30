import core from "@actions/core"
import github from "@actions/github"
import { Octokit } from "@octokit/rest"
import fs from "fs"
import path from "path"
import request from "request"
import tar from "tar"
import AdmZip from "adm-zip"

const main = async () => {
  try {
    const repos = core.getInput("repository")
    if (repos.length) {
      let results = []
      const file = core.getInput("file", { required: true })
      const token = core.getInput("token")
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
          const outputFile = repo.concat(file)
          const query =
            github.ref === "refs/heads/master"
              ? octokit.repos
                  .getLatestRelease({ owner, repo })
                  .then((release) => release.data.id)
              : octokit.repos.listReleases({ owner, repo }).then((releases) => {
                  if (github.context.payload.pull_request) {
                    const release = releases.data.find(
                      (r) =>
                        r.name === github.context.payload.pull_request.base.ref
                    )
                    if (release) {
                      return release.id
                    }
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
              .then((assets) => {
                const asset = assets.data.find((asset) =>
                  asset.name.includes(file)
                )
                console.log(
                  "Asset:",
                  asset,
                  asset == undefined,
                  asset === undefined,
                  asset == "undefined",
                  asset === "undefined",
                  typeof asset == undefined,
                  typeof asset === undefined,
                  typeof asset == "undefined",
                  typeof asset === "undefined"
                )
                if (typeof asset === undefined) {
                  resolve("")
                }
                console.log("Asset name:", asset.name)
                request({
                  url: asset.url,
                  method: "GET",
                  headers: {
                    accept: "application/octet-stream",
                    Authorization: "Bearer " + token,
                    "User-Agent": "",
                  },
                })
                  .pipe(fs.createWriteStream(outputFile))
                  .on("finish", function () {
                    const extension = outputFile.split(".").pop()
                    if (extension == "zip") {
                      console.log("Unzipping", asset.name)
                      const zip = new AdmZip(outputFile)
                      zip.extractAllTo(process.env.GITHUB_WORKSPACE)
                      let extract_name = asset.name.slice(0, -4)
                      if (extract_name.endsWith("-private")) {
                        extract_name = extract_name.slice(0, -8)
                      }
                      console.log("Unzip to:", extract_name)
                      const result = path.join(
                        process.env.GITHUB_WORKSPACE,
                        extract_name
                      )
                      console.log("Result:", result)
                      resolve(result)
                    } else if (extension == "gz") {
                      console.log("Untaring", asset.name)
                      fs.createReadStream(outputFile)
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
                          fs.unlinkSync(outputFile)
                          resolve(result)
                        })
                    } else if (extension == "whl") {
                      console.log("Skipping", asset.name)
                      const result = path.join(
                        process.env.GITHUB_WORKSPACE,
                        asset.name
                      )
                      resolve(result)
                    }
                  })
              })
          })
        })
        results.push(promise)
      })
      Promise.all(results).then((outputs) => {
        let result = ""
        outputs.forEach((output) => (result += output + ";"))
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
