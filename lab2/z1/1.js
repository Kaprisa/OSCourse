function GetLinesWithStr(fileName, str)
{
  var ForReading = 1;
  var fso = new ActiveXObject('Scripting.FileSystemObject');
  if (!fso.FileExists(fileName)) return false;
  var f = fso.OpenTextFile(fileName, ForReading);
  var s = f.ReadAll();
  f.Close();
  var lines = s.split('\n');
  var n = '';
  for (line in lines) {
    var index = lines[line].indexOf('AAA');
  	if ( index > 0 ) {
      n += "Line: " + line + ", position: " + index + ", string: " + lines[line] + '\n'
  	}
  }
  return(n);
}


var args = WScript.Arguments;

if (args.length) {
	var res = GetLinesWithStr(args(0), 'ААА');
  if (res) {
    WScript.Echo(res);
  } else {
    WScript.Echo('Your file not exists');
  }
} else {
	WScript.Echo('Please, add your file name into command arguments');
}


