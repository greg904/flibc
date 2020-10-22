# Make sure that the shell is the same everywhere.
SHELL = /bin/sh

src_c := $(wildcard src/*.c) $(wildcard libs/*.c)
src_asm := $(wildcard src/*.S)
objs := $(src_c:%.c=%.o) $(src_asm:%.S=%.o)
inc_deps := $(src_c:%.c=%.d)

CFLAGS = -std=gnu11 -ffreestanding -nostdlib -flto -fPIC -O2 -Wall -Wextra -Werror

.PHONY: all
all: libflibc.a

.PHONY: clean
clean:
	rm -f $(objs) $(inc_deps) libflibc.a

.PHONY: format
format:
	clang-format -i $(src_c) include/flibc/*.h

%.o: %.c
	$(CC) $< -c -MD -o $@ -Iinclude -Ilibs $(CPPFLAGS) $(CFLAGS)

%.o: %.S
	$(CC) $< -c -MD -o $@ $(ASFLAGS)

# The compiler will generate dependencies for each implementation file.
-include $(inc_deps)

libflibc.a: $(objs)
	$(AR) rcs $@ $^ 

