set path="C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
mkdir build
cd build
cmake ..
rem Build a second time to fix PCH folder
cmake ..
pause
cd..
call BuildShaders.bat
exit
