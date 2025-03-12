



xflag:
	gcc -o xflag xflag.c -Os -s -static


test:
	gcc -o xflag.test xflag.c -Os -s -static -DTEST
	./xflag.test  2>&1 | tee test.result
	diff -ac --color=always test.expect test.result && echo test ok || ( echo test failed && false )


inittest:
	./xflag.test  2>&1 | tee test.expect



