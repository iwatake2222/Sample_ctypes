#!/bin/sh

CC=gcc

$CC -c linux/*.cpp
$CC -shared -fPIC -o libMyCLib.so *.o
rm *.o

$CC -I./ -L./ -o mainForUnitTest linux/mainForUnitTest.cpp -lMyCLib

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./
./mainForUnitTest


