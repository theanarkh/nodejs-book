const { FuncWithCallback } = require('./build/Release/test.node');

const instace = new FuncWithCallback();
console.log(instace.method());