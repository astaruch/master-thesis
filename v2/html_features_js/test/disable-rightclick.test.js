'use strict'

const Page = require('../src/page')
const assert = require('assert')
const { JSDOM } = require('jsdom')


describe('FeatureTests', function() {
  describe('disableRightclick', function() {
    const url = 'https://google.com'
    const DOM1 = `<body oncontextmenu="return false;">`

    const DOM2 = `
<div>
  <a href="http://phishing.com"
      onmouseover="this.src='map_location_revealed.gif';"
      onmouseout="this.src='map.gif';"/>Figures for February’s
</div>
`
    const page = new Page(url)

    it('disabled rightlick should return 1', () => {
      const DOM = new JSDOM(DOM1)
      const forms = page.featureDisableRightclick(DOM)
      assert.strictEqual(forms, 1)
    })
    it('enabled rightlick should return 0', () => {
      const DOM = new JSDOM(DOM2)
      const forms = page.featureDisableRightclick(DOM)
      assert.strictEqual(forms, 0)
    })
  })
})
