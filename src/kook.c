/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <kook.h>

static int g_kook_syscall_table_init_done = 0;

static kook_syscall_table_t g_kook_syscall_table;

int kook(const int syscall_nr, void *new_addr, void **old_addr) {
    if (!g_kook_syscall_table_init_done) {
        g_kook_syscall_table = get_syscall_table_addr();
        g_kook_syscall_table_init_done = (g_kook_syscall_table != NULL);
    }

    if (!g_kook_syscall_table_init_done) {
        return 1;
    }

#if defined(__linux__)

    if (old_addr != NULL) {
        (*old_addr) = (void *) g_kook_syscall_table[syscall_nr];
    }

    kook_set_page_rd_wr(**g_kook_syscall_table);

    g_kook_syscall_table[syscall_nr] = new_addr;

    kook_set_page_rd(**g_kook_syscall_table);

#elif defined(__FreeBSD__)

    if (old_addr != NULL) {
        *old_addr = (void *) g_kook_syscall_table[syscall_nr].sy_call;
    }

    g_kook_syscall_table[syscall_nr].sy_call = (sy_call_t *) new_addr;

#else

    return 1;

#endif

    return 0;
}
