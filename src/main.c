/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <krooked.h>
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Rafael Santiago");

MODULE_DESCRIPTION("Just a test for this stuff");

MODULE_VERSION("0.0.0");

static int __init ini(void) {
    return 0;
}

static void __exit finis(void) {
}

module_init(ini);
module_exit(finis);


