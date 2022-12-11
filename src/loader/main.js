const path = require('path');
const loader = require('./build/Release/main.node');
const filepath = path.resolve(__dirname, 'demo.js');
loader.compile(filepath)(require, module.exports, module);