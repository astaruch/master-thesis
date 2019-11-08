'use strict'

const Page = require('../src/page')
const assert = require('assert')
const { JSDOM } = require('jsdom')

describe('FeatureTests', function () {
  describe('rewriteStatusbar', function () {
    const url = 'https://google.com'
    const DOM1 = `
  <div>
    <a href="http://phishing.com"
        onmouseover="window.status='http://legit-site.com';"
        onmouseout="window.status='http://this-site.com';"/>Figures for February’s racing.
  </div>
`

    const DOM2 = `
<div>
  <a href="http://phishing.com"
      onmouseover="this.src='map_location_revealed.gif';"
      onmouseout="this.src='map.gif';"/>Figures for February’s
</div>
`

    const DOM3 = `
<html>
  <iframe width="0" height="0" frameborder="0" src="http://loadus.exelator.com/load/?p=258&g=024&c=23706&ctg=modeling&j=w">
</iframe>
</html>
`
    const page = new Page(url)

    it('phishy onmouseover should return 1', () => {
      const DOM = new JSDOM(DOM1)
      const forms = page.featureRewriteStatusbar(DOM)
      assert.strictEqual(forms, 1)
    })
    it('legit onmouseover should return 0', () => {
      const DOM = new JSDOM(DOM2)
      const forms = page.featureRewriteStatusbar(DOM)
      assert.strictEqual(forms, 0)
    })
    it('no event should return 0', () => {
      const DOM = new JSDOM(DOM3)
      const forms = page.featureRewriteStatusbar(DOM)
      assert.strictEqual(forms, 0)
    })
  })
})
