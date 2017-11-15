set PATH=C:\msys64\mingw64\bin;%PATH%
set CC=gcc

cd /d %~dp0

%CC% -c windows/*.cpp
%CC% -shared -o myCLib.dll *.o
del *.o

%CC%  -I./ -L./ -o mainForUnitTest.exe windows/mainForUnitTest.cpp -l myCLib
mainForUnitTest.exe
pause

