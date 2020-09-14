cd 3rdparty\bgfx
..\bx\tools\bin\windows\genie --platform=x64 --with-tools vs2019
pause
cd ..\assimp
BUILDBINARIES_EXAMPLE.bat
pause
cd ..\yaml
if not exist "build" mkdir "build"
cd build
cmake -G "Visual Studio 16 2019" ../
pause
