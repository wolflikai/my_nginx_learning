
default:	build

clean:
	rm -rf Makefile objs

build:
	$(MAKE) -f objs/Makefile

install:
	$(MAKE) -f objs/Makefile install

modules:
	$(MAKE) -f objs/Makefile modules

upgrade:
	./installed/sbin/nginx -t

	kill -USR2 `cat ./installed/logs/nginx.pid`
	sleep 1
	test -f ./installed/logs/nginx.pid.oldbin

	kill -QUIT `cat ./installed/logs/nginx.pid.oldbin`
