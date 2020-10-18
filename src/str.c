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

#include <flibc/str.h>

size_t strlen(const char *str)
{
	const char *start = str;
	while (*str != '\0')
		str++;
	return str - start;
}

int strcmp(const char *a, const char *b)
{
	for (; *a != '\0'; ++a, ++b) {
		if (*b == '\0')
			return 1;

		if (*a < *b) {
			return -1;
		} else if (*a > *b) {
			return 1;
		}
	}

	if (*b != '\0')
		return -1;

	return 0;
}
