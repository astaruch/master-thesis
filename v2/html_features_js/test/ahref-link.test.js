'use strict'

const Page = require('../src/page')
const assert = require('assert')
const { JSDOM } = require('jsdom')


describe('FeatureTests', function() {
  describe('ahrefLink', function() {
    const url = 'https://google.com'
    const DOM1 = `
<html>
  <body>
    <a href="#start-of-content" tabindex="1" class="p-3 bg-blue text-white show-on-focus js-skip-to-content">Skip to content</a>
    <a class="Header-link" href="https://github.com/" data-hotkey="g d" aria-label="Homepage" data-ga-click="Header, go to dashboard, icon:logo">
    <a class="Header-link d-block d-lg-none py-2 py-lg-0 border-top border-lg-top-0 border-white-fade-15" data-ga-click="Header, click, Nav menu - item:dashboard:user" aria-label="Dashboard" href="/dashboard">
    Dashboard
  </a>
    <a role="menuitem" class="dropdown-item" href="/astaruch/address-book-api/issues/new" data-ga-click="Header, create new issue" data-skip-pjax>
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

    it('should return 0.25 (1/4) of links to outer world', () => {
      const DOM = new JSDOM(DOM1)
      const links = page.featureAhrefLinkTest(DOM, url)
      assert.strictEqual(links, 0.25)
    })

    it('should return 1 (1/1) as all links are to outer world', () => {
      const DOM = new JSDOM(DOM2)
      const value = page.featureAhrefLinkTest(DOM, url)
      assert.strictEqual(value, 1)
    })
  })
})
