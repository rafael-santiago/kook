/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <cutest/src/kutest.h>
#include <kook.h>

#define KOOK_TEST_FILE_PATH "kook.paragaricutirimirruaru.test.txt"
#define KOOK_TEST_DIRNAME "kook-test"

static void *original_mkdir = NULL;

static int hook_callnr = 0;

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
    struct mkdir_args args;
    KUTE_ASSERT(kook(SYS_mkdir, dummy_mkdir, &original_mkdir) == 0);
    KUTE_ASSERT(original_mkdir == (void *)sys_mkdir);
    KUTE_ASSERT((sy_call_t *)dummy_mkdir == sysent[SYS_mkdir].sy_call);
    // INFO(Rafael): Ascertaining it.
    args.path = KOOK_TEST_DIRNAME;
    args.mode = 0644;
    KUTE_ASSERT(sysent[SYS_mkdir].sy_call(curthread, &args) == 0);
    KUTE_ASSERT(hook_callnr == 1);
    KUTE_ASSERT(kern_rmdirat(curthread, AT_FDCWD, KOOK_TEST_DIRNAME, UIO_SYSSPACE) == 0);
KUTE_TEST_CASE_END

KUTE_TEST_CASE(unhook_test)
    struct mkdir_args args;
    KUTE_ASSERT(kook(SYS_mkdir, original_mkdir, NULL) == 0);
    KUTE_ASSERT(original_mkdir == (void *)sysent[SYS_mkdir].sy_call);
    // INFO(Rafael): Anyway, let's make sure.
    args.path = KOOK_TEST_DIRNAME;
    args.mode = 0644;
    KUTE_ASSERT(sysent[SYS_mkdir].sy_call(curthread, &args) == 0);
    KUTE_ASSERT(hook_callnr == 1);
    KUTE_ASSERT(kern_rmdirat(curthread, AT_FDCWD, KOOK_TEST_DIRNAME, UIO_SYSSPACE) == 0);
KUTE_TEST_CASE_END

static int dummy_mkdir(struct thread *td, void *args) {
    hook_callnr++;
    return sys_mkdir(td, args);
}

#endif

#undef KOOK_TEST_FILE_PATH
#undef KOOK_TEST_DIRNAME
