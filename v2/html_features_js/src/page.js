'use strict'

const fetch = require('node-fetch')
const { JSDOM, VirtualConsole } = require('jsdom')

class Page {
  constructor(url, includeValues) {
    this.url = url
    this.columns = {
      inputTag: 'input_tag',
      srcLink: 'src_link',
      formHandler: 'form_handler',
      invisibleIframe: 'invisible_iframe',
      rewriteStatusbar: 'rewrite_statusbar',
      disableRightclick: 'disable_rightclick',
      ahrefLink: 'ahref_link',
      popupWindow: 'popup_window',
      faviconLink: 'favicon_link',
      oldTechnologies: 'old_technologies',
      missleadingLink: 'missleading_link',
      hostnameTitle: 'hostname_title'
    }
    this.tests = {
      // Without explicit binding, we would not have 'this' inside these functions
      inputTag: this.featureInputTagTest.bind(this),
      srcLink: this.featureSrcLinkTest.bind(this),
      formHandler: this.featureFormHandlerTest.bind(this),
      invisibleIframe: this.featureInvisibleIframeTest.bind(this),
      rewriteStatusbar: this.featureRewriteStatusbarTest.bind(this),
      disableRightclick: this.featureDisableRightclickTest.bind(this),
      ahrefLink: this.featureAhrefLinkTest.bind(this),
      popupWindow: this.featurePopupWindowTest.bind(this),
      faviconLink: this.featureFaviconLinkTest.bind(this),
      oldTechnologies: this.featureOldTechnologiesTest.bind(this),
      missleadingLink: this.featureMissleadingLinkTest.bind(this),
      hostnameTitle: this.featureHostnameTitleTest.bind(this)
    }
    this.includeValues = includeValues
    this.parsed = new URL(url)
  }

  async getDOM(url) {
    const page = await fetch(url, {
      headers: {
        'Content-Type': 'text/html; charset=UTF-8'
      },
    }).then(res => res.text())
    const virtualConsole = new VirtualConsole()
    return new JSDOM(page, { virtualConsole })
  }

  /**
   * Compute feature value for requested feateures
   * @param {Object} features Object containing columns
   * @returns {Object} Object with value for each feature
   */
  async performTests(features) {
    const dom = await this.getDOM(this.url)
    const results = {}
    Object.keys(features).forEach(feature => {
      results[this.columns[feature]] = this.tests[feature](dom, this.parsed)
      if (this.includeValues) {
        const valueProp = `${feature}Value`
        if (this[valueProp]) {
          results[`${this.columns[feature]}_value`] = this[valueProp]
        }
        // results[`${this.columns[feature]}_value`] = this[`${feature}Value`]
      }
    })
    return results
  }

  /**
   *
   * @param {JSDOM} dom Webpage DOM
   * @returns {Number} number of input elements
   */
  featureInputTag(dom) {
    this.inputTagValue = dom.window.document.querySelectorAll('input').length
    return this.inputTagValue
  }

  /**
   *
   * @param {JSDOM} dom WebPage DOM
   * @returns {Number} computed phishing value for this feature
   */
   featureInputTagTest(dom) {
    const value = this.featureInputTag(dom)
    return value > 0 ? 1 : 0
  }

  /**
   *
   * @param {JSDOM} dom WebPage DOM
   * @param {URL} url URL which is investigated
   * @returns {Number} computed phishing value for this feature
   */
  featureSrcLinkTest(dom, url) {
    // https://developer.mozilla.org/en-US/docs/Web/API/NodeList
    // https://developer.mozilla.org/en-US/docs/Web/API/Element
    let differentLinks = 0
    let allLinks = 0
    dom.window.document.querySelectorAll('*').forEach(node => {
      const src = node.attributes.getNamedItem('src')
      if (!src) {
        return
      }
      try {
        allLinks += 1
        const srcUrl = new URL(src.value)
        if (srcUrl.hostname !== url.hostname) {
          differentLinks += 1
        }
      } catch (err) {
        if (err instanceof TypeError) {
          // this is if src=<link> is relative URL. We are not interested in these URL
        }

      }
    })
    return (differentLinks / allLinks) || 0;
}

  featureFormHandler(dom) {
    let count = 0
    // eslint-disable-next-line no-script-url
    const suspiciousActions = ['', 'about:blank', 'blank', '#skip', '#', 'javascript:true' ]
    dom.window.document.querySelectorAll('form').forEach(node => {
      const action = node.attributes.getNamedItem('action')
      if (!action) {
        return
      }
      if (suspiciousActions.includes(action.value)) {
        count++
      }
    })
    this.formHandlerValue = count
    return count
  }

  featureFormHandlerTest(dom) {
    const result = this.featureFormHandler(dom)
    return result > 1 ? 1 : 0
  }

  featureInvisibleIframe(dom) {
    let count = 0
    dom.window.document.querySelectorAll('iframe').forEach(node => {
      // https://www.w3schools.com/tags/att_iframe_seamless.asp
      const seamless = node.attributes.getNamedItem('seamless')
      if (seamless) {
        count++
        return
      }
      // 0x0 dimensions
      let height = node.attributes.getNamedItem('height')
      let width = node.attributes.getNamedItem('width')
      if (!height || !width) {
        return
      }
      height = Number(height.value)
      width = Number(width.value)
      if (height === 0 && width === 0) {
        count++
      }
    })
    this.invisibleIframeValue = count
    return count
  }

  featureInvisibleIframeTest(dom) {
    const iframes = this.featureInvisibleIframe(dom)
    return iframes > 0 ? 1 : 0
  }

