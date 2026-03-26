@echo off

rem Find UE and Launcher
set EnginePath=
FOR /F "tokens=2* skip=2" %%a in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\EpicGames\Unreal Engine\5.7" /v "InstalledDirectory"') do set EnginePath=%%b

if not exist "%EnginePath%" (
	if not exist "%1" (

	@echo "UE5.7 Is not installed. Unable to proceed. Try [Command] [ARG1: Enter UE path. For example: E:\UE_5.7]"
	pause
	exit /b 1
	)
	set EnginePath=%1
)
echo EnginePath=%EnginePath%

rem Project
set ProjectRoot=%~dp0\..
echo ProjectRoot=%ProjectRoot%
set ProjectPureName=One
echo ProjectPureName=%ProjectPureName%

rem Unreal path
set RunUATPath=%EnginePath%\Engine\Build\BatchFiles\RunUAT.bat
echo RunUATPath=%RunUATPath%
set UBTRelativePath=%EnginePath%\Engine\Binaries\DotNET\UnrealBuildTool.exe
echo UBTRelativePath=%UBTRelativePath%
set EditorUE=%EnginePath%\Engine\Binaries\Win64\UE4Editor.exe
echo EditorUE=%EditorUE%
set DebugUE=%EnginePath%\Engine\Binaries\Win64\UE4Editor-Win64-DebugGame.exe
echo DebugUE=%DebugUE%

rem Path to Project
set ProjectName=%ProjectPureName%.uproject
echo ProjectName=%ProjectName%
set ProjectPath=%ProjectRoot%\%ProjectName%
echo ProjectPath=%ProjectPath%

