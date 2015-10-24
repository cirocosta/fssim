# Try to find GNU Readline 
# (https://cnswww.cns.cwru.edu/php/chet/readline/rltop.html)
# Once done, defines:
#   READLINE_FOUND        -  has found readline dependency
#   READLINE_INCLUDE_DIRS -  include directories
#   READLINE_LIBRARIES    -  library to link against

find_path(READLINE_INCLUDE_DIR 
  NAMES readline/readline.h 
  PATHS /usr/local/include 
        /usr/include
)

find_library(READLINE_LIBRARY 
  NAMES readline 
  PATHS /usr/local/lib
        /usr/lib
)

mark_as_advanced(
  READLINE_INCLUDE_DIR
  READLINE_LIBRARY
)

set(READLINE_LIBRARIES ${READLINE_LIBRARY})
set(READLINE_INCLUDE_DIRS ${READLINE_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  READLINE DEFAULT_MSG
  READLINE_LIBRARY READLINE_INCLUDE_DIR
)
