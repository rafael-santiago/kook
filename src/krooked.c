/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <krooked.h>

static int g_krooked_syscall_table_init_done = 0;

static krooked_syscall_table_t g_krooked_syscall_table;

int krooked_syscall_hook(const int syscall_nr, void *new_addr, void *old_addr) {
    if (!g_krooked_syscall_table_init_done) {
        g_krooked_syscall_table = get_syscall_table_addr();
        g_krooked_syscall_table_init_done = (g_krooked_syscall_table != NULL);
    }

    if (!g_krooked_syscall_table_init_done) {
        return 0;
    }

    if (old_addr != NULL) {
        old_addr = (void *) g_krooked_syscall_table[syscall_nr];
    }

#if defined(__linux__)
    krooked_set_page_rd_wr(**g_krooked_syscall_table);
#endif

    g_krooked_syscall_table[syscall_nr] = new_addr;

#if defined(__linux__)
    krooked_set_page_rd(**g_krooked_syscall_table);
#endif

    return 1;
}
