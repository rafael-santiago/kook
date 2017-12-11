/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <linux/memory_utils.h>
#include <linux/module.h>

void kook_enable_memory_protection(void) {
    // TODO(Rafael): Maybe use inline assembly to make it less version dependent.
    write_cr0(read_cr0() | 0x00010000);
}

void kook_disable_memory_protection(void) {
    // TODO(Rafael): Maybe use inline assembly to make it less version dependent.
    write_cr0(read_cr0() & 0xFFFEFFFF);
}
