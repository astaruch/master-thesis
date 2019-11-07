/* eslint-disable no-process-exit */
'use strict'

const yargs = require('yargs')
const Page = require('./page')
const jsdomDevtoolsFormatter = require('jsdom-devtools-formatter');
const readline = require('readline')


const main = async () => {
  // These strings are used in the specific order in option arguments. Move with a caution!
  const featureStrings = [
    'feat-input-tag', 'feat-src-link', 'feat-form-handler', 'feat-invisible-iframe', 'feat-rewrite-statusbar',
    'feat-disable-rightclick', 'feat-ahref-link', 'feat-popup-window', 'feat-favicon-link', 'feat-old-technologies',
    'feat-missleading-link', 'feat-hostname-title'
  ]
  const argv = yargs
    .usage('Application for phishing defence\nUsage:\n$0 [OPTION...]')
    .help('help').alias('help', 'h')
    .group(['url', 'stdin'], 'Input:')
    .describe('url', 'Enter one URL as parameter' )
    .describe('stdin', 'Take input URLs from stdin')
    .group(['output-json', 'output-lines', 'output-values-string', 'print-values'], 'Output formats:')
    .describe('output-json', 'Return results as escaped JSON' )
    .describe('output-values-string', 'Return results as one string in format "<value1>,..,<value2>"')
    .describe('output-lines', 'Return results as lines in format "<column name> <value>\\n"')
    .describe('include-values', 'Prints also values used for tuning')
    .group(featureStrings, 'Features:')
    .describe(featureStrings[0], 'Flag wether check how many input tags has page')
    .describe(featureStrings[1], 'Flag wether check if src=<link> is matching hostname')
    .describe(featureStrings[2], 'Flag wether check <form> handlers')
    .describe(featureStrings[3], 'Flag wether check invisible <iframe> elements')
    .describe(featureStrings[4], 'Flag wether check rewriting a status bar')
    .describe(featureStrings[5], 'Flag wether check that page has disabled right-click')
    .describe(featureStrings[6], 'Flag wether check <a href="LINK"> pointing to outer world')
    .describe(featureStrings[7], 'Flag wether check PopUp windows')
    .describe(featureStrings[8], 'Flag wether check if favicon is pointing to another site')
    .describe(featureStrings[9], 'Flag wether check if site is not running new technologies')
    .describe(featureStrings[10], 'Flag wether check if text value of <a> link is same as actual link')
    .describe(featureStrings[11], 'Flag wether check if hostname is matching title (0: yes, 1: no)')

    if (!argv.argv.stdin && !argv.argv.url) {
      console.error('You have to provide URL to check or start as "--stdin"')
      process.exit(1)
    }
    jsdomDevtoolsFormatter.install();

    const features = {}
    // We are relying on the order. check c++ file service/src/features/feature_enum.h
    if (argv.argv.featInputTag) features.inputTag = 'inputTag'
    if (argv.argv.featSrcLink) features.srcLink = 'srcLink'
    if (argv.argv.featFormHandler) features.formHandler = 'formHandler'
    if (argv.argv.featInvisibleIframe) features.invisibleIframe = 'invisibleIframe'
    if (argv.argv.featRewriteStatusbar) features.rewriteStatusbar = 'rewriteStatusbar'
    if (argv.argv.featDisableRightclick) features.disableRightclick = 'disableRightclick'
    if (argv.argv.featAhrefLink) features.ahrefLink = 'ahrefLink'
    if (argv.argv.featPopupWindow) features.popupWindow = 'popupWindow'
    if (argv.argv.featFaviconLink) features.faviconLink = 'faviconLink'
    if (argv.argv.featOldTechnologies) features.oldTechnologies = 'oldTechnologies'
    if (argv.argv.featMissleadingLink) features.missleadingLink = 'missleadingLink'
    if (argv.argv.featHostnameTitle) features.hostnameTitle = 'hostnameTitle'

    let urls = []
    if (argv.argv.url) {
      urls.push(argv.argv.url)
    } else if (argv.argv.stdin) {
      const rl = readline.createInterface({
        input: process.stdin,
      })
      for await (const line of rl) {
        urls.push(line)
        console.log(`Line from file: ${line}`);
      }
    } else {
      console.error('Missing input type argument (e.g. --stdin)')
      process.exit(1)
    }
    for await (const url of urls) {
      console.log(`--> Checking ${url}`)
      const page = new Page(url, argv.argv.includeValues)
      const results = await page.performTests(features)

      if (argv.argv.outputJson) {
        console.log(JSON.stringify(results))
      } else if (argv.argv.outputLines) {
        Object.keys(results).forEach(feature => {
          console.log(`${feature} ${results[feature]}`)
        })
      } else if (argv.argv.outputValuesString) {
        console.log(results)
      } else {
        console.error('You need to set output format')
        process.exit(1)
      }
    }
}

if (require.main === module) {
  main()
}
