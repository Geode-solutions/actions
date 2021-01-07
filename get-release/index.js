const core = require('@actions/core');
const {Octokit} = require('@octokit/rest');
const fs = require('fs');
const request = require('request');
const unzipper = require('unzipper');
const tar = require('tar');

try {
  const repo = core.getInput('repository', {required: true});
  console.log('Looking for repository:', repo);
  const owner = core.getInput('owner', {required: true});
  const file = core.getInput('file', {required: true});
  const version = core.getInput('version');
  const outputFile = repo.concat(file);
  const token = core.getInput('token');
  const octokit = new Octokit({auth: token});

  const promise = version ? octokit.repos.getReleaseByTag({owner, repo, tag: version}) : octokit.repos.getLatestRelease({owner, repo});
  promise.then(release => {
    console.log(release);
    const release_id = release.data.id;
    octokit.repos.listReleaseAssets({owner, repo, release_id})
        .then(assets => {
          console.log(assets);
          const asset = assets.data.find(asset => asset.name.includes(file));
          console.log('Asset name:', asset.name);
          request({
            url:asset.url,
            method: "GET",
            headers: {
              accept: 'application/octet-stream',
              Authorization: 'Bearer ' + token,
              "User-Agent": ""
            }
          }).pipe(fs.createWriteStream(outputFile))
                .on('finish', function() {
                  const extension = outputFile.split('.').pop();
                  console.log('Extension:', extension);
                  if (extension == 'zip') {
                    console.log('Unzipping');
                    fs.createReadStream(outputFile)
                        .pipe(unzipper.Extract(
                            {path: process.env.GITHUB_WORKSPACE}))
                        .on('close', function() {
                          console.log('Unzip to:', asset.name.slice(0, -4));
                          core.setOutput('path', asset.name.slice(0, -4));
                          fs.unlinkSync(outputFile);
                        });
                  } else if (extension == 'gz') {
                    console.log('Untaring');
                    fs.createReadStream(outputFile)
                        .pipe(tar.x())
                        .on('close', function() {
                          console.log('Untar to:', asset.name.slice(0, -7));
                          core.setOutput('path', asset.name.slice(0, -7));
                          fs.unlinkSync(outputFile);
                        });
                  }
                });
          //});
        });
  });
} catch (error) {
  core.setFailed(error.message);
}
