
import { expect } from 'chai'
import { elastosCarrier } from '../carrier_stub'

describe('Tests for Carrier Stub', function () {
  before(async function () {

    this.timeout(60000)

    /*
    ************************************************************************
    * Setup two child processes that I can communicate with
    ************************************************************************
     */
    console.log('Carrier Stub')

  })

  it('Should call new_ela', async function() {

    this.timeout(5000)

    let msg = await elastosCarrier.test()

    expect(msg).to.be.equal('Hello World')

  })

})
