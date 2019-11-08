'use strict'

const Page = require('../src/page')
const assert = require('assert')
const { JSDOM } = require('jsdom')

describe('FeatureTests', function () {
  describe('invisibleIframe', function () {
    const url = 'https://google.com'
    const DOM1 = `
<html>
  <iframe width="700" scrolling="no" height="400" frameborder="0" src="yourFramePage.html" seamless="seamless">
</html>
`

    const DOM2 = `
<html>
  <iframe width="0" height="0" frameborder="0" src="http://loadus.exelator.com/load/?p=258&g=024&c=23706&ctg=modeling&j=w">
</iframe>
</html>
`

    const DOM3 = `
<form action="/action_page.php">
  First name: <input type="text" name="fname"><br>
  Last name: <input type="text" name="lname"><br>
  <input type="submit" value="Submit">
</form>
`
    const page = new Page(url)

    it('class seamless should return 1', () => {
      const DOM = new JSDOM(DOM1)
      const forms = page.featureInvisibleIframe(DOM)
      assert.strictEqual(forms, 1)
    })
    it('zero dimensions should return 1', () => {
      const DOM = new JSDOM(DOM2)
      const forms = page.featureInvisibleIframe(DOM)
      assert.strictEqual(forms, 1)
    })
    it('no iframe should return 0', () => {
      const DOM = new JSDOM(DOM3)
      const forms = page.featureInvisibleIframe(DOM)
      assert.strictEqual(forms, 0)
    })
  })
})
