const core = require('@actions/core');
const Octokit = require('@octokit/rest');
const fetch = require('node-fetch');
const fs = require('fs');
const unzipper = require('unzipper');
const tar = require('tar');

try {
  const repo = core.getInput('repository', {required: true});
  console.log('Looking for repository:', repo);
  const owner = core.getInput('user', {required: true});
  const file = core.getInput('file', {required: true});
  const outputFile = repo.concat(file);
  const token = core.getInput('token');
  if (token) {
    console.log('Ocktokit with token');
    var octokit = new Octokit({auth: token});
    var hasToken = true;
  } else {
    console.log('Ocktokit without token');
    var octokit = new Octokit();
    var hasToken = false;
  }

  octokit.repos.getLatestRelease({owner, repo}).then(latest => {
    console.log(latest);
    const release_id = latest.data.id;
    octokit.repos.listAssetsForRelease({owner, repo, release_id})
        .then(assets => {
          console.log(assets);
          const asset = assets.data.find(asset => asset.name.includes(file));
          console.log('Asset name:', asset.name);
          let assetUrl = asset.url
          if (hasToken) {
            assetUrl = assetUrl.concat('?access_token=', token);
          }
          fetch(assetUrl, {
            headers: {accept: 'application/octet-stream'}
          }).then(response => {
            console.log('Downloading to:', outputFile);
            response.body.pipe(fs.createWriteStream(outputFile))
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
          });
        });
  });
} catch (error) {
  core.setFailed(error.message);
}
