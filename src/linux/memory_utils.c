/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <linux/memory_utils.h>

// INFO(Rafael): If you want to use read/write_cr0 functions define the following macro.
#undef KOOK_HAS_SPECIAL_INSNS

#if defined(KOOK_SPECIAL_INSNS)
# include <linux/module.h>
#endif

void kook_enable_memory_protection(void) {
#if !defined(KOOK_HAS_SPECIAL_INSNS)
# if defined(__i386__)
    __asm__ __volatile__ ("cli\n\t"
                          "mov %%cr0, %%eax\n\t"
                          "or $0x00010000, %%eax\n\t"
                          "mov %%eax, %%cr0\n\t"
                          "sti" :);
# elif defined(__x86_64__)
    __asm__ __volatile__ ("cli\n\t"
                          "mov %%cr0, %%rax\n\t"
                          "or $0x0000000000010000, %%rax\n\t"
                          "mov %%rax, %%cr0\n\t"
                          "sti" :);
# else
#  error "You must define KOOK_HAS_SPECIAL_INSNS macro."
# endif
#else
    write_cr0(read_cr0() | 0x00010000);
#endif
}

void kook_disable_memory_protection(void) {
#if !defined(KOOK_HAS_SPECIAL_INSNS)
# if defined(__i386__)
    __asm__ __volatile__ ("cli\n\t"
                          "mov %%cr0, %%eax\n\t"
                          "and $0xFFFEFFFF, %%eax\n\t"
                          "mov %%eax, %%cr0\n\t"
                          "sti" :);
# elif defined(__x86_64__)
    __asm__ __volatile__ ("cli\n\t"
                          "mov %%cr0, %%rax\n\t"
                          "and $0xFFFFFFFFFFFEFFFF, %%rax\n\t"
                          "mov %%rax, %%cr0\n\t"
                          "sti" :);
# else
#  error "You must define KOOK_HAS_SPECIAL_INSNS macro."
# endif
#else
    write_cr0(read_cr0() & 0xFFFEFFFF);
#endif
}
