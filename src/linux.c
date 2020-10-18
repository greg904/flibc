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

#include <stdnoreturn.h>

#include <flibc/linux.h>
#include <flibc/util.h>

#include <parse_vdso.h>

typedef int (*sys_clock_gettime_t)(clockid_t, struct timespec *);
static sys_clock_gettime_t sys_clock_gettime_vdso;

extern int main(int argc, char **argv);

#ifdef __x86_64__
__attribute__((used))
noreturn void sys_main_amd64(void *stack)
{
	char *stack_c = (char *)stack;
	char **argv = (char **)(stack_c + 8);

	/* Find the start of the auxiliary vector. */
	uint64_t argc = *(uint64_t *)stack_c;
	char **envp = argv + argc + 1;
	char **auxv = envp;
	while (*auxv != NULL)
		++auxv;
	++auxv;

	/* Load VSDO functions. */
	vdso_init_from_auxv((void *)auxv);
	sys_clock_gettime_vdso =
	    (sys_clock_gettime_t)vdso_sym("LINUX_2.6", "__vdso_clock_gettime");

	sys_exit(main(argc, argv));
}

static uint64_t sys_1(uint64_t num, uint64_t a);
static uint64_t sys_2(uint64_t num, uint64_t a, uint64_t b);
static uint64_t sys_3(uint64_t num, uint64_t a, uint64_t b, uint64_t c);
static uint64_t sys_4(uint64_t num, uint64_t a, uint64_t b, uint64_t c,
		      uint64_t d);
static uint64_t sys_5(uint64_t num, uint64_t a, uint64_t b, uint64_t c,
		      uint64_t d, uint64_t e);
#else
#	error Only the amd64 architecture is supported for now!
#endif

ssize_t sys_read(int fd, void *buf, size_t count)
{
	return (ssize_t)sys_3(0, fd, (uint64_t)buf, count);
}

ssize_t sys_write(int fd, const void *buf, size_t count)
{
	return (ssize_t)sys_3(1, fd, (uint64_t)buf, count);
}

int sys_open(const char *path, int flags, umode_t mode)
{
	return (int)sys_3(2, (uint64_t)path, flags, mode);
}

int sys_close(int fd) { return (int)sys_1(3, fd); }

noreturn void sys_exit(int code)
{
	sys_1(60, code);
	for (;;) {
	}
}

pid_t sys_clone(unsigned long flags, void *stack, int *parent_tid,
		int *child_tid, unsigned long tls)
{
	return (long)sys_5(56, flags, (uint64_t)stack, (uint64_t)parent_tid,
			   (uint64_t)child_tid, tls);
}

int sys_kill(pid_t pid, int signal) { return (int)sys_2(62, pid, signal); }

int sys_socket(int family, int type, int protocol)
{
	return (int)sys_3(41, family, type, protocol);
}

int sys_bind(int fd, struct sockaddr *addr, size_t addr_len)
{
	return (int)sys_3(49, fd, (uint64_t)addr, addr_len);
}

int sys_listen(int fd, int backlog) { return (int)sys_2(50, fd, backlog); }

int sys_accept4(int fd, struct sockaddr *peer_addr, int *peer_addr_len,
		int flags)
{
	return (int)sys_4(288, fd, (uint64_t)peer_addr, (uint64_t)peer_addr_len,
			  flags);
}

int sys_epoll_create1(int flags) { return (int)sys_1(291, flags); }

int sys_epoll_ctl(int epoll_fd, int op, int fd, struct epoll_event *event)
{
	return (int)sys_4(233, epoll_fd, op, fd, (uint64_t)event);
}

int sys_epoll_wait(int epoll_fd, struct epoll_event *events, int max_events,
		   int timeout)
{
	return (int)sys_4(232, epoll_fd, (uint64_t)events, max_events, timeout);
}

int sys_clock_gettime(clockid_t clock, struct timespec *time)
{
	if (sys_clock_gettime_vdso != NULL)
		return sys_clock_gettime_vdso(clock, time);

	return (int)sys_2(228, clock, (uint64_t)time);
}

int sys_nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
	return (int)sys_2(35, (uint64_t)rqtp, (uint64_t)rmtp);
}

#ifdef __x86_64__
static uint64_t sys_1(uint64_t num, uint64_t a)
{
	uint64_t ret;
	asm volatile("syscall"
		     : "=a"(ret)
		     : "0"(num), "D"(a)
		     : "rcx", "r11", "memory");
	return ret;
}

static uint64_t sys_2(uint64_t num, uint64_t a, uint64_t b)
{
	uint64_t ret;
	asm volatile("syscall"
		     : "=a"(ret)
		     : "0"(num), "D"(a), "S"(b)
		     : "rcx", "r11", "memory");
	return ret;
}

static uint64_t sys_3(uint64_t num, uint64_t a, uint64_t b, uint64_t c)
{

	uint64_t ret;
	asm volatile("syscall"
		     : "=a"(ret)
		     : "0"(num), "D"(a), "S"(b), "d"(c)
		     : "rcx", "r11", "memory");
	return ret;
}

static uint64_t sys_4(uint64_t num, uint64_t a, uint64_t b, uint64_t c,
		      uint64_t d)
{
	register uint64_t r10 asm("r10") = d;

	uint64_t ret;
	asm volatile("syscall"
		     : "=a"(ret)
		     : "0"(num), "D"(a), "S"(b), "d"(c), "r"(r10)
		     : "rcx", "r11", "memory");
	return ret;
}

static uint64_t sys_5(uint64_t num, uint64_t a, uint64_t b, uint64_t c,
		      uint64_t d, uint64_t e)
{
	register uint64_t r8 asm("r8") = e;
	register uint64_t r10 asm("r10") = d;

	uint64_t ret;
	asm volatile("syscall"
		     : "=a"(ret)
		     : "0"(num), "D"(a), "S"(b), "d"(c), "r"(r10), "r"(r8)
		     : "rcx", "r11", "memory");
	return ret;
}
#else
#	error Only the amd64 architecture is supported for now!
#endif
