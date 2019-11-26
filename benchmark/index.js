const core = require('@actions/core');
const Octokit = require('@octokit/rest');
const fetch = require( 'node-fetch');
const child_process = require( 'child_process');
const request = require( 'request');
const fs = require('fs');
const xml2js = require('xml2js');

function get_results(directory){
  const files = fs.readdirSync(directory).filter(name => name.startsWith('test-'));
  const results = [];
  files.forEach(file => {
    console.log(file);
    const result = {name:'', value:{}};
    const output_file = directory +'/'+file + '.xml';
    child_process.spawnSync(directory +'/'+file,['-r xml']);
    child_process.spawnSync(directory +'/'+file,['-r xml','-o ' + output_file]);
    const xml = fs.readFileSync(output_file, {encoding: 'utf8'});
    xml2js.parseString(xml, (err, json) => {
      const data = json.Catch.Group[0].TestCase[0].BenchmarkResults[0];
      result.name = data.$.name;
      const values = data.mean[0].$;
      result.value.mean = parseFloat(values.value);
      result.value.down = parseFloat(values.lowerBound);
      result.value.up = parseFloat(values.upperBound);
      results.push(result);
    });
  });
  return results;
}


try {
  const repository = core.getInput('repository', { required: true });
  const path = 'assets/' + repository + '.json';
  const directory = process.env.GITHUB_WORKSPACE + '/' + core.getInput('directory', { required: true });
  const token = core.getInput('token', { required: true });
  console.log("Using directory:", directory);
  const results = get_results(directory);
  console.log(results);

  request.get('https://raw.githubusercontent.com/Geode-solutions/benchmarks/master/' + path, (error, response, data) => {
    if (!error && response.statusCode == 200) {
        data = JSON.parse(data);
        results.forEach(result => {
          const benchmark = data.find(element => element.name == result.name);
          if(benchmark != undefined) {
            benchmark.values.push(result.value);
          } else {
            data.push({name: result.name, values:[result.value]});
          }         
        });
        console.log(data);
        const content = Buffer.from(JSON.stringify(data)).toString("base64");
        const owner = 'Geode-solutions';
        const repo = 'benchmarks';
        const octokit = new Octokit({auth: token});
        octokit.repos.getContents({owner, repo, path }).then(response => {
          const sha = response.data.sha;
          octokit.repos.createOrUpdateFile({owner, repo, path,
            message:"update benchmarks",
            content, sha
          });
        });
     }
  });
} catch (error) {
  core.setFailed(error.message);
}
