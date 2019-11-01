/* eslint-disable no-process-exit */
'use strict'

const yargs = require('yargs')
const Page = require('./page')
const jsdomDevtoolsFormatter = require('jsdom-devtools-formatter');

const main = async () => {
  const argv = yargs
    .usage('Application for phishing defence\nUsage:\n$0 [OPTION...]')
    .help('help').alias('help', 'h')
    .group(['url'], 'Input:')
    .describe('url', 'Enter one URL as parameter' )
    .group(['output-json', 'output-lines'], 'Output formats:')
    .describe('output-json', 'Return results as escaped JSON' )
    .describe('output-lines', 'Return results as lines in format "<column name> <value>\\n"')
    .group(['feat-input-tag'], 'Features:')
    .describe('feat-input-tag', 'Flag wether check how many input tags has page')

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

    if (argv.argv.outputJson) {
      console.log(JSON.stringify(results))
      return
    }
    if (argv.argv.outputLines) {
      Object.keys(results).forEach(feature => {
        console.log(`${feature} ${results[feature]}`)
      })
      return
    }
    console.error('You need to set output format')
    process.exit(1)
}

if (require.main === module) {
  main()
}
