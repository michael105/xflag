



xflag: xflag.c
	gcc -o xflag xflag.c -Os -s -static


xflag.test: xflag.c
	gcc -o xflag.test xflag.c -Os -s -static -DTEST


test: xflag.test
	./xflag.test  2>&1 | tee test.result
	diff -ac --color=always test.expect test.result && echo -e "-------\ntest ok" || ( echo test failed && false )


inittest:
	./xflag.test  2>&1 | tee test.expect



up: test
	sed -Ei "s/(^#define VERSION .*)devel.*/\1devel-`revision.sh`\"/" xflag.c
	git commit -m revision xflag.c
	sed -Ei "s/^(#define GITCOMMIT ).*/\1`git log | sed -e '1s/commit //;q'`/" xflag.c
	git commit -m commithash xflag.c


