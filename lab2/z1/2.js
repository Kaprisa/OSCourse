function SortAndWrite(fileName)
{
  var fso = new ActiveXObject('Scripting.FileSystemObject');
  if (!fso.FileExists(fileName)) return false;
  var nameArr = fileName.split('.');
  var sortedFileName = nameArr[0] + '_sorted.' + nameArr[1];
  var n = 1;
  while (fso.FileExists(sortedFileName)) {
  	sortedFileName = nameArr[0] + '_sorted' + '_' + n + '.' + nameArr[1];
  	n ++;
  }
  WScript.CreateObject('WScript.Shell').Run('sort ' + fileName + ' /+2' + ' /o ' + sortedFileName)
  return(sortedFileName);
}


var args = WScript.Arguments;

if (args.length) {
	var fileName = SortAndWrite(args(0));
	if (fileName) {
		WScript.Echo('Your file successfully sorted! New sorted file name is ' + fileName);
	} else {
		WScript.Echo('Your file not exists');
	}
} else {
	WScript.Echo('Please, add your file name into command arguments');
}


