const { MyObject } = require(`./build/Release/addon`);

new MyObject();
// global.object = new MyObject();
// new MyObject().ref();

gc();