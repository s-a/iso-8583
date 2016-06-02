var ISO8583 = require('./index.js');
var assert = require('assert');


var message = new ISO8583.Message(0);
console.log("message.pack()");
console.log(message.pack().length);
console.log("message.pack() end");
