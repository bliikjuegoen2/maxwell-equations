#!/usr/bin/sh
rm *.so

swig -python fields.i
./setup.py build
cp build/lib.linux-x86_64-cpython-310/_fields.cpython-310-x86_64-linux-gnu.so ./_fields.so

echo build done