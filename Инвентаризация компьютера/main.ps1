<#
.DESCRIPTION
Инвертаризация компьютера
#>

$fragc = Get-WmiObject  win32_processor | Select-Object -Property Caption, DeviceID, Manufacturer, MaxClockSpeed, Name, SocketDesignation |
ConvertTo-Html -Fragment -PreContent '<div class="h"><h2>Процессор</h2></div>' | Out-String

$frag0 = Get-WmiObject Win32_ComputerSystem | Select-Object -Property Domain, Manufacturer, Model, Name, TotalPhysicalMemory | 
ConvertTo-Html -Fragment -PreContent '<div class="h"><h2>О компьютере</h2></div>' | Out-String

$frag1 = Get-WmiObject Win32_OperatingSystem | Select-Object -Property Caption,BuildType,BuildNumber,BootDevice,Manufacturer,Name,Version,SystemDrive,SystemDirectory,SystemDevice,SerialNumber,OSType,OSLanguage,OSArchitecture,NumberOfProcesses,NumberOfUsers |
ConvertTo-Html -Fragment -As LIST -PreContent '<div class="h"><h2>О системе</h2></div>' | Out-String

$frag2 = Get-WmiObject Win32_LogicalDisk -Filter 'DriveType=3' |
Select-Object @{name='Drive';expression={$_.DeviceID}},
@{name='Size(GB)';expression={$_.Size / 1GB -as [int]}},
@{name='FreeSpace(GB)';expression={$_.freespace / 1GB -as [int]}} |
ConvertTo-Html -Fragment -PreContent '<div class="h"><h2>Диски</h2></div>' | Out-String

$frag3 = Get-WmiObject Win32_UserAccount |
Select-Object @{name='AccountType';expression={$_.AccountType}},
@{name='Caption';expression={$_.Caption}},
@{name='Domain';expression={$_.Domain}},
@{name='SID';expression={$_.SID}},
@{name='FullName';expression={$_.FullName}},
@{name='Name';expression={$_.Name}} |
ConvertTo-Html -Fragment -PreContent '<div class="h"><h2>Группы пользователей</h2></div>' | Out-String

$frag4 = Get-WmiObject Win32_PhysicalMemory |
select @{name='DeviceLocator';expression={$_.DeviceLocator}},
@{name='BankLabel';expression={$_.BankLabel}},
@{name='Capacity';expression={$_.Capacity}},
@{name='Manufacturer';expression={$_.Manufacturer}},
@{name='PartNumber';expression={$_.PartNumber}},
@{name='SerialNumber';expression={$_.SerialNumber}},
@{name='TotalWidth';expression={$_.TotalWidth}},
@{name='Speed';expression={$_.Speed}} |
ConvertTo-Html -Fragment -PreContent '<div class="h"><h2>Оперативная память</h2></div>' | Out-String

$frag5 = Get-WmiObject Win32_Service -Filter "StartMode='Auto'" |
Select-Object @{name='Name';expression={$_.Name}},
@{name='ProcessId';expression={$_.ProcessId}},
@{name='State';expression={$_.State}} |
ConvertTo-Html -Fragment -PreContent '<div class="h"><h2>Программы автозапуска</h2></div>' | Out-String

$frag6 = Get-WmiObject Win32_Service -Filter "State='Running'" |
Select-Object @{name='Name';expression={$_.Name}},
@{name='ProcessId';expression={$_.ProcessId}},
@{name='StartMode';expression={$_.StartMode}} |
ConvertTo-Html -Fragment -PreContent '<div class="h"><h2>Работающие сервисы системы</h2></div>' | Out-String
#Format-List -Property * |

$frag7 = Get-EventLog -LogName System -EntryType Error -After (Get-date).AddMonths(-2) |  
Select-Object @{name='EventID';expression={$_.EventID}},
@{name='Index';expression={$_.Index}},
@{name='Category';expression={$_.CategoryNumber}},
@{name='Message';expression={$_.Message}},
@{name='Source';expression={$_.Source}},
@{name='InstanceId';expression={$_.InstanceId}},
@{name='TimeGenerated';expression={$_.TimeGenerated}},
@{name='TimeWritten';expression={$_.TimeWritten}}, 
@{name='UserName';expression={$_.UserName}}|
ConvertTo-Html -Fragment -PreContent '<div class="h"><h2>Ошибки системы за последние 2 месяца</h2></div>' | Out-String



$head = @'
<style>
body { background-color:#E4E5D2;
           font-family: "Lucida Sans Unicode", "Lucida Grande", Sans-Serif;
           font-size:14px; }
.h {
  background: #E4E5D2;
  padding: 50px 20px;
  margin: 50px auto;
  text-align: center;
} 
.h h2 {
  font-family: 'Open Sans', sans-serif;
  position: relative;
  color: #5C2610;
  font-size: 3em;
  font-weight: normal;
  line-height: 1;
  padding: 10px 0;
  margin: 0;
  display: inline-block;
}
.h h2:before {
  content: ""; 
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%,-50%);
  width: 80%;
  height: 200%;
  border-radius: 50%/30%;
  border: 6px solid #5C2610;
  border-left-color: transparent;
  border-right-color: transparent;
}
table {
font-size: 14px;
border-radius: 10px;
border-spacing: 0;
text-align: center;
margin: 0 auto;
}
th {
background: #BCEBDD;
color: white;
text-shadow: 0 1px 1px #2D2020;
padding: 10px 20px;
}
th, td {
border-style: solid;
border-width: 0 1px 1px 0;
border-color: white;
}
th:first-child, td:first-child {
text-align: left;
}
th:first-child {
border-top-left-radius: 10px;
}
th:last-child {
border-top-right-radius: 10px;
border-right: none;
}
td {
padding: 10px 20px;
background: #F8E391;
}
tr:last-child td:first-child {
border-radius: 0 0 0 10px;
}
tr:last-child td:last-child {
border-radius: 0 0 10px 0;
}
tr td:last-child {
border-right: none;
}
</style>
'@
ConvertTo-HTML -head $head -PostContent $frag0,$fragc,$frag1,$frag2,$frag3,$frag4,$frag5,$frag6,$frag7 -PreContent "<div>Текущий пользователь: $env:UserName</div>" > main.html



