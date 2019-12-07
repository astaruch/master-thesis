/* eslint-disable no-process-exit */
'use strict'

const yargs = require('yargs')
const readline = require('readline')
const net = require('net')
const Page = require('./page')

const parseCmdline = () => {
  const args = yargs
    .usage('HTML features analysis\nUsage:\n$0 [OPTION...]')
    .help('help').alias('help', 'h')
    .describe('verbose', 'Flag whether enable verbose mode')
    .option('server', {
      type: 'number',
      describe: 'listen on socket for JSONs on a given port'
    }).group(['url', 'stdin', 'server'], 'Input:')
    .describe('url', 'Enter one URL as parameter')
    .describe('stdin', 'Take input URLs from stdin')
    .group(['output-json', 'output-values-string', 'print-only-header'], 'Output formats:')
    .describe('output-json', 'prints result as JSON')
    .describe('output-values-string', 'prints results as comma separated string (suited for .csv)')
    .describe('print-only-header', 'prints CSV header and exits')
    .group(['include-values', 'include-url', 'include-header'], 'Extra informations in output:')
    .describe('include-values', 'include also extra intermediate values')
    .describe('include-url', 'include to the output given URL')
    .describe('include-header', 'include also header line for the csv format')
    // features
    .describe('feat-input-tag', 'check how many input tags has page')
    .describe('feat-src-link', 'check if src=<link> is matching hostname')
    .describe('feat-form-handler', 'check <form> handlers')
    .describe('feat-invisible-iframe', 'check invisible <iframe> elements')
    .describe('feat-rewrite-statusbar', 'check rewriting a status bar')
    .describe('feat-disable-rightclick', 'check that page has disabled right-click')
    .describe('feat-ahref-link', 'check <a href="LINK"> pointing to outer world')
    .describe('feat-popup-window', 'check PopUp windows')
    .describe('feat-favicon-link', 'check if favicon is pointing to another site')
    .describe('feat-old-technologies', 'check if site is not running new technologies')
    .describe('feat-missleading-link', 'check text values of <a href> links')
    .describe('feat-hostname-title', 'check if hostname is matching title')
  return args
}

const setFeaturesToNegativeValue = (url, features, columnNames, argv) => {
  if (argv.argv.server || argv.argv.outputJson) {
    const json = {}
    if (argv.argv.includeUrl) {
      json.url = url
    }
    Object.keys(features).forEach(feature => { json[columnNames[feature]] = -1 })
    return json
  } else {
    const csvLine = []
    if (argv.argv.includeUrl) {
      csvLine.push(`"${url}"`)
    }
    const size = Object.keys(features).length
    csvLine.push(Array(size).fill(-1))
    return csvLine.join(',')
  }
}

const checkUrl = async (url, features, argv, verbose) => {
  if (verbose) console.log(`Checking ${url}`)
  const page = new Page(url, argv.argv.includeValues, verbose)

  let badUrl = false
  const results = await page.performTests(features).catch(err => {
    if (verbose) console.error(err.message)
    badUrl = true
    return setFeaturesToNegativeValue(url, features, page.columns, argv)
  })

  if (badUrl) {
    return results
  }

  if (argv.argv.outputJson) {
    return results
  } else if (argv.argv.outputValuesString) {
    const csvLine = []
    if (argv.argv.includeUrl) {
      csvLine.push(`"${url}"`)
    }
    Object.keys(results).forEach(key => csvLine.push(results[key]))
    return csvLine.join(',')
  }
}

