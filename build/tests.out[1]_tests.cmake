add_test([=[SanityTest.BasicAssertions]=]  /Users/jason/Folder/code/cpp/music/build/tests.out [==[--gtest_filter=SanityTest.BasicAssertions]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[SanityTest.BasicAssertions]=]  PROPERTIES WORKING_DIRECTORY /Users/jason/Folder/code/cpp/music/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  tests.out_TESTS SanityTest.BasicAssertions)
