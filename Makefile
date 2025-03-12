



xflag:
	gcc -o xflag xflag.c -Os -s -static


test:
	gcc -o xflag.test xflag.c -Os -s -static -DTEST
	./xflag.test
	echo Ok





