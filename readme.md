### nginx learning

> `./extended/test_module` has a tiny third module for test code;


### how to build

```
./auto/configure --prefix=./installed --add-module=./extended/test_module   // just first to configure
make -j24 && make install  // make when you change the code
```

### how to config

```
// nginx.conf

localhost /test {
    mytest;
}

// you can open the `http://ip/test` on browser to test your code
```