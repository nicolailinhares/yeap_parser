lib_C_SRCS:=$(wildcard lib/*.c)
lib_C_OBJS:=$(patsubst lib/%.c,%.o,$(wildcard lib/*.c))
test_C_SRCS:=$(wildcard tests/*.c)
test_C_OBJS:=$(patsubst tests/%.c,%.o,$(wildcard tests/*.c))

libobjs:
	mkdir -p build
	clang -ggdb3 -O0 -Qunused-arguments -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -c $(lib_C_SRCS)
	mv $(lib_C_OBJS) build/

testsobjs:
	clang -ggdb3 -O0 -Qunused-arguments -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -c $(test_C_SRCS)
	mv $(test_C_OBJS) build/

test: libobjs testsobjs
	clang -ggdb3 -O0 -Qunused-arguments -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -o test.out $(wildcard build/*.o) -lm

clean:
	rm -r build
