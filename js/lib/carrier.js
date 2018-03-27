
const carrier = require('../../build/Release/carrier')

module.exports = {
  create: function create (greeting) {
    return carrier.new(greeting)
  }
}
