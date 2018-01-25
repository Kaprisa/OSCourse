function MoveFile(dir1, dir2, fileName)
{
  var fso = new ActiveXObject('Scripting.FileSystemObject');
  if (!fso.FolderExists(dir1)) return {
  		error: 'Source Directory not exists'
  };
  if (!fso.FolderExists(dir2)) return {
      error: 'Destination Directory not exists'
  };
  if (!fso.FileExists(dir1 + '/' +fileName)) return {
  		error: 'File not exists'
  };
  if (fso.FileExists(dir2 + '/' +fileName)) return {
      error: 'File with this name in the destination directory already exists'
  };
  fso.MoveFile(dir1 + '/' + fileName, dir2 + '/');
  return('Your file successfully moved!');
}


var args = WScript.Arguments;

if (args.length > 2) {
	var res = MoveFile(args(0), args(1), args(2));
	if (!res.error) {
		WScript.Echo(res);
	} else {
		WScript.Echo(res.error);
	}
} else {
	WScript.Echo('Please, add command line arguments: dir1 dir2 file');
}


