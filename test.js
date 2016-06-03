var ISO8583 = require('./index.js');
var assert = require('assert');


var message = new ISO8583.Message(0);
console.log("message.pack()");
console.log(message.pack().length);
console.log("message.pack() end");



console.log("message.test()");
var messageFields = [
	[0, "1234"],
	[2, "1234567890123456"],
	[4, "5699"],
	[11, "234"],
	[39, "4"],
	[41, "12345"],
	[42, "678901234"],
	[125, "BLAH BLAH"]
];
console.log(message.test(messageFields));
console.log("message.test() end");


