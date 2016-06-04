var NativeExtension = require('bindings')('ISO8583');

NativeExtension.Message.prototype.parse = function(data) {
	var result = [];
	var msg = new Buffer(data, "hex");
	var len = msg.length;
  	var unpackedMessage = this.unpack(msg, len);

	for (var i = 0; i < unpackedMessage.length; i++) {
		var u = unpackedMessage[i];
		if (u){
			result.push([parseInt(u.key, 10), u.value.trim()]);
		}
	}

	return result;
};

module.exports = NativeExtension;
