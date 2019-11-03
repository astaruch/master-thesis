'use strict'

const Page = require('../src/page')
const assert = require('assert')
const { JSDOM } = require('jsdom')


describe('FeatureTests', function() {
  describe('missleadingLink', function() {
    const url = 'https://google.com'
    const DOM1 = `
  <html>
  <a href="http://phishing-site.com">www.google.com</a>
  </html>
  `
    const DOM2 = `
  <html>
    <a href="http://google.com">google.com</a>
  </html>
    `

    const page = new Page(url)

    it('missleading link should return 1', () => {
      const DOM = new JSDOM(DOM1)
      const value = page.featureMissleadingLink(DOM)
      assert.strictEqual(value, 1)
    })
    it('normal link should return 0', () => {
      const DOM = new JSDOM(DOM2)
      const value = page.featureMissleadingLink(DOM)
      assert.strictEqual(value, 0)
    })
  })
})
