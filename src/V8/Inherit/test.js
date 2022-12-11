const { Parent, Child } = require('./build/Release/test.node');
const child = new Child();

console.log('Child.prototype: ', Child.prototype);
console.log('Parent.prototype === Child.prototype: ', Parent.prototype === Child.prototype);
console.log('child.protoField: ', child.protoField);
child.protoMethod();
console.log('child.instanceField: ', child.instanceField);
console.log('child.instanceMethod: ', child.instanceMethod);

console.log('child.childProtoField: ', child.childProtoField);
console.log('child.childInstanceField: ', child.childInstanceField);
console.log('child instanceof Parent: ', child instanceof Parent);