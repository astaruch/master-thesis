'use strict'

const Page = require('../src/page')
const assert = require('assert')
const { JSDOM } = require('jsdom')


describe('FeatureTests', function() {
  describe('formHandler', function() {
    const url = 'https://google.com'
    const DOM1 = `
<html>
  <form action='#'/>
</html>
`
    const DOM2 = `
<html>
  <form action=''/>
</html>
`
    const DOM3 = `
<html>
  <form action='about:blank'/>
</html>
`
    const DOM4 = `
<html>
  <form action='javascript:true'/>
</html>
`
    const DOM5 = `
<html>
  <form action='#skip'/>
</html>
`

    const DOM6 = `
<html>
  <form action='blank'/>
</html>
`

    const DOM7 = `
<html>
  <form action='javascript'/>
</html>
`
    const page = new Page(url)

    it('should return 1', () => {
      const DOM = new JSDOM(DOM1)
      const forms = page.featureFormHandler(DOM)
      assert.strictEqual(forms, 1)
    })
    it('should return 1', () => {
      const DOM = new JSDOM(DOM2)
      const forms = page.featureFormHandler(DOM)
      assert.strictEqual(forms, 1)
    })
    it('should return 1', () => {
      const DOM = new JSDOM(DOM3)
      const forms = page.featureFormHandler(DOM)
      assert.strictEqual(forms, 1)
    })
    it('should return 1', () => {
      const DOM = new JSDOM(DOM4)
      const forms = page.featureFormHandler(DOM)
      assert.strictEqual(forms, 1)
    })
    it('should return 1', () => {
      const DOM = new JSDOM(DOM5)
      const forms = page.featureFormHandler(DOM)
      assert.strictEqual(forms, 1)
    })
    it('should return 1', () => {
      const DOM = new JSDOM(DOM6)
      const forms = page.featureFormHandler(DOM)
      assert.strictEqual(forms, 1)
    })
    it('should return 0', () => {
      const DOM = new JSDOM(DOM7)
      const forms = page.featureFormHandler(DOM)
      assert.strictEqual(forms, 0)
    })

  })
})
