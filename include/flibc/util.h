/*
 * Copyright (C) 2020 Greg Depoire--Ferrer <greg.depoire@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef FLIBC_UTIL_H
#define FLIBC_UTIL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <flibc/linux.h>
#include <flibc/str.h>

#define F_UNUSED(x) (void)(x)

#define F__STR_VALUE(x) #x
#define F_STR_VALUE(x) F__STR_VALUE(x)
#define F_STR_LINE F_STR_VALUE(__LINE__)

#define F_PRINT(fd, str) _util_print(fd, str, strlen(str))

#define F_ASSERT(check)                                                        \
	if (__builtin_expect(!(check), false)) {                               \
		F_UNUSED(F_PRINT(2, "Assertion failed at ") &&                 \
			 F_PRINT(2, __FILE__) &&                               \
			 F_PRINT(2, ":" F_STR_LINE ".\n"));                    \
		sys_exit(1);                                                   \
	}

#define F_ASSERT_UNREACHABLE()                                                 \
	do {                                                                   \
		F_UNUSED(                                                      \
		    F_PRINT(2,                                                 \
			    "Code that was not supposed to be reachable was "  \
			    "executed at ") &&                                 \
		    F_PRINT(2, __FILE__) && F_PRINT(2, ":" F_STR_LINE ".\n")); \
		sys_exit(1);                                                   \
	} while (false)

/* This function is used internally by the F_PRINT macro. */
bool _util_print(int fd, const char *str, size_t len);

void util_reverse(char *start, char *end);

void util_write_num(uint64_t num, char *str, size_t capacity);
bool util_parse_num(const char *str, size_t len, uint64_t *out);
bool util_read_num_from_file(const char *path, uint64_t *out);

/* Keep the naming from libc for the functions that already exist in libc to
   make porting and understanding the code easier. */
uint16_t htons(uint16_t val);

#endif
