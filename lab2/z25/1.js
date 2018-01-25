function GetLinesWithStr(file1, file2, str)
{
  var fso = new ActiveXObject('Scripting.FileSystemObject');
  if (!fso.FileExists(file1)) return {
    error: 'Result file not found'
  };
  if (!fso.FileExists(file2)) return {
    error: 'Source file not found'
  };
  var f2 = fso.OpenTextFile(file2, 1);
  var s = f2.ReadAll();
  f2.Close();
  var lines = s.split('\n');
  var n = '';
  for (line in lines) {
    if (lines[line].indexOf(str) > 0) {
      n += lines[line] + '\n';
    }
  }
  var f = fso.OpenTextFile(file1, 2, true);
  f.Write(n);
  f.Close();
  return('Your lines successfully copied!');
}


var args = WScript.Arguments;

if (args.length > 3) {
  var res = GetLinesWithStr(args(0) + '/' + args(1), args(2), args(3));
  if (!res.error) {
    WScript.Echo(res);
  } else {
    WScript.Echo(res.error);
  }
} else {
  WScript.Echo('Please, add command line arguments: dir file1 file2 text');
}