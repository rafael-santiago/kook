/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef KROOKED_H
#define KROOKED_H 1

#if defined(__linux__)
#include <linux/get_syscall_table_addr.h>
#include <linux/kernel.h>

typedef unsigned long ** krooked_syscall_table_t;

#endif

int krooked_syscall_hook(const int syscall_nr, void *new_addr, void *old_addr);

#endif
