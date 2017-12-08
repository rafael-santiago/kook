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

#if defined(__FreeBSD__)

#include <sys/proc.h>
#include <sys/syscallsubr.h>
#include <sys/fcntl.h>

static int create_file(void);

static void close_file(const int fd);

static int file_exists(void);

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
    int error = kern_openat(curthread, AT_FDCWD, KOOK_TEST_FILE_PATH, UIO_SYSSPACE, O_RDONLY, 0644);
    int exists = (error == 0);

    if (exists) {
        kern_close(curthread, curthread->td_retval[0]);
    }

    return exists;
}

// TODO(Rafael): Remove the file.

#endif


KUTE_DECLARE_TEST_CASE(kook_test_monkey);

KUTE_DECLARE_TEST_CASE(hook_test);

KUTE_DECLARE_TEST_CASE(unhook_test);

KUTE_TEST_CASE(hook_test)
    // TODO(Rafael): Guess what?
KUTE_TEST_CASE_END

KUTE_TEST_CASE(unhook_test)
    // TODO(Rafael): Guess what?
KUTE_TEST_CASE_END

KUTE_TEST_CASE(kook_test_monkey)
    KUTE_RUN_TEST(hook_test);
    KUTE_RUN_TEST(unhook_test);
KUTE_TEST_CASE_END

KUTE_MAIN(kook_test_monkey);
