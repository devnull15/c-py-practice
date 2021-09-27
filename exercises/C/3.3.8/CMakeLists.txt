cmake_minimum_required(VERSION 3.16)

set(JQR_ITEM "3.3.8")
project(
    ${JQR_ITEM} 
    LANGUAGES C)

set(ITEM_HEADERS ${JQR_ITEM}_headers)
set(ITEM_SUBMISSION ${JQR_ITEM}_submission)
set(ITEM_TESTS ${JQR_ITEM}_test)
set(ITEM_EXEC ${JQR_ITEM}_exe)

add_library(${ITEM_SUBMISSION} SHARED "src/hash_table.c" "include/hash_table.h")

add_library(${ITEM_TESTS} SHARED "tests/hash_table_test.c")


# add the executable
add_executable(${ITEM_EXEC} tests/main_tests.c "include/hash_table.h" "src/hash_table.c")

target_link_libraries(${ITEM_EXEC} ${ITEM_SUBMISSION} ${ITEM_TESTS} cunit)
target_compile_options(${ITEM_EXEC} PUBLIC "-Wall" "-Wextra" "-g")
add_test(${JQR_ITEM}_tests                ${ITEM_EXEC})
