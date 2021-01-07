const core = require('@actions/core');
const {Octokit} = require('@octokit/rest');
const fs = require('fs');
const request = require('request');

try {
  const token = core.getInput('token');
  const octokit = new Octokit({auth: token});
  const [owner, repo] = process.env.GITHUB_REPOSITORY.split("/");
  const public = repo.slice(0, -8);

  const promise = octokit.repos.getLatestRelease({owner, repo});
  promise.then(release_res => {
    const release = release_res.data;

    octokit.repos.createRelease({
      owner, 
      repo: public, 
      name: release.name, 
      tag_name: release.tag_name, 
      body: release.body
    }).then(public_release_res =>{
     const public_release = public_release_res.data;
    octokit.repos.listReleaseAssets({owner, repo, release_id: release.id})
        .then(assets => {
          console.log(assets.data);
          assets.data.forEach(asset => {
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
                    repo: public, 
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
          });
      } );
  });
} catch (error) {
  core.setFailed(error.message);
}