  featureRewriteStatusbarTest(dom) {
    let rewriting = false
    dom.window.document.querySelectorAll('a').forEach(node => {
      const onmouseover = node.attributes.getNamedItem('onmouseover')
      const onmouseout = node.attributes.getNamedItem('onmouseout')
      if ((onmouseover && onmouseover.value.startsWith('window.status')) ||
          (onmouseout && onmouseout.value.startsWith('window.status')))
      {
        rewriting = true
      }
    })
    return Number(rewriting)
  }

  featureDisableRightclick(dom) {
    const body = dom.window.document.body
    if (!body && !body.attributes) {
      return 1
    }
    const oncontextmenu = body.attributes.getNamedItem('oncontextmenu')
    if (oncontextmenu && oncontextmenu.value.includes('return false')) {
      return 1
    }
    return 0
  }

  featureDisableRightclickTest(dom) {
    const body = dom.window.document.body
    if (!body && !body.attributes) {
      return 1
    }
    const oncontextmenu = body.attributes.getNamedItem('oncontextmenu')
    if (oncontextmenu && oncontextmenu.value.includes('return false')) {
      return 1
    }
    return 0
  }

  featureAhrefLinkTest(dom) {
    let count = 0
    let allLinks = 0
    dom.window.document.querySelectorAll('a').forEach(node => {
      const href = node.attributes.getNamedItem('href')
      if (!href) {
        return
      }
      try {
        allLinks += 1
        const hrefLink = new URL(href.value)
        if (hrefLink.hostname !== this.parsed.hostname) {
          count += 1
        }
      } catch (err) {
        if (err instanceof TypeError) {
          // href is relative URL
        }
      }
    })
    return (count / allLinks) || 0
  }

  featurePopupWindow(dom) {
    let count = 0
    const scripts = dom.window.document.querySelectorAll('script')
    const popupStrings = ['prompt(', 'alert(', 'confirm(']
    dom.window.document.querySelectorAll('*').forEach(node => {
      const onclick = node.attributes.getNamedItem('onclick')
      if (!onclick) {
        return
      }
      for (let popup of popupStrings) {
        if (onclick.value.includes(popup)) {
          count++
          return
        }
      }
      // check each <script></script> tag that if contains popup string.
      // we are checking only text fields without dowlnoading resources
      scripts.forEach(script => {
        if (script.attributes.length > 0) {
          return
        }
        for (let popup of popupStrings) {
          if (script.text.includes(popup)) {
            count++
            return
          }
        }
      })
    })
    this.popupWindowValue = count
    return count
  }

  featurePopupWindowTest(dom) {
    return this.featurePopupWindow(dom) > 0 ? 1 : 0
  }

  featureFaviconLinkTest(dom) {
    let anotherSite = false
    dom.window.document.querySelectorAll('link').forEach(node => {
      const rel = node.attributes.getNamedItem('rel')
      if (!rel) {
        return
      }
      if (!rel.value.includes('icon')) {
        return
      }
      const href = node.attributes.getNamedItem('href')
      try {
        const faviconURL = new URL(href.value)
        // we constructed faviconURL, need to check if hostname are same
        if (faviconURL.hostname !== this.parsed.hostname) {
          anotherSite = true
          return
        }
      } catch (err) {
        if (err instanceof TypeError) {
          // favicon URL is relative url => it's poiting to this site
          // thus it's not phishing
        }
      }
    })
    return Number(anotherSite)
  }

  featureOldTechnologiesTest(dom) {
    const window = dom.window
    const document = dom.window.document
    if(!!window.React || !!document.querySelector('[data-reactroot], [data-reactid]')) {
      return 0
    }

    if(!!window.angular ||
      !!document.querySelector('.ng-binding, [ng-app], [data-ng-app], [ng-controller], [data-ng-controller], [ng-repeat], [data-ng-repeat]') ||
      !!document.querySelector('script[src*="angular.js"], script[src*="angular.min.js"]'))
    {
      return 0
    }

    if(!!window.Backbone) return 0
    if(!!window.Ember) return 0
    if(!!window.Vue) return 0
    if(!!window.Meteor) return 0

    return 1
  }

  featureMissleadingLink(dom) {
    let value = 0
    dom.window.document.querySelectorAll('a').forEach(node => {
      let text = node.text
      const href = node.attributes.getNamedItem('href')
      if (!text || !href) {
        return
      }
      try {
        // text may be anything. but if it's web (e.g. www.google.com)
        // we need to add protocol.
        if (!text.startsWith('http')) {
          text = 'http://' + text
        }
        // we are expecting that these calls will throw
        const displayedURL = new URL(text)
        const actualURL = new URL(href.value)
        // the links are pointing to different sites
        if (displayedURL.hostname !== actualURL.hostname) {
          value++
          return
        }
      } catch (err) {
        if (err instanceof TypeError) {
          // the links are either relative links or the text is not URL
          // we are not interested in them
        }
      }
    })
    this.missleadingLinkValue = value
    return value
  }

  featureMissleadingLinkTest(dom) {
    return this.featureMissleadingLink(dom) > 0 ? 1 : 0
  }

  featureHostnameTitleTest(dom) {
    const title = dom.window.document.title.toLowerCase()
    let hostname = this.parsed.hostname.match(/(?:www\.)?(?<HOST>.*)(?:\..*)/u)
    if (!hostname) {
      return 0
    }
    hostname = hostname.groups['HOST'].toLowerCase()
    if (title.includes(hostname)) {
      return 0
    }
    return 1
  }
}

module.exports = Page
