const core = require('@actions/core');
const {Octokit} = require('@octokit/rest');
const fs = require('fs');
const path = require('path');
const child_process = require('child_process');

try {
  const repos = core.getInput('repository');
  if (repos.length) {
    let results = [];
    const owner = core.getInput('owner', {required: true});
    const token = core.getInput('token');
    const octokit = new Octokit({auth: token});
    repos.split(';').forEach(repo => {
      if (!repo.length) {
        return;
      }
      let promise = new Promise(function(resolve) {
        console.log('Looking for repository:', repo);
        octokit.rest.actions
            .listWorkflowRunsForRepo(
                {owner, repo, per_page: '1', event: 'schedule'})
            .then(wf => {
              const conclusion = wf.data.workflow_runs[0].conclusion;
              console.log(conclusion);
              if (conclusion == 'success') {
                octokit.repos.getLatestRelease({owner, repo}).then(release => {
                  const version = release.data.tag_name;
                  console.log(release.data.tag_name);
                  resolve({repo, version})
                })
              }
            });
      });
      results.push(promise);
    });
    Promise.all(results).then((outputs) => {
      console.log(outputs)
      const json = JSON.stringify(outputs.sort(), null, 2)
      console.log(json);
      fs.writeFile('versions.json', json, (err) => {
        if (err) throw err;
        console.log('The file has been saved!');
        child_process.execSync(path.join(__dirname, 'commit.sh'))
      })
    })
  }
} catch (error) {
  core.setFailed(error.message);
}
