cmake_minimum_required(VERSION 3.13)

set(PROJECT bsle-prep)
project(${PROJECT} LANGUAGES C)

add_compile_options(-Werror -Wextra -Wall -pedantic -g -fsanitize=address)
link_libraries(-fsanitize=address)

add_subdirectory(ll)

include_directories(ll/include)


set(SOURCES
  src/test.c
  )

set(LIBS
  ll
  )

add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBS})
