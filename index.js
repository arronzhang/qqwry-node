var qqwry = require("./build/default/qqwry.node").qqwry;
exports.open =  function(file) {
	return new qqwry(file);
};
