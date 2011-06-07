/*
* Test for the memery leak. exec this and see the memery console.
*/
var qqwry = require("qqwry");
var assert = require("assert");

var qqwry = qqwry.open(__dirname + "/../zhejie/zhejie.com/geodata/qqwry.dat");


function run () {
	var q = qqwry.query("115.193.152.250");
	//console.log(q);
}

function runGroup () {
	for (var i = 0; i < 10000; i++) {
		run();
	};
}

setInterval( runGroup, 2000 );
