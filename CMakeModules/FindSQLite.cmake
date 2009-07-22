# - Find SQLite
# Find the SQLite includes and library
# This module defines
#  SQLite_FOUND, If false, do not try to use SQLite.
#  SQLite_INCLUDE_DIR, where to find sqlite3.h, etc.
#  SQLite_LIBRARIES, the libraries needed to use SQLite3.

find_path( SQLite_INCLUDE_DIR sqlite3.h)

set( SQLite_NAMES  ${SQLite_NAMES} sqlite3 )
find_library( SQLite_LIBRARY NAMES ${SQLite_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set SQLite_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs )
find_package_handle_standard_args( SQLite  DEFAULT_MSG
                                   SQLite_LIBRARY  SQLite_INCLUDE_DIR )

if( SQLite_FOUND )
    set( SQLite_LIBRARIES  ${SQLite_LIBRARY} )
endif( SQLite_FOUND )

# Deprecated declarations.
set( NATIVE_SQLite_INCLUDE_PATH  ${SQLite_INCLUDE_DIR} )
if( SQLite_LIBRARY )
    get_filename_component( NATIVE_SQLite_LIB_PATH  ${SQLite_LIBRARY} PATH )
endif( SQLite_LIBRARY )

mark_as_advanced( SQLite_LIBRARY  SQLite_INCLUDE_DIR )
