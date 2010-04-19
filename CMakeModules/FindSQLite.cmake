# - Find SQLite
# Find the SQLite includes and library
# This module defines
#  SQLite_FOUND, If false, do not try to use SQLite.
#  SQLite_INCLUDE_DIR, where to find sqlite3.h, etc.
#  SQLite_LIBRARIES, the libraries needed to use SQLite3.

find_path( SQLite_INCLUDE_DIR sqlite3.h)

set( SQLite_NAMES  ${SQLite_NAMES} sqlite3 )
find_library( SQLite_LIBRARIES  NAMES ${SQLite_NAMES} )

find_package_handle_standard_args( SQLite  DEFAULT_MSG
                                   SQLite_LIBRARIES  SQLite_INCLUDE_DIR )
set( SQLite_FOUND  ${SQLITE_FOUND} )

mark_as_advanced( SQLite_LIBRARIES  SQLite_INCLUDE_DIR )
