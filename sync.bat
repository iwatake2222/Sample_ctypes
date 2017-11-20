rem @echo off

set PATH=C:\msys64\usr\bin\;%PATH%

cd /d %~dp0

rsync -auv %~n1  pi@192.168.1.88:/home/pi/mirror/

rem pause

