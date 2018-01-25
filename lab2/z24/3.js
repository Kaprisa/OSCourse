function DeleteFilesWithExtFromDir(dir, nameStart)
{
  var fso = new ActiveXObject("Scripting.FileSystemObject");
  if (!fso.FolderExists(dir)) return false;
  var dirObj = fso.GetFolder(dir);
  var fc = new Enumerator(dirObj.files);
  for (; !fc.atEnd(); fc.moveNext()) {
    var name = fc.item();
    if (fso.GetFileName(name).substr(0, 1) === nameStart) {
      fso.DeleteFile(name);
    }
  }
  return('Files with name ' + nameStart + '* successfully deleted' );
}

var args = WScript.Arguments;

if (args.length) {
	var res = DeleteFilesWithExtFromDir(args(0), args(1));
  if (res) {
    WScript.Echo(res);
  } else {
    WScript.Echo('Your directory not exists');
  }
} else {
	WScript.Echo('Please, add your directory name into command arguments');
}