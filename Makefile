object_files := libs/parse_vdso.o \
	src/linux.o \
	src/mem.o \
	src/str.o \
	src/util.o
deps_files := $(object_files:%.o=%.d)

CCFLAGS ?= -fno-stack-protector -flto -fPIC -O2 -Wall -Wextra -Werror
CCFLAGS += -Iinclude -Ilibs -std=gnu11 -ffreestanding -nostdlib

.PHONY: all
all: libflibc.a

.PHONY: clean
clean:
	rm -f ${object_files} ${deps_files} libflibc.a

.PHONY: format
format:
	clang-format -i src/*.c include/flibc/*.h

%.o: %.c
	${CC} $< -c -MD -o $@ ${CCFLAGS}

# The compiler will generate dependencies for each implementation file.
-include ${deps_files}

libflibc.a: ${object_files}
	${AR} rcs $@ $^ 
