cmake_minimum_required(VERSION 3.13)

set(PROJECT bsle-prep)
project(${PROJECT} LANGUAGES C)

add_compile_options(-Werror -Wextra -Wall -pedantic -g -fsanitize=address)
link_libraries(-fsanitize=address)

option(LL "" ON)

configure_file(${PROJECT_SOURCE_DIR}/include/test.h.in ${PROJECT_SOURCE_DIR}/include/test.h)

if(LL)
  add_subdirectory(ll)
  list(APPEND INCLUDES ll/include)
  list(APPEND LIBS ll)
endif()

list(APPEND SOURCES src/test.c)
list(APPEND INCLUDES ${PROJECT_SOURCE_DIR}/include)

include_directories(${INCLUDES})
add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBS})
