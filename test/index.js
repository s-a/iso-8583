var ISO8583 = require('../');
var assert = require('assert');


describe('native extension', function() {
  it('should export a wrapped object', function() {
    var message = new ISO8583.Message(0);

console.log(message.unpack());

    assert.equal(message.pack(), 1);
    assert.equal(message.unpack(), 2);
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
});
