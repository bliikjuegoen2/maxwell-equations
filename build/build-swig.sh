#!/usr/bin/sh

rm *.o
rm *.so

swig -python fields.i
gcc -c fields.c fields_wrap.c -I/usr/include/python3.10 -fPIC
ld -shared fields.o fields_wrap.o -o _fields.so

echo build done