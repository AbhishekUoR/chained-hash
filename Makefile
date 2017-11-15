all: test_hash

test_hash: hash.o test_hash.c
	gcc --std=c99 -g test_hash.c hash.o -o test_hash

hash.o: hash.c hash.h
	gcc --std=c99 -g -c hash.c

clean:
	rm -f hash.o test_hash
	rm -rf ./*.dSYM
