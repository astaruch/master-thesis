'use strict'

// Put this file to the directory where your node.js code is located. This could be the root
// directory, or a subdirectory if your project consists of both node.js and browser code.
module.exports = {
  extends: [
    'standard',
  ],
  overrides: [{
    files: [
      'src/**/*.js',
      'test/**/*.js',
    ],
    env: {
      mocha: true
    }
  }],

}
