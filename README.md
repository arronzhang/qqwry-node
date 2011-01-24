NodeJS driver for libqqwry
==========================================

Get geo for ip 

Data from [QQWry.dat][QQWry]

The c library is base on [libqqwry][libqqwry]

I'm a beginner with c++, if you find some bugs in it, please post a [issue](https://github.com/zzdhidden/qqwry-node/issues).


Install
-----------------------------------------

	npm install qqwry

Usage
-----------------------------------------

Download qqwry.dat from [QQWry.dat][QQWry]

	var qqwry = require("qqwry");
	
	var qqwry = qqwry.open(__dirname + "/qqwry.dat");
	console.log(qqwry.query("115.193.152.250"));
	console.log(qqwry.query("222.73.68.35"));
	console.log(qqwry.query("220.191.113.36"));
	qqwry.close();

[libqqwry]:https://github.com/erning/libqqwry
[QQWry]:http://www.cz88.net/ip

