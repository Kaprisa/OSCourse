@echo off

for %%d in (%*) do (
  for %%i in (%%d\*.txt) do echo %%~ni 
)

