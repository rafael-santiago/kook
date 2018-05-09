/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <test/cutest/src/kutest.h>
#include <kook.h>

static void *original_mkdir = NULL;

KUTE_DECLARE_TEST_CASE(kook_test_monkey);
KUTE_DECLARE_TEST_CASE(hook_test);
KUTE_DECLARE_TEST_CASE(unhook_test);
KUTE_DECLARE_TEST_CASE(get_syscall_table_addr_test);

KUTE_TEST_CASE(get_syscall_table_addr_test)
    KUTE_ASSERT(get_syscall_table_addr() != NULL);
KUTE_TEST_CASE_END

KUTE_TEST_CASE(kook_test_monkey)
    KUTE_RUN_TEST(get_syscall_table_addr_test);
    KUTE_RUN_TEST(hook_test);
    KUTE_RUN_TEST(unhook_test);
KUTE_TEST_CASE_END

KUTE_MAIN(kook_test_monkey);

#if defined(__FreeBSD__)

#include <sys/proc.h>
#include <sys/syscallsubr.h>
#include <sys/syscall.h>
#include <sys/sysproto.h>
#include <sys/fcntl.h>

static int dummy_mkdir(struct thread *td, void *args);

KUTE_TEST_CASE(hook_test)
    KUTE_ASSERT(kook(SYS_mkdir, dummy_mkdir, &original_mkdir) == 0);
    KUTE_ASSERT(original_mkdir == (void *)sys_mkdir);
    KUTE_ASSERT((sy_call_t *)dummy_mkdir == sysent[SYS_mkdir].sy_call);
KUTE_TEST_CASE_END

KUTE_TEST_CASE(unhook_test)
    KUTE_ASSERT(kook(SYS_mkdir, original_mkdir, NULL) == 0);
    KUTE_ASSERT(original_mkdir == (void *)sysent[SYS_mkdir].sy_call);
KUTE_TEST_CASE_END

static int dummy_mkdir(struct thread *td, void *args) {
    return sys_mkdir(td, args);
}

#elif defined(__linux__)

#include <linux/unistd.h>

asmlinkage long dummy_mkdir(const char __user *pathname, umode_t mode);
asmlinkage long (*sys_mkdir)(const char __user *pathname, umode_t mode);

KUTE_TEST_CASE(hook_test)
    kook_syscall_table_t sy_table = get_syscall_table_addr();
    sys_mkdir = (void *)sy_table[__NR_mkdir];
    KUTE_ASSERT(kook(__NR_mkdir, dummy_mkdir, &original_mkdir) == 0);
    KUTE_ASSERT(original_mkdir == (void *)sys_mkdir);
    KUTE_ASSERT((void *)dummy_mkdir == sy_table[__NR_mkdir]);
KUTE_TEST_CASE_END

KUTE_TEST_CASE(unhook_test)
    kook_syscall_table_t sy_table = get_syscall_table_addr();
    KUTE_ASSERT(kook(__NR_mkdir, original_mkdir, NULL) == 0);
    KUTE_ASSERT(original_mkdir == sy_table[__NR_mkdir]);
KUTE_TEST_CASE_END

asmlinkage long dummy_mkdir(const char __user *pathname, umode_t mode) {
    return sys_mkdir(pathname, mode);
}

#elif defined(__NetBSD__)

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/syscall.h>
#include <sys/syscallargs.h>

static int dummy_mkdir(struct lwp *, const struct sys_mkdir_args *, 
register_t *);

KUTE_TEST_CASE(hook_test)
   KUTE_ASSERT(kook(SYS_mkdir, dummy_mkdir, &original_mkdir) == 0);
   KUTE_ASSERT(original_mkdir == (void *)sys_mkdir);
   KUTE_ASSERT((sy_call_t *)dummy_mkdir == sysent[SYS_mkdir].sy_call);
KUTE_TEST_CASE_END

KUTE_TEST_CASE(unhook_test)
   KUTE_ASSERT(kook(SYS_mkdir, original_mkdir, NULL) == 0);
   KUTE_ASSERT(original_mkdir == (void *)sysent[SYS_mkdir].sy_call);
KUTE_TEST_CASE_END

static int dummy_mkdir(struct lwp *lp, const struct sys_mkdir_args *ap, 
register_t *rp) {
   return sys_mkdir(lp, ap, rp);
}

#endif
