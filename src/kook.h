/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef KOOK_H
#define KOOK_H 1

# if defined(__linux__)
#  include <linux/get_syscall_table_addr.h>
#  include <linux/memory_utils.h>
#  include <linux/kernel.h>

   typedef unsigned long ** kook_syscall_table_t;

# elif defined(__FreeBSD__)
#  include <freebsd/get_syscall_table_addr.h>
#  include <sys/sysent.h>

   typedef struct sysent * kook_syscall_table_t;

# elif defined(__NetBSD__)
#  include <netbsd/get_syscall_table_addr.h>
#  include <sys/systm.h>

   typedef struct sysent * kook_syscall_table_t;

# else
#  error "Not supported platform."
# endif

int kook(const int syscall_nr, void *new_addr, void **old_addr);

#endif
