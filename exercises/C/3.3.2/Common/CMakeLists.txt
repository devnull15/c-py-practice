# For catching assertions in the test harness
add_library(wrap_assert STATIC "wrap_assert/wrap_assert.c")
target_include_directories(wrap_assert PUBLIC "wrap_assert")
set_target_properties(wrap_assert PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(wrap_assert PRIVATE WRAP_ASSERT)

# The counterpart to wrap_assert. To be used in the test harness
add_library(test_wrap_assert STATIC "test_wrap_assert/test_wrap_assert.c")
target_include_directories(test_wrap_assert PUBLIC "test_wrap_assert")
set_target_properties(test_wrap_assert PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(test_wrap_assert PUBLIC wrap_assert)

