const core = require('@actions/core');
const { Octokit } = require('@octokit/rest');
const fs = require('fs');

try {
  const owner = "Geode-solutions";
  const repo = "Versions";
  const token = core.getInput('token');
  const octokit = new Octokit({ auth: token });
  octokit.rest.actions
    .listWorkflowRunsForRepo(
      { owner, repo, per_page: '1', event: 'schedule' })
    .then(wf => {
      const conclusion = wf.data.workflow_runs[0].conclusion;
      console.log(conclusion);
      core.setOutput('update', conclusion == 'success');
    })
    .catch(error => core.setFailed(error));
} catch (error) {
  core.setFailed(error.message);
}
