#!/bin/sh

# luajit
wget http://luajit.org/download/LuaJIT-2.0.1.tar.gz
tar -zxf LuaJIT-2.0.1.tar.gz
cd LuaJIT-2.0.1
make

cd ..

# v8
wget -O v8-master.tar.gz https://github.com/v8/v8/archive/master.tar.gz
tar -zxf v8-master.tar.gz
cd v8-master
make dependencies
make native
