#!/bin/bash

./auto/configure --prefix=./installed --add-module=./extended/test_module --with-cc-opt="-w"
make -j24 && make install
./installed/sbin/nginx -c /home/reading/reading/installed/conf/nginx.conf