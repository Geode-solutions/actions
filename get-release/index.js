const core = require('@actions/core');
const { Octokit } = require('@octokit/rest');
const fs = require('fs');
const path = require('path');
const request = require('request');
const unzipper = require('unzipper');
const tar = require('tar');

try {
  const repos = core.getInput('repository');
  if (repos.length) {
    let results = [];
    const file = core.getInput('file', { required: true });
    const version = core.getInput('version');
    const token = core.getInput('token');
    const octokit = new Octokit({ auth: token });
    repos.split(';').forEach(owner_repo => {
      if (!owner_repo.length) {
        return;
      }
      const owner_repo_array = owner_repo.split('/')
      let owner = 'Geode-solutions'
      let repo = owner_repo_array[0]
      if (owner_repo_array.length == 2) {
        owner = owner_repo_array[0]
        repo = owner_repo_array[1]
      }
      let promise = new Promise(function (resolve) {
        console.log('Looking for repository:', repo);
        const outputFile = repo.concat(file);

        const query = version === 'master' ?
          octokit.repos.getLatestRelease({ owner, repo }).then(release => release.data.id) :
          octokit.repos.listReleases({ owner, repo }).then(releases => releases.data[0].id);
        query.then(release_id => {
          octokit.repos.listReleaseAssets({ owner, repo, release_id })
            .then(assets => {
              console.log(assets);
              const asset =
                assets.data.find(asset => asset.name.includes(file));
              console.log('Asset name:', asset.name);
              request({
                url: asset.url,
                method: 'GET',
                headers: {
                  accept: 'application/octet-stream',
                  Authorization: 'Bearer ' + token,
                  'User-Agent': ''
                }
              })
                .pipe(fs.createWriteStream(outputFile))
                .on('finish', function () {
                  const extension = outputFile.split('.').pop();
                  console.log('Extension:', extension);
                  if (extension == 'zip') {
                    console.log('Unzipping', asset.name);
                    fs.createReadStream(outputFile)
                      .pipe(unzipper.Extract(
                        { path: process.env.GITHUB_WORKSPACE }))
                      .on('close', function () {
                        let extract_name = asset.name.slice(0, -4);
                        if (extract_name.endsWith('-private')) {
                          extract_name = extract_name.slice(0, -8);
                        }
                        console.log('Unzip to:', extract_name);
                        const result = path.join(
                          process.env.GITHUB_WORKSPACE, extract_name);
                        console.log('Result:', result);
                        fs.unlinkSync(outputFile);
                        resolve(result);
                      });
                  } else if (extension == 'gz') {
                    console.log('Untaring', asset.name);
                    fs.createReadStream(outputFile)
                      .pipe(tar.x())
                      .on('close', function () {
                        let extract_name = asset.name.slice(0, -7);
                        if (extract_name.endsWith('-private')) {
                          extract_name = extract_name.slice(0, -8);
                        }
                        console.log('Untar to:', extract_name);
                        const result = path.join(
                          process.env.GITHUB_WORKSPACE, extract_name);
                        console.log('Result:', result);
                        fs.unlinkSync(outputFile);
                        resolve(result);
                      });
                  }
                });
            });
        });
      });
      results.push(promise);
    });
    Promise.all(results).then(outputs => {
      let result = '';
      outputs.forEach(output => result += output + ';');
      core.setOutput('path', result);
      console.log('Final result:', result);
    })
  }
} catch (error) {
  core.setFailed(error.message);
}
