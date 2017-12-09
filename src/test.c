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

static int create_file(void);

static void close_file(const int fd);

static int file_exists(void);

static void remove_file(void);

static int dummy_mkdir(struct thread *td, void *args);

KUTE_TEST_CASE(hook_test)
    KUTE_ASSERT(kook(SYS_mkdir, dummy_mkdir, &original_mkdir) == 0);
    KUTE_ASSERT(original_mkdir == (void *)sys_mkdir);
    KUTE_ASSERT(kern_mkdirat(curthread, AT_FDCWD, KOOK_TEST_DIRNAME, UIO_SYSSPACE, 0644) == 0);
    KUTE_ASSERT(file_exists() == 1);
    remove_file();
    KUTE_ASSERT(file_exists() == 0);
    KUTE_ASSERT(kern_rmdirat(curthread, AT_FDCWD, KOOK_TEST_DIRNAME, UIO_SYSSPACE) == 0);
KUTE_TEST_CASE_END

KUTE_TEST_CASE(unhook_test)
    KUTE_ASSERT(kook(SYS_mkdir, original_mkdir, NULL) == 0);
    KUTE_ASSERT(kern_mkdirat(curthread, AT_FDCWD, KOOK_TEST_DIRNAME, UIO_SYSSPACE, 0644) == 0);
    KUTE_ASSERT(file_exists() == 0);
    KUTE_ASSERT(kern_rmdirat(curthread, AT_FDCWD, KOOK_TEST_DIRNAME, UIO_SYSSPACE) == 0);
KUTE_TEST_CASE_END

static int create_file(void) {
    int error;

    error = kern_openat(curthread, AT_FDCWD, KOOK_TEST_FILE_PATH, UIO_SYSSPACE, O_WRONLY | O_CREAT | O_APPEND, 0644);

    if (error == 0) {
        return curthread->td_retval[0];
    }

    return -1;
}

static void close_file(const int fd) {
    kern_close(curthread, fd);
}

static int file_exists(void) {
    int exists = (kern_openat(curthread, AT_FDCWD, KOOK_TEST_FILE_PATH, UIO_SYSSPACE, O_RDONLY, 0644) == 0);

    if (exists) {
        kern_close(curthread, curthread->td_retval[0]);
    }

    return exists;
}

static void remove_file(void) {
    kern_unlinkat(curthread, AT_FDCWD, KOOK_TEST_FILE_PATH, UIO_USERSPACE, 0);
}

static int dummy_mkdir(struct thread *td, void *args) {
    close_file(create_file());
    return sys_mkdir(td, args);
}

#endif

#undef KOOK_TEST_FILE_PATH
#undef KOOK_TEST_DIRNAME
