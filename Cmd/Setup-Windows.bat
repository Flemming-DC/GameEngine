@echo off

pushd ..
Binaries\Premake\Windows\premake5.exe --file=Build.lua vs2022 --verbose
popd
pause