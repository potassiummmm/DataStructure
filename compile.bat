@echo off
for /d %%i in (*) do (
cd %%i
echo Compiling binaries for problem %%i...
g++ -std=c++14 ./*.cpp -Wall -o %%i.exe
cd ../
)
echo Compilation complete.