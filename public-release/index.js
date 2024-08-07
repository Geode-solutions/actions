import core from '@actions/core';
import {Octokit} from '@octokit/rest';
import fs from 'fs';
import request from 'request';

try {
  const token = core.getInput('token');
  const octokit = new Octokit({auth: token});
  const [owner, repo] = process.env.GITHUB_REPOSITORY.split("/");
  console.log("owner", owner);
  console.log("repo", repo);
  const public_repo = repo.slice(0, -8);
  console.log("public", public_repo);

  const promise = octokit.repos.getLatestRelease({owner, repo});
  promise
  .then(release_res => {
    const release = release_res.data;
    console.log("release name", release.name);
    octokit.repos.createRelease({
      owner, 
      repo: public_repo, 
      name: release.name, 
      tag_name: release.tag_name, 
      body: release.body
    }).then(public_release_res =>{
     const public_release = public_release_res.data;
     console.log("public_release name", public_release.name);
    octokit.repos.listReleaseAssets({owner, repo, release_id: release.id})
        .then(assets => {
          assets.data.forEach(asset => {
          console.log(asset.url);
          if(asset.name.includes("private")) {
            return;
          }
          request({
            url: asset.url,
            method: "GET",
            headers: {
              accept: 'application/octet-stream',
              Authorization: 'Bearer ' + token,
              "User-Agent": ""
            }
          }).pipe(fs.createWriteStream(asset.name))
                .on('finish', function() {
                  octokit.repos.uploadReleaseAsset({
                    owner, 
                    repo: public_repo, 
                    release_id: public_release.id,
                    name: asset.name,
                    data: fs.createReadStream(asset.name),
                    headers: {
                        'content-type': asset.content_type,
                        'content-length': asset.size
                    }
                  });
                });
              })
          }).catch(err => console.log(err));
      } ).catch(err => console.log(err));
  }).catch(err => console.log(err));
} catch (error) {
  core.setFailed(error.message);
}
