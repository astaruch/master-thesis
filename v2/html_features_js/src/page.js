'use strict'

const fetch = require('node-fetch')
const { JSDOM } = require('jsdom')

class Page {
  constructor(url) {
    // console.log(`${url}`)
    this.url = url
    this.columns = {
      inputTag:'input_tag'
    }
    this.tests = {
      inputTag: this.featureInputTag
    }
  }

  async getDOM(url) {
    const page = await fetch(url, {
      headers: {
        'Content-Type': 'text/html; charset=UTF-8'
      },
    }).then(res => res.text())
    return new JSDOM(page)
  }

  async performTests(features) {
    const dom = await this.getDOM(this.url)
    const results = {}
    Object.keys(features).forEach(feature => {
      // console.log(`Performing test: ${feature}`)
      results[this.columns[feature]] = this.tests[feature](dom)
    })
    return results
  }

  featureInputTag(dom) {
    return dom.window.document.querySelectorAll('input').length > 0 ? 1 : 0
  }
}

module.exports = Page
