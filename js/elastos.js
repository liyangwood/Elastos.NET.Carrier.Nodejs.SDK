'use strict'

const carrier = require('./lib/carrier')

class Elastos {

  constructor () {

    this.PACKAGE_VERSION = require('../package.json').version
    this.carrier = carrier
  }

}

// we want a singleton so we reuse existing Carrier nodes
module.exports = new Elastos()

// expose constructor as a named property to enable mocking with Sinon.JS
module.exports.Elastos = Elastos
