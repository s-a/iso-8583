var ISO8583 = require('../');
var assert = require('assert');

function TestMessage() {
	return this;
}

TestMessage.prototype.packed = function() {
	return [
		"12", "34", "D0", "20", "00", "00", "02", "C0", "00", "00", "00", "00", "00", "00", "00", "00", "00", "08", "16", "12", "34", "56", "78", "90", "12", "34", "56", "00", "00", "00", "00", "56", "99", "00", "02", "34", "00", "04", "31", "32", "33", "34", "35", "20", "20", "20", "36", "37", "38", "39", "30", "31", "32", "33", "34", "20", "20", "20", "20", "20", "20", "00", "09", "42", "4C", "41", "48", "20", "42", "4C", "41", "48"
	];
};

TestMessage.prototype.unpacked = function() {
	return [
		[0, "1234"],
		[2, "1234567890123456"],
		[4, "000000005699"],
		[11, "000234"],
		[39, "004"],
		[41, "12345"],
		[42, "678901234"],
		[125, "BLAH BLAH"]
	];
};

var testMessage = new TestMessage();

describe('native extension', function() {
	it('should export a wrapped object', function() {
		var message = new ISO8583.Message();

		assert.equal(typeof(message.packSync), "function");
		assert.equal(typeof(message.unpackSync), "function");
	});
});

describe('pack iso8583 message', function() {
	it('should pack message values and return an Array of HEX values', function() {

		var msg = testMessage.unpacked();
		var message = new ISO8583.Message();
		var packedMessage = message.packSync(msg);

		assert.equal(packedMessage.join(" "), testMessage.packed().join(" "));
	});
});

describe('parse and unpack iso8583 message', function() {
	it('should parse and unpack a message and return an Array of values', function() {
		var msg = testMessage.packed().join("");
		var message = new ISO8583.Message();
		var unpackedMessage = message.parseSync(msg);

		assert.deepEqual(unpackedMessage, testMessage.unpacked());
	});
});

/*
it('should export function that returns nothing', function() {
	assert.equal(ISO8583.nothing(), undefined);
});

it('should export a function that returns a string', function() {
	assert.equal(typeof ISO8583.aString(), 'string');
});

it('should export a function that returns a boolean', function() {
	assert.equal(typeof ISO8583.aBoolean(), 'boolean');
});

it('should export function that returns a number', function() {
	assert.equal(typeof ISO8583.aNumber(), 'number');
});

it('should export function that returns an object', function() {
	assert.equal(typeof ISO8583.anObject(), 'object');
});

it('should export function that returns an object with a key, value pair', function() {
	assert.deepEqual(ISO8583.anObject(), {'key': 'value'});
});

it('should export function that returns an array', function() {
	assert.equal(Array.isArray(ISO8583.anArray()), true);
});

it('should export function that returns an array with some values', function() {
	assert.deepEqual(ISO8583.anArray(), [1, 2, 3]);
});

it('should export function that calls a callback', function(done) {
	ISO8583.callback(done);
});
*/
