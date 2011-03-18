var qqwry = require("./build/default/qqwry.node").qqwry;

var re = /(安徽|福建|甘肃|广东|广西|贵州|海南|河北|河南|黑龙江|湖北|湖南|吉林|江苏|江西|辽宁|内蒙古|宁夏|青海|山东|山西|陕西|四川|西藏|新疆|云南|浙江)?省?([^市\s]*市?)([^\s]*)[\s|]*([^\s]*)/;
/* Query province and city */
qqwry.prototype.q = function(ip){
	var str = this.query(ip);
	str = str.match(re);
	if( str ) {
		return {
			province: str[1],
			city: str[2],
			district: str[3],
			vender: str[4]
		};
	}
	return null;
};

exports.open =  function(file) {
	return new qqwry(file);
};
