cmake_minimum_required(VERSION 3.16)

if(NOT TARGET CUnitCI)
  message(WARNING " If this fails because of CIUnit.h then you aren't in the schoolhouse docker container, use the following line: docker run -v '$(pwd):/code' -it registry.cybbh.space/17d/bolc/docker/build:latest /bin/bash")
endif()


set(JQR_ITEM "3.1.6")
project(
    ${JQR_ITEM} 
    LANGUAGES C)

set(ITEM_HEADERS ${JQR_ITEM}_headers)
set(ITEM_SUBMISSION ${JQR_ITEM}_submission)
set(ITEM_TESTS ${JQR_ITEM}_test)
set(ITEM_EXEC ${JQR_ITEM}_exe)

add_library(${ITEM_SUBMISSION} SHARED "src/args.c" "include/args.h")

add_library(${ITEM_TESTS} SHARED "tests/args_test.c")


# add the executable
add_executable(${ITEM_EXEC} tests/main_tests.c "include/args.h" "src/args.c")

target_link_libraries(${ITEM_EXEC} ${ITEM_SUBMISSION} ${ITEM_TESTS} cunit)
target_compile_options(${ITEM_EXEC} PUBLIC "-Wall" "-Wextra" "-g")
add_test(${JQR_ITEM}_tests                ${ITEM_EXEC})
