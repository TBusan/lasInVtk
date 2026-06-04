@echo off
call "D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

set "WindowsSdkDir=D:\Windows Kits\10"
set "WindowsSdkVersion=10.0.22621.0"
set "PATH=%WindowsSdkDir%\bin\%WindowsSdkVersion%\x64;%PATH%"
set "LIB=%LIB%;%WindowsSdkDir%\Lib\%WindowsSdkVersion%\um\x64;%WindowsSdkDir%\Lib\%WindowsSdkVersion%\ucrt\x64"
set "INCLUDE=%INCLUDE%;%WindowsSdkDir%\Include\%WindowsSdkVersion%\um;%WindowsSdkDir%\Include\%WindowsSdkVersion%\ucrt"

if exist build rmdir /s /q build
cmake -B build -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build
pause
