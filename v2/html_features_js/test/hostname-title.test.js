'use strict'

const Page = require('../src/page')
const assert = require('assert')
const { JSDOM } = require('jsdom')


describe('FeatureTests', function() {
  describe('hostnameTitle', function() {
    const url = 'https://google.com'
    const DOM1 = `
  <html>
  <title>PhishTank | Join the fight against phishing</title>
  </html>
  `
    const DOM2 = `
  <html>
  <title>Google</title>
  </html>
   `

    const page = new Page(url)

    it('different title should return 1', () => {
      const DOM = new JSDOM(DOM1)
      const value = page.featureHostnameTitle(DOM)
      assert.strictEqual(value, 1)
    })
    it('same title should return 0', () => {
      const DOM = new JSDOM(DOM2)
      const value = page.featureHostnameTitle(DOM)
      assert.strictEqual(value, 0)
    })
  })
})
