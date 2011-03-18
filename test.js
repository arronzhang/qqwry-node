var qqwry = require("./index");
var assert = require("assert");

var qqwry = qqwry.open(__dirname + "/../zhejie/zhejie.com/geodata/qqwry.dat");

var data = [
	["115.193.152.250", "浙江", "杭州市"],
	["60.13.250.0", "新疆", "克拉玛依市"],
	["117.25.128.84", "福建", "厦门市"],
	["222.73.68.35", "上海", "上海市"],
	["203.198.23.69", "香港", "香港"],
	["221.223.102.183", "北京市", "北京市"],
	["211.66.210.249", "广东", ""],
];

data.forEach(function(area) {
	var q = qqwry.q(area[0]);
	console.log(q);
	q && assert.equal(q.city, area[2]);
});

qqwry.close();


