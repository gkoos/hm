CFLAGS = -Wall -Werror -Wextra -pedantic -g -O3 -std=gnu99

build:
	@mkdir -p build
	${CC} ${CFLAGS} -fPIC -shared -o build/libhashmap.so src/hashmap.c src/hashfuncs.c -Iincludes

build-test:
	@mkdir -p build
	${CC} ${CFLAGS} -o build/tests tests/tests.c -Lbuild -lhashmap -Iincludes -Itools/cnt -Wl,-rpath=build

.PHONY: build build-test
