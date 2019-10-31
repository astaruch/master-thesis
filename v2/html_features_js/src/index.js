/* eslint-disable no-process-exit */
'use strict'

const yargs = require('yargs')
const Page = require('./page')
const jsdomDevtoolsFormatter = require('jsdom-devtools-formatter');

const main = async () => {
  const argv = yargs
    .usage('Application for phishing defence\nUsage:\n$0 [OPTION...]')
    .help('help').alias('help', 'h')
    .option('url', { description: 'Enter one escaped URL as parameter'})
    .option('feat-input-tag', {description: 'Path containing Phishtank csv files'})

    if (!argv.argv.url) {
      console.error('You have to provide URL to check')
      process.exit(1)
    }
    jsdomDevtoolsFormatter.install();

    const page = new Page(argv.argv.url)
    const features = {}
    if (argv.argv.featInputTag) {
      features.inputTag = 'inputTag'
    }
    const results = await page.performTests(features)
    console.log((results))
    // console.log(JSON.stringify(results))
}

if (require.main === module) {
  main()
}
