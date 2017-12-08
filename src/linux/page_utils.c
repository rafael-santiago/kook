/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <linux/page_utils.h>
#include <asm/cacheflush.h>

void kook_set_page_rd_wr(unsigned long address) {
    unsigned int lv;
    pte_t *page = lookup_address(address, &lv);
    if (!(page->pte & _PAGE_RW)) {
        page->pte |= _PAGE_RW;
    }
}

void kook_set_page_rd(unsigned long address) {
    unsigned int lv;
    pte_t *page = lookup_address(address, &lv);
    page->pte = page->pte & (~_PAGE_RW);
}
