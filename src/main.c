/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <kook.h>

#if defined(__linux__)

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

#elif defined(__FreeBSD__)

#include <kook.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>

static int ld(struct module *mod, int cmd, void *arg) {
    int error = 0;

    switch(cmd) {
        case MOD_LOAD:
            break;

        case MOD_UNLOAD:
            break;

        default:
            error = EOPNOTSUPP;
            break;
    }

    return error;
}

static moduledata_t test_mod = {
    "kook-test",
    ld,
    NULL
};

DECLARE_MODULE(kook, test_mod, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);

#endif
