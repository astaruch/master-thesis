'use strict'

const Page = require('../src/page')
const assert = require('assert')
const { JSDOM } = require('jsdom')

const url = 'https://google.com'

describe('FeatureTests', function() {
  describe('inputTag', function() {
    const DOMInputTag = `
<form action="/action_page.php">
  First name: <input type="text" name="fname"><br>
  Last name: <input type="text" name="lname"><br>
  <input type="submit" value="Submit">
</form>
`

const DOMNoInputTag = `
<HTML>
  <HEAD>
    <TITLE>Your Title Here</TITLE>
  </HEAD>
  <BODY BGCOLOR="FFFFFF">
    <CENTER><IMG SRC="clouds.jpg" ALIGN="BOTTOM"> </CENTER>
    <HR>
    <a href="http://somegreatsite.com">Link Name</a>
    is a link to another nifty site
    <H1>This is a Header</H1>
    <H2>This is a Medium Header</H2>
    Send me mail at <a href="mailto:support@yourcompany.com">
    support@yourcompany.com</a>.
    <P> This is a new paragraph!
    <P> <B>This is a new paragraph!</B>
    <BR> <B><I>This is a new sentence without a paragraph break, in bold italics.</I></B>
    <HR>
  </BODY>
</HTML>
`
    const page = new Page(url)

    it('should return 3 <input> tags', () => {
      const DOM = new JSDOM(DOMInputTag)
      const tags = page.featureInputTag(DOM)
      assert.strictEqual(tags, 3)
    })

    it('should return 1 as phishing value', () => {
      const DOM = new JSDOM(DOMInputTag)
      const value = page.featureInputTagTest(DOM)
      assert.strictEqual(value, 1)
    })

    it('should return 0 <input> tags', () => {
      const DOM = new JSDOM(DOMNoInputTag)
      const tags = page.featureInputTag(DOM)
      assert.strictEqual(tags, 0)
    })

    it('should return 0 as phishing value', () => {
      const DOM = new JSDOM(DOMNoInputTag)
      const value = page.featureInputTagTest(DOM)
      assert.strictEqual(value, 0)
    })

  })
})
