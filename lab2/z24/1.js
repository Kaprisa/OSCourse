function DeleteFilesWithExtFromDir(dir, extStart)
{
  var fso = new ActiveXObject("Scripting.FileSystemObject");
  if (!fso.FolderExists(dir)) return false;
  var dirObj = fso.GetFolder(dir);
  var fc = new Enumerator(dirObj.files);
  for (; !fc.atEnd(); fc.moveNext()) {
    var name = fc.item();
    if (fso.GetExtensionName(name).substr(0, 1) === extStart) {
      fso.DeleteFile(name);
    }
  }
  return('Files with extention ' + extStart + '* successfully deleted' );
}

var args = WScript.Arguments;

if (args.length > 1) {
	var res = DeleteFilesWithExtFromDir(args(0), args(1));
  if (res) {
    WScript.Echo(res);
  } else {
    WScript.Echo('Your directory not exists');
  }
} else {
	WScript.Echo('Please, add your directory name into command arguments');
}