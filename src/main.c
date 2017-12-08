/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <cutest/src/kutest.h>
#include <kook.h>

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
