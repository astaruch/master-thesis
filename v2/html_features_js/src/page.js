'use strict'

const fetch = require('node-fetch')
const { JSDOM } = require('jsdom')

class Page {
  constructor(url) {
    this.url = url
    this.columns = {
      inputTag: 'input_tag',
      srcLink: 'src_link',
      formHandler: 'form_handler',
    }
    this.tests = {
      // Without explicit binding, we would not have 'this' inside these functions
      inputTag: this.featureInputTagTest.bind(this),
      srcLink: this.featureSrcLinkTest.bind(this),
      formHandler: this.featureFormHandlerTest.bind(this),
    }
    this.parsed = new URL(url)
  }

  async getDOM(url) {
    const page = await fetch(url, {
      headers: {
        'Content-Type': 'text/html; charset=UTF-8'
      },
    }).then(res => res.text())
    return new JSDOM(page)
  }

  /**
   * Compute feature value for requested feateures
   * @param {Object} features Object containing columns
   * @returns {Object} Object with value for each feature
   */
  async performTests(features) {
    const dom = await this.getDOM(this.url)
    const results = {}
    Object.keys(features).forEach(feature => {
      results[this.columns[feature]] = this.tests[feature](dom, this.parsed)
    })
    return results
  }

  /**
   *
   * @param {JSDOM} dom Webpage DOM
   * @returns {Number} number of input elements
   */
  featureInputTag(dom) {
    return dom.window.document.querySelectorAll('input').length
  }

  /**
   *
   * @param {JSDOM} dom WebPage DOM
   * @returns {Number} computed phishing value for this feature
   */
   featureInputTagTest(dom) {
    const value = this.featureInputTag(dom)
    return value > 0 ? 1 : 0
  }

  /**
   *
   * @param {JSDOM} dom WebPage DOM
   * @param {URL} url URL which is investigated
   * @returns {Number} ratio of links pointing to itself vs outer world
   */
  featureSrcLink(dom, url) {
    // https://developer.mozilla.org/en-US/docs/Web/API/NodeList
    // https://developer.mozilla.org/en-US/docs/Web/API/Element
    let differentLinks = 0
    let allLinks = 0
    dom.window.document.querySelectorAll('*').forEach(node => {
      const src = node.attributes.getNamedItem('src')
      if (!src) {
        return
      }
      try {
        allLinks += 1
        const srcUrl = new URL(src.value)
        if (srcUrl.hostname !== url.hostname) {
          differentLinks += 1
        }
      } catch (err) {
        if (err instanceof TypeError) {
          // this is if src=<link> is relative URL. We are not interested in these URL
        }

      }
    })
    return differentLinks / allLinks;
  }

  /**
   *
   * @param {JSDOM} dom WebPage DOM
   * @param {URL} url URL which is investigated
   * @returns {Number} computed phishing value for this feature
   */
  featureSrcLinkTest(dom, url) {
    const result = this.featureSrcLink(dom, url)
    return result > 0.8 ? 1 : 0
  }

  featureFormHandler(dom) {
    let count = 0
    const suspiciousActions = ['', 'about:blank', 'blank', '#skip', '#', 'javascript:true' ]
    dom.window.document.querySelectorAll('form').forEach(node => {
      const action = node.attributes.getNamedItem('action')
      if (!action) {
        return
      }
      if (suspiciousActions.includes(action)) {
        count++
      }
      console.log(action.value)
    })
    return count
  }

  featureFormHandlerTest(dom) {
    const result = this.featureFormHandler(dom)
    return result > 1 ? 1 : 0
  }
}

module.exports = Page
