'use strict'

const { fetchWithTimeout } = require('./fetchWithTimeout')
const { AbortController } = require('abort-controller')
const { JSDOM, VirtualConsole } = require('jsdom')

function computeValue (min, value, max) {
  value = Math.max(min, Math.min(value, max))
  return (value - min) / (max - min)
}

class Page {
  constructor (url, includeValues, verbose) {
    this.url = url
    this.columns = {
      inputTag: 'input_tag',
      inputTagValue: 'input_tag_value',
      srcLink: 'src_link',
      formHandler: 'form_handler',
      formHandlerValue: 'form_handler_value',
      invisibleIframe: 'invisible_iframe',
      invisibleIframeValue: 'invisible_iframe_value',
      rewriteStatusbar: 'rewrite_statusbar',
      disableRightclick: 'disable_rightclick',
      ahrefLink: 'ahref_link',
      popupWindow: 'popup_window',
      popupWindowValue: 'popup_window_value',
      faviconLink: 'favicon_link',
      oldTechnologies: 'old_technologies',
      missleadingLink: 'missleading_link',
      missleadingLinkValue: 'missleading_link_value',
      hostnameTitle: 'hostname_title'
    }
    this.tests = {
      // Without explicit binding, we would not have 'this' inside these functions
      inputTag: this.featureInputTag.bind(this),
      inputTagValue: this.featureInputTagValue.bind(this),
      srcLink: this.featureSrcLink.bind(this),
      formHandler: this.featureFormHandler.bind(this),
      formHandlerValue: this.featureFormHandlerValue.bind(this),
      invisibleIframe: this.featureInvisibleIframe.bind(this),
      invisibleIframeValue: this.featureInvisibleIframeValue.bind(this),
      rewriteStatusbar: this.featureRewriteStatusbar.bind(this),
      disableRightclick: this.featureDisableRightclick.bind(this),
      ahrefLink: this.featureAhrefLink.bind(this),
      popupWindow: this.featurePopupWindow.bind(this),
      popupWindowValue: this.featurePopupWindowValue.bind(this),
      faviconLink: this.featureFaviconLink.bind(this),
      oldTechnologies: this.featureOldTechnologies.bind(this),
      missleadingLink: this.featureMissleadingLink.bind(this),
      missleadingLinkValue: this.featureMissleadingLinkValue.bind(this),
      hostnameTitle: this.featureHostnameTitle.bind(this)
    }
    this.includeValues = includeValues
    this.parsed = new URL(url)
    this.verbose = verbose
  }

  async getDOM (url) {
    // we need to have a chance cancel weird request such as hanging 'http://w107fm.com/wp-admin/css/web/bt.com/clients/'
    const controller = new AbortController()
    const signal = controller.signal

    const abortFetching = () => {
      if (this.verbose) console.log(`Abort sent after 5s for '${this.url}'`)
      controller.abort()
    }

    const abortTimeout = setTimeout(abortFetching, 5000)

    const page = await fetchWithTimeout(url, {
      headers: {
        'Content-Type': 'text/html; charset=UTF-8'
      },
      signal: signal
    }, 3000)
      .then(res => res.text())
      .catch(err => {
        if (err.code === 'ENOTFOUND') {
          throw new Error('WARNING: Site is down')
        } else if (err.name === 'AbortError') {
          throw new Error('WARNING: Request aborted (site didnt sent request in 5s)')
        }
        throw err
      })

    if (page.length === 0) {
      throw new Error('WARNING: Empty page')
    }
    // we don't need to send abort cause we have page
    clearTimeout(abortTimeout)
    const virtualConsole = new VirtualConsole()
    return new JSDOM(page, { virtualConsole })
  }

  /**
   * Compute feature value for requested feateures
   * @param {Object} features Object containing columns
   * @returns {Object} Object with value for each feature
   */
  async performTests (features) {
    const dom = await this.getDOM(this.url).catch(err => {
      throw err
    })
    const results = {}
    Object.keys(features).forEach(feature => {
      results[this.columns[feature]] = this.tests[feature](dom, this.parsed)
    })
    return results
  }

