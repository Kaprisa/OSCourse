function ReadFile(dir, fileName)
{
  var ForReading = 1;
  var fso = new ActiveXObject('Scripting.FileSystemObject');
  if (!fso.FolderExists(dir)) return {
  		error: "Directory not exists"
  };
  if (!fso.FileExists(dir + '/' +fileName)) return {
  		error: "File not exists"
  };
  var f = fso.OpenTextFile(dir + '/' + fileName, ForReading);
  var s = f.ReadAll();
  f.Close();
  if (!s.length) return {
  	error: "Your file are empty"
  }
  return(s);
}


var args = WScript.Arguments;

if (args.length) {
	var fileContent = ReadFile(args(0), args(1));
	if (!fileContent.error) {
		WScript.Echo(fileContent);
	} else {
		WScript.Echo(fileContent.error);
	}
} else {
	WScript.Echo('Please, add your file name into command arguments');
}


