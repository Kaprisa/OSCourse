@echo off
if not exist %1 (echo This directory not found)
else (
	if not exist %2 (
		md %1 & md %2
		echo hello1 > %2\file1.txt
		echo hello2 > %2\file2.txt
		echo hello3 > %2\file3.txt
	)
	copy %2\*.txt %1
	setlocal EnableDelayedExpansion
	for %%d in (%1\*.txt) do (
		set/a n=0
		set f=%%~nxd
		set nm=!f!
		call find_name %2
		move %1\!f! %2\!nm!
	)
	endlocal
)