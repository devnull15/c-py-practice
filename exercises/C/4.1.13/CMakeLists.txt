#i have never used cmake before send help

cmake_minimum_required(VERSION 3.16)
set(JQR_ITEM "3.1.12")
project(${JQR_ITEM} "C")

set(ITEM_HEADERS ${JQR_ITEM}_headers)
set(ITEM_SUBMISSION ${JQR_ITEM}_submission)
set(ITEM_TESTS ${JQR_ITEM}_test)
set(ITEM_EXEC ${JQR_ITEM}_exe)

add_subdirectory(libhostname)

add_library(${ITEM_SUBMISSION} SHARED
        "include/send_udp.h" "src/send_udp.c"
        "include/send_tcp.h" "src/send_tcp.c"
        "include/converse_tcp.h" "src/converse_tcp.c"
        "include/converse_udp.h" "src/converse_udp.c"
        "include/send_unix.h" "src/send_unix.c"
        "include/broadcast_udp.h" "src/broadcast_udp.c"
        "include/set_hostname.h" "src/set_hostname.c")

add_library(${ITEM_TESTS} SHARED 
        "tests/common.c"
        "tests/test_send_udp.c"
        "tests/test_send_tcp.c"
        "tests/test_converse_tcp.c"
        "tests/test_converse_udp.c"
        "tests/test_send_unix.c"
        "tests/test_broadcast_udp.c"
        "tests/test_set_hostname.c"
        "tests/test_check_functions.c")

# add the executable
add_executable(${ITEM_EXEC} "tests/main_tests.c" 
	"include/send_udp.h" "src/send_udp.c"
        "include/send_tcp.h" "src/send_tcp.c"
        "include/converse_tcp.h" "src/converse_tcp.c"
        "include/converse_udp.h" "src/converse_udp.c"
        "include/send_unix.h" "src/send_unix.c"
        "include/broadcast_udp.h" "src/broadcast_udp.c"
        "include/set_hostname.h" "src/set_hostname.c")

target_link_libraries(${ITEM_EXEC} ${ITEM_SUBMISSION} ${ITEM_TESTS} pthread
	cunit dl)
target_compile_options(${ITEM_EXEC} PUBLIC "-Wall" "-Wextra" "-g" "-fsanitize=address" "-static-libasan")
target_link_options(${ITEM_EXEC} PUBLIC "-Wall" "-Wextra" "-g" "-fsanitize=address" "-static-libasan")
add_test(${JQR_ITEM}_tests                ${ITEM_EXEC})
