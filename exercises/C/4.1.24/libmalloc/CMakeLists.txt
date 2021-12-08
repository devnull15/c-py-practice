add_library(malloc SHARED libmalloc.c)
target_link_libraries(malloc dl)