const parseFeatures = argv => {
  const features = {}
  const includeValues = argv.argv.includeValues
  // We are relying on the order. check c++ file service/src/features/feature_enum.h
  if (argv.argv.featInputTag) {
    features.inputTag = 'inputTag'
    if (includeValues) features.inputTagValue = 'inputTagValue'
  }
  if (argv.argv.featSrcLink) {
    features.srcLink = 'srcLink'
  }
  if (argv.argv.featFormHandler) {
    features.formHandler = 'formHandler'
    if (includeValues) features.formHandlerValue = 'formHandlerValue'
  }
  if (argv.argv.featInvisibleIframe) {
    features.invisibleIframe = 'invisibleIframe'
    if (includeValues) features.invisibleIframeValue = 'invisibleIframeValue'
  }
  if (argv.argv.featRewriteStatusbar) {
    features.rewriteStatusbar = 'rewriteStatusbar'
  }
  if (argv.argv.featDisableRightclick) {
    features.disableRightclick = 'disableRightclick'
  }
  if (argv.argv.featAhrefLink) {
    features.ahrefLink = 'ahrefLink'
  }
  if (argv.argv.featPopupWindow) {
    features.popupWindow = 'popupWindow'
    if (includeValues) features.popupWindowValue = 'popupWindowValue'
  }
  if (argv.argv.featFaviconLink) {
    features.faviconLink = 'faviconLink'
  }
  if (argv.argv.featOldTechnologies) {
    features.oldTechnologies = 'oldTechnologies'
  }
  if (argv.argv.featMissleadingLink) {
    features.missleadingLink = 'missleadingLink'
    if (includeValues) features.missleadingLinkValue = 'missleadingLinkValue'
  }
  if (argv.argv.featHostnameTitle) {
    features.hostnameTitle = 'hostnameTitle'
  }
  return features
}

const runServer = (port, verbose) => {
  const server = net.createServer(connection => {
    console.log('Client connected')
    connection.on('end', () => {
      console.log('Client disconnected')
    })
    connection.on('data', async data => {
      console.log(`Request: ${data}`)
      try {
        const json = JSON.parse(data) // throws an exception that we are handling later
        const url = json.url
        if (!url) {
          const response = {
            error: 'HTML_ANALYSIS_MODULE',
            message: 'Missing "url" field'
          }
          console.log(`Response: ${JSON.stringify(response)}`)
          connection.write(JSON.stringify(response))
          return
        }

        // prepare arguments
        const argv = {
          argv: {
            outputJson: true,
            ...json.features
          }
        }
        const features = parseFeatures(argv)
        if (Object.keys(features).length === 0) {
          const response = {
            error: 'HTML_ANALYSIS_MODULE',
            message: 'Missing what features we need to check'
          }
          console.log(`Response: ${JSON.stringify(response)}`)
          connection.write(JSON.stringify(response))
          return
        }
        verbose && console.log(`Checking following features: ${JSON.stringify(features)}`)

        const response = await checkUrl(url, features, argv, verbose)
        console.log(`Response: ${JSON.stringify(response)}`)
        connection.write(JSON.stringify(response))
      } catch (err) {
        const response = {
          error: 'HTML_ANALYSIS_MODULE',
          message: err.message
        }
        console.log(`Response: ${JSON.stringify(response)}`)
        connection.write(JSON.stringify(response))
      }
    })
  })
  server.on('error', (err) => {
    throw err
  })
  server.listen(port, () => {
    console.log(`Starting server for HTML features analysis on port ${port}`)
  })
}

const getCsvHeader = (features, includeUrl) => {
  const page = new Page('http://example.com')
  const columns = []
  if (includeUrl) {
    columns.push('url')
  }
  Object.keys(features).forEach(key => columns.push(page.columns[key]))
  return columns.join(',')
}

const main = async () => {
  const argv = parseCmdline()
  const verbose = argv.argv.verbose

  if (argv.argv.server) {
    const port = argv.argv.server
    return runServer(port, verbose)
  }

  const features = parseFeatures(argv)

  if (argv.argv.printOnlyHeader) {
    const header = getCsvHeader(features, argv.argv.includeUrl)
    return console.log(header)
  }

  if (!argv.argv.stdin && !argv.argv.url) {
    console.error('You have to provide URL to check or start as "--stdin"')
    process.exit(1)
  }

  const urls = []
  if (argv.argv.url) {
    urls.push(argv.argv.url)
  } else if (argv.argv.stdin) {
    const rl = readline.createInterface({
      input: process.stdin
    })
    for await (const line of rl) {
      urls.push(line)
    }
  }

  if (!argv.argv.outputJson && argv.argv.includeHeader) {
    const header = getCsvHeader(features, argv.argv.includeUrl)
    console.log(header)
  }

  for await (const url of urls) {
    const results = await checkUrl(url, features, argv, verbose)
    if (argv.argv.outputJson) {
      // JSON
      console.log(JSON.stringify(results))
    } else {
      // CSV line
      console.log(results)
    }
  }
  process.exit(0)
}

if (require.main === module) {
  main()
}
