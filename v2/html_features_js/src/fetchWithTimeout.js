'use strict'
const fetch = require('node-fetch')

const fetchWithTimeout = function (url, options, timeout = 7000) {
  return Promise.race([
    fetch(url, options),
    new Promise((_, reject) =>
    setTimeout(() => reject(new Error('ERROR: Timeout')), timeout)
    )
  ]);
}

module.exports = { fetchWithTimeout }
