#!/bin/sh
# 搭建一个webscoket协议的服务器，打算用libwebsockts这个库
# wget http://git.warmcat.com/cgi-bin/cgit/libwebsockets/snapshot/libwebsockets-1.4-chrome43-firefox-36.tar.gz
# tar xvzf libwebsockets-1.4-chrome43-firefox-36.tar.gz
#cd libwebsockets-1.4-chrome43-firefox-36

# build
git clone https://github.com/warmcat/libwebsockets.git
cd libwebsockets
documentRoot=`pwd`
mkdir build
cmake ..
make

cd build
documentRoot=`pwd`
libdir=libwebsockets
cd $libdir/build/bin
./libwebsockets-test-server --resource_path=$documentRoot/test-server