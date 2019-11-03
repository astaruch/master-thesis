/* eslint-disable no-process-exit */
'use strict'

const yargs = require('yargs')
const Page = require('./page')
const jsdomDevtoolsFormatter = require('jsdom-devtools-formatter');

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
    .group(['url'], 'Input:')
    .describe('url', 'Enter one URL as parameter' )
    .group(['output-json', 'output-lines'], 'Output formats:')
    .describe('output-json', 'Return results as escaped JSON' )
    .describe('output-lines', 'Return results as lines in format "<column name> <value>\\n"')
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

    if (!argv.argv.url) {
      console.error('You have to provide URL to check')
      process.exit(1)
    }
    jsdomDevtoolsFormatter.install();

    const page = new Page(argv.argv.url)
    const features = {}
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
