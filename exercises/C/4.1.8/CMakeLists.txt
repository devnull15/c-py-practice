#i have never used cmake before send help

cmake_minimum_required(VERSION 3.16)
set(JQR_ITEM "3.1.7")
project(${JQR_ITEM} "C")

set(ITEM_HEADERS ${JQR_ITEM}_headers)
set(ITEM_SUBMISSION ${JQR_ITEM}_submission)
set(ITEM_TESTS ${JQR_ITEM}_test)
set(ITEM_EXEC ${JQR_ITEM}_exe)

add_library(${ITEM_SUBMISSION} SHARED "src/file_io.c" "include/file_io.h")

add_library(${ITEM_TESTS} SHARED "tests/file_io_test.c")


# add the executable
add_executable(${ITEM_EXEC} tests/main_tests.c "include/file_io.h" "src/file_io.c")

target_link_libraries(${ITEM_EXEC} ${ITEM_SUBMISSION} ${ITEM_TESTS} cunit)
target_compile_options(${ITEM_EXEC} PUBLIC "-Wall" "-Wextra" "-g")
add_test(${JQR_ITEM}_tests                ${ITEM_EXEC})