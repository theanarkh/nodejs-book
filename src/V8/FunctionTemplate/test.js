const { Parent } = require('./build/Release/test.node');

const parent = new Parent();
console.log('Parent.prototype: ', Parent.prototype);
console.log('parent.protoField: ', parent.protoField);
parent.protoMethod();
console.log('parent.instanceField: ', parent.instanceField);
parent.instanceMethod();