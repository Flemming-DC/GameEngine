@echo off
echo. 
echo "This script only works in 'Developer command prompt for VS 2019', which you can find in the start menu. The normal command prompt cannot run this script."
echo. 

echo copy resources
xcopy .\Game\res .\Binaries\windows-x86\Debug\Game-InEditor\res /E /Y /I > nul
xcopy .\Game\res .\Binaries\windows-x86\Debug\Game\res /E /Y /I > nul
xcopy .\Game\res .\Binaries\windows-x86\Release\Game-InEditor\res /E /Y /I > nul
xcopy .\Game\res .\Binaries\windows-x86\Release\Game\res /E /Y /I > nul

echo copy imgui.ini
copy .\Game\imgui.ini .\Binaries\windows-x86\Debug\Game-InEditor\imgui.ini > nul
copy .\Game\imgui.ini .\Binaries\windows-x86\Debug\Game\imgui.ini > nul
copy .\Game\imgui.ini .\Binaries\windows-x86\Release\Game-InEditor\imgui.ini > nul
copy .\Game\imgui.ini .\Binaries\windows-x86\Release\Game\imgui.ini > nul

echo compile
msbuild GameEngine.sln /property:Configuration=Debug > nul
msbuild GameEngine.sln /property:Configuration=Release > nul

echo done