/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <freebsd/get_syscall_table_addr.h>
#include <sys/sysent.h>

void *get_syscall_table_addr(void) {
    return &sysent[0];
}
