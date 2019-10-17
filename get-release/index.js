const core = require('@actions/core');
const github = require('@actions/github');
const fetch = require( 'node-fetch');
const fs = require('fs');
const unzipper = require('unzipper');
const tar = require('tar');

try {
  const repo = core.getInput('repository', { required: true });
  const owner = 'Geode-solutions';
  const file = core.getInput('file', { required: true });
  const outputFile = repo.concat(file);
  const token = core.getInput('token'));
  const octokit = new github.GitHub(token);

  octokit.repos.getLatestRelease({owner,repo})
    .then(latest => {
      const release_id = latest.data.id;
      octokit.repos.listAssetsForRelease({owner,repo,release_id})
        .then(assets => {
          const asset = assets.data.find(asset => asset.name.includes(file));
          const assetUrl = asset.url
          fetch(`${assetUrl}?access_token=${token}`, {
            headers: {
              accept: 'application/octet-stream'
            }
          }).then(response => {
            response.body.pipe(fs.createWriteStream(outputFile)).on('finish', function() {
              const archive = fs.createReadStream(outputFile);
              const extension = outputFile.split('.').pop();
              if (extension == "zip"){
                archive.pipe(unzipper.Extract({ path: repo })).on('finish', function() {
                  core.setOutput('path', repo.concat('/',fs.readdirSync(repo)[0]));
                  fs.unlinkSync(outputFile);
                });
              } else if (extension == "gz"){
                archive.pipe(tar.x({ C: repo })).on('finish', function() {
                  core.setOutput('path', repo.concat('/',fs.readdirSync(repo)[0]));
                  fs.unlinkSync(outputFile);
                });
              }
            });            
          });
        }
      );
    }
  );
} catch (error) {
  core.setFailed(error.message);
}
