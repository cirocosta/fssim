include_directories(${INCLUDE_DIRS} ${READLINE_INCLUDE_DIRS})

add_executable(fssim fssim.c)

file(GLOB_RECURSE srcs "*.c")
list(REMOVE_ITEM srcs 
     "${CMAKE_CURRENT_SOURCE_DIR}/fssim.c")

add_library(libfssim ${srcs})
target_link_libraries(fssim libfssim)

set(LIBRARIES ${LIBRARIES} "libfssim" PARENT_SCOPE)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${READLINE_INCLUDE_DIRS} PARENT_SCOPE)
