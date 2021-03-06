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

#ifndef FLIBC_LINUX_H
#define FLIBC_LINUX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>

/**
 * Linux/UNIX specifix functions, structs and macros.
 */

#define EINTR 4
#define EAGAIN 11

/* For clone. */
#define CLONE_FS 0x00000200
#define CLONE_FILES 0x00000400
#define CLONE_PARENT 0x00008000
#define CLONE_IO 0x80000000

/* For kill. */
#define SIGKILL 9

/* For socket and bind. */
#define AF_INET 2
#define INADDR_ANY 0x00000000

/* For socket and accept4. */
#define SOCK_CLOEXEC O_CLOEXEC
#define SOCK_NONBLOCK O_NONBLOCK

#define O_CLOEXEC 02000000
#define O_NONBLOCK 00004000

/* For epoll_create1. */
#define EPOLL_CLOEXEC O_CLOEXEC

/* For epoll_ctl. */
#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_DEL 2
#define EPOLL_CTL_MOD 3

/* For epoll_ctl. */
#define EPOLLIN 0x00000001
#define EPOLLOUT 0x00000004
#define EPOLLERR 0x00000008
#define EPOLLRDHUP 0x00002000
#define EPOLLEXCLUSIVE (1U << 28)
#define EPOLLWAKEUP (1U << 29)
#define EPOLLET (1U << 31)

/* For clock_gettime. */
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1

/* Used in various syscalls such as read and write. */
typedef int64_t ssize_t;

/* For open. */
typedef unsigned short umode_t;

/* For socket and bind. */
enum socktype {
	SOCK_STREAM = 1,
};

/* For bind. */
struct sockaddr {
	unsigned short sa_family;
	char sa_data[14];
};

/* For bind. */
struct sockaddr_in {
	unsigned short sin_family;
	uint16_t sin_port;
	uint32_t sin_addr;
	char padding[8];
};

/* For epoll_ctl. */
typedef union epoll_data {
	void *ptr;
	int fd;
	uint32_t u32;
	uint64_t u64;
} epoll_data_t;

/* For epoll_ctl. */
struct epoll_event {
	int events;
	epoll_data_t data;
} __attribute__((packed));

/* For clock_gettime. */
typedef int clockid_t;

/* For clock_gettime and nanosleep. */
typedef long time_t;

/* For clock_gettime and nanosleep. */
struct timespec {
	time_t tv_sec;
	long tv_nsec;
};

/* For clone. */
typedef int pid_t;

ssize_t sys_read(int fd, void *buf, size_t count);
ssize_t sys_write(int fd, const void *buf, size_t count);
int sys_open(const char *path, int flags, umode_t mode);
int sys_close(int fd);
noreturn void sys_exit(int code);

pid_t sys_clone(unsigned long flags, void *stack, int *parent_tid,
	int *child_tid, unsigned long tls);
int sys_kill(pid_t pid, int signal);

int sys_socket(int family, int type, int protocol);
int sys_bind(int fd, struct sockaddr *addr, size_t addr_len);
int sys_listen(int fd, int backlog);
int sys_accept4(
	int fd, struct sockaddr *peer_addr, int *peer_addr_len, int flags);

int sys_epoll_create1(int flags);
int sys_epoll_ctl(int epoll_fd, int op, int fd, struct epoll_event *event);
int sys_epoll_wait(
	int epoll_fd, struct epoll_event *events, int max_events, int timeout);

int sys_clock_gettime(clockid_t clock, struct timespec *time);
int sys_nanosleep(const struct timespec *rqtp, struct timespec *rmtp);

#endif
