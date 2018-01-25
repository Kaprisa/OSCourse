function ReadFile(fileName)
{
  var ForReading = 1;
  var fso = new ActiveXObject('Scripting.FileSystemObject');
  if (!fso.FileExists(fileName)) return false;
  var f = fso.OpenTextFile(fileName, ForReading);
  var s = f.ReadAll();
  f.Close();
  return(s);
}


var args = WScript.Arguments;

if (args.length) {
	var fileContent = ReadFile(args(0));
	if (fileContent) {
		WScript.Echo(fileContent);
	} else {
		WScript.Echo('Your file not exists');
	}
} else {
	WScript.Echo('Please, add your file name into command arguments');
}