  /**
   *
   * @param {JSDOM} dom Webpage DOM
   * @returns {Number} number of input elements
   */
  featureInputTagValue (dom) {
    this.inputTagValue = dom.window.document.querySelectorAll('input').length
    return this.inputTagValue
  }

  /**
   *
   * @param {JSDOM} dom WebPage DOM
   * @returns {Number} computed phishing value for this feature
   */
  featureInputTag (dom) {
    const value = this.featureInputTagValue(dom)
    return computeValue(0, value, 50)
  }

  /**
   *
   * @param {JSDOM} dom WebPage DOM
   * @param {URL} url URL which is investigated
   * @returns {Number} computed phishing value for this feature
   */
  featureSrcLink (dom, url) {
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
    return (differentLinks / allLinks) || 0
  }

  featureFormHandlerValue (dom) {
    let count = 0
    // eslint-disable-next-line no-script-url
    const suspiciousActions = ['', 'about:blank', 'blank', '#skip', '#', 'javascript:true']
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

  featureFormHandler (dom) {
    const result = this.featureFormHandlerValue(dom)
    return computeValue(0, result, 5)
  }

  featureInvisibleIframeValue (dom) {
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

  featureInvisibleIframe (dom) {
    const iframes = this.featureInvisibleIframeValue(dom)
    return computeValue(0, iframes, 12)
  }

  featureRewriteStatusbar (dom) {
    let rewriting = false
    dom.window.document.querySelectorAll('a').forEach(node => {
      const onmouseover = node.attributes.getNamedItem('onmouseover')
      const onmouseout = node.attributes.getNamedItem('onmouseout')
      if ((onmouseover && onmouseover.value.startsWith('window.status')) ||
          (onmouseout && onmouseout.value.startsWith('window.status'))) {
        rewriting = true
      }
    })
    return Number(rewriting)
  }

  featureDisableRightclick (dom) {
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

  featureAhrefLink (dom) {
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

  featurePopupWindowValue (dom) {
    let count = 0
    const scripts = dom.window.document.querySelectorAll('script')
    const popupStrings = ['prompt(', 'alert(', 'confirm(']
    dom.window.document.querySelectorAll('*').forEach(node => {
      const onclick = node.attributes.getNamedItem('onclick')
      if (!onclick) {
        return
      }
      for (const popup of popupStrings) {
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
        for (const popup of popupStrings) {
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

  featurePopupWindow (dom) {
    const value = this.featurePopupWindowValue(dom)
    return computeValue(0, value, 20)
  }

  featureFaviconLink (dom) {
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

  featureOldTechnologies (dom) {
    const window = dom.window
    const document = dom.window.document
    if (!!window.React || !!document.querySelector('[data-reactroot], [data-reactid]')) {
      return 0
    }

    if (!!window.angular ||
      !!document.querySelector('.ng-binding, [ng-app], [data-ng-app], [ng-controller], [data-ng-controller], [ng-repeat], [data-ng-repeat]') ||
      !!document.querySelector('script[src*="angular.js"], script[src*="angular.min.js"]')) {
      return 0
    }

    if (window.Backbone) return 0
    if (window.Ember) return 0
    if (window.Vue) return 0
    if (window.Meteor) return 0

    return 1
  }

  featureMissleadingLinkValue (dom) {
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

  featureMissleadingLink (dom) {
    const value = this.featureMissleadingLinkValue(dom)
    return computeValue(0, value, 100)
  }

  featureHostnameTitle (dom) {
    let title = dom.window.document.querySelector('title')
    if (!title || !title.text) {
      return 1
    }
    title = title.text.toLowerCase()
    let hostname = this.parsed.hostname.match(/(?:www\.)?(?<HOST>.*)(?:\..*)/u)
    if (!hostname) {
      return 0
    }
    hostname = hostname.groups.HOST.toLowerCase()
    if (title.includes(hostname)) {
      return 0
    }
    return 1
  }
}

module.exports = Page
