'use strict'

const Page = require('../src/page')
const assert = require('assert')
const { JSDOM } = require('jsdom')

describe('FeatureTests', function () {
  describe('srcLink', function () {
    const url = 'https://google.com'
    const DOM1 = `
<html>
  <body>
    <p><img src="http://phishing.com/htmlpage.jpg"></p>
    <p><img src="http://phishing.com/htmlpage2.jpg"></p>
    <p><img src="http://google.com/htmlpage.jpg"></p>
    <p><img src="htmlpage.jpg"></p>
  </body>
</html>
`

    const DOM2 = `
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

    it('should return 0.75 as phishing value', () => {
      const DOM = new JSDOM(DOM1)
      const value = page.featureSrcLink(DOM, url)
      assert.strictEqual(value, 0.75)
    })

    it('should return 0 as phishing value', () => {
      const DOM = new JSDOM(DOM2)
      const value = page.featureSrcLink(DOM)
      assert.strictEqual(value, 0)
    })
  })
})
