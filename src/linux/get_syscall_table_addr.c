/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <linux/get_syscall_table_addr.h>
#include <linux/kallsyms.h>

#define GET_SYSCALL_TABLE_NAME "sys_call_table"

void *get_syscall_table_addr(void) {
    return (void *) kallsyms_lookup_name(GET_SYSCALL_TABLE_NAME);
}

#undef GET_SYSCALL_TABLE_NAME
