var args = WScript.Arguments;

var str = '';

for (var i = 0; i < args.length; i ++) {
	str += args(i) + '\n';
}

WScript.Echo(str);



