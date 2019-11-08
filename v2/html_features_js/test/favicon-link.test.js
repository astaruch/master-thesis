'use strict'

const Page = require('../src/page')
const assert = require('assert')
const { JSDOM } = require('jsdom')

describe('FeatureTests', function () {
  describe('faviconLink', function () {
    const url = 'https://google.com'
    const DOM1 = `
  <html>
    <link rel="icon" href="/favicon.ico" type="image/x-icon" />
  </html>
    `

    const DOM2 = `
<html>
  <link rel="shortcut icon" href="http://google.com/favicon.ico" type="image/x-icon" />
</html>
  `

    const DOM3 = `
<html>
  <link rel="icon shortcut" href="https://cdn.sstatic.net/Sites/stackoverflow/img/favicon.ico?v=4f32ecc8f43d" type="image/x-icon" />
</html>
  `

    const DOM4 = `
 <html/>
  `
    const page = new Page(url)

    it('relative domain should return 0', () => {
      const DOM = new JSDOM(DOM1)
      const value = page.featureFaviconLinkTest(DOM)
      assert.strictEqual(value, 0)
    })
    it('same absolute domain should return 0', () => {
      const DOM = new JSDOM(DOM2)
      const value = page.featureFaviconLinkTest(DOM)
      assert.strictEqual(value, 0)
    })
    it('external site should return 1', () => {
      const DOM = new JSDOM(DOM3)
      const value = page.featureFaviconLinkTest(DOM)
      assert.strictEqual(value, 1)
    })
    it('default favicon should return 0', () => {
      const DOM = new JSDOM(DOM4)
      const value = page.featureFaviconLinkTest(DOM)
      assert.strictEqual(value, 0)
    })
  })
})
