add_library(hostname SHARED hostname.c)
target_link_libraries(hostname dl)
