
import { expect } from 'chai'

describe('Tests', function () {

  before(async function () {

    this.timeout(60000)

    /*
    ************************************************************************
    * Setup two child processes that I can communicate with
    ************************************************************************
     */
    console.log('Test Index')

  })

  it('Should verify initialization', async () => {

    expect(1).to.be.equal(1)

  })

})
