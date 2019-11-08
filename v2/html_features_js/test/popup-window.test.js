'use strict'

const Page = require('../src/page')
const assert = require('assert')
const { JSDOM } = require('jsdom')

describe('FeatureTests', function () {
  describe('popupWindow', function () {
    const url = 'https://google.com'
    const DOM1 = `
<!DOCTYPE html>
<html>
<body>

<h2>JavaScript Prompt</h2>

<button onclick="myFunction()">Try it</button>

<p id="demo"></p>

<script>
function myFunction() {
  var txt;
  var person = prompt("Please enter your name:", "Harry Potter");
  if (person == null || person == "") {
    txt = "User cancelled the prompt.";
  } else {
    txt = "Hello " + person + "! How are you today?";
  }
  document.getElementById("demo").innerHTML = txt;
}
</script>

</body>
</html>
`
    const DOM2 = `
<!DOCTYPE html>
<html>
<body>

<h2>JavaScript</h2>
<p>Line-breaks in a popup box.</p>

<button onclick="alert('Hello\nHow are you?')">Try it</button>

</body>
</html>
`

    const DOM3 = `
<!DOCTYPE html>
<html>
<body>

<h2>JavaScript Alert</h2>

<button onclick="myFunction()">Try it</button>

<script>
function myFunction() {
  alert("I am an alert box!");
}
</script>

</body>
</html>
`

    const DOM4 = `
<!DOCTYPE html>
<html>
<body>

<h2>JavaScript</h2>
<p>Line-breaks in a popup box.</p>

<button onclick="confirm('Hello\nHow are you?')">Try it</button>

</body>
</html>
`

    const DOM5 = `
<!DOCTYPE html>
<html>
<body>

<button onclick="window.open('http://google.com')">Try it</button>

</body>
</html>
`
    const DOM6 = `
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
    const page = new Page(url)

    it('prompt inside function should return 1', () => {
      const DOM = new JSDOM(DOM1)
      const value = page.featurePopupWindow(DOM, url)
      assert.strictEqual(value, 1)
    })

    it('onclick alert should return 1 ', () => {
      const DOM = new JSDOM(DOM2)
      const value = page.featurePopupWindow(DOM, url)
      assert.strictEqual(value, 1)
    })

    it('alert inside function should return 1', () => {
      const DOM = new JSDOM(DOM3)
      const value = page.featurePopupWindow(DOM, url)
      assert.strictEqual(value, 1)
    })

    it('confirm should return 1 ', () => {
      const DOM = new JSDOM(DOM4)
      const value = page.featurePopupWindow(DOM, url)
      assert.strictEqual(value, 1)
    })

    it('window open should should return 0', () => {
      const DOM = new JSDOM(DOM5)
      const value = page.featurePopupWindow(DOM, url)
      assert.strictEqual(value, 0)
    })

    it('no popup should return 0', () => {
      const DOM = new JSDOM(DOM6)
      const value = page.featurePopupWindow(DOM, url)
      assert.strictEqual(value, 0)
    })
  })
})
