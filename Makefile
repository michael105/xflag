



xflag: xflag.c
	gcc -o xflag xflag.c -Os -s -static


xflag.test: xflag.c
	gcc -o xflag.test xflag.c -Os -s -static -DTEST


test: xflag.test
	./xflag.test  2>&1 | tee test.result
	diff -ac --color=always test.expect test.result && echo -e "-------\ntest ok" || ( echo test failed && false )


inittest:
	./xflag.test  2>&1 | tee test.expect



up:
	rev=revision.sh
	echo $(rev)
