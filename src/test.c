/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <cutest/src/kutest.h>
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

#endif
