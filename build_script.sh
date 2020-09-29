#!/bin/bash

WORK_DIR=$(pwd)

./auto/configure --prefix=./installed --add-module=./extended/test_module --with-cc-opt="-w"
make -j24 && make install
./installed/sbin/nginx -c ${WORK_DIR}/installed/conf/nginx.conf