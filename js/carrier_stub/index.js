


class ElastosCarrier {

  constructor() {

  }

  async test() {
    return new Promise((resolve) => {
      setTimeout(() => {
        resolve('Hello World')
      }, 4000)
    });
  }
}

export let elastosCarrier = new ElastosCarrier();
