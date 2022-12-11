// node --expose-gc test.js
const { Demo } = require(`./build/Release/addon`);

new Demo();

gc();