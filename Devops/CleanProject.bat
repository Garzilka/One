@echo off

call "%~dp0\Config.bat" %1

set dirsToRemove=Intermediate DerivedDataCache Binaries .vs .idea Script

for %%a in (%dirsToRemove%) do (
    echo %ProjectRoot%\%%a
    rmdir /S /Q %ProjectRoot%\%%a
)

del %ProjectRoot%\%ProjectPureName%.sln

for /D %%a in ("%ProjectRoot%\Plugins\*") do (
	call :SUB %%a
)

PAUSE
EXIT

:SUB outer
echo Path="%1"
if "%1" EQU "%ProjectRoot%\Plugins\Runtime" goto CONTINUE
rmdir /S /Q %1\Intermediate
rmdir /S /Q %1\Binaries
EXIT /B 0

:CONTINUE
echo "This path is ignore"
EXIT /B 0
