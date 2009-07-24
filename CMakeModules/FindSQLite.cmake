# - Find SQLite
# Find the SQLite includes and library
# This module defines
#  SQLite_FOUND, If false, do not try to use SQLite.
#  SQLite_INCLUDE_DIR, where to find sqlite3.h, etc.
#  SQLite_LIBRARIES, the libraries needed to use SQLite3.

find_path( SQLite_INCLUDE_DIR sqlite3.h)

set( SQLite_NAMES  ${SQLite_NAMES} sqlite3 )
find_library( SQLite_LIBRARY  NAMES ${SQLite_NAMES} )


#This is for CMake 2.6:
## handle the QUIETLY and REQUIRED arguments and set SQLite_FOUND to TRUE if 
## all listed variables are TRUE
#include( FindPackageHandleStandardArgs )
#find_package_handle_standard_args( SQLite  DEFAULT_MSG
#                                   SQLite_LIBRARY  SQLite_INCLUDE_DIR )
#This is for CMake 2.4:
if ( SQLite_INCLUDE_DIR  AND  SQLite_LIBRARY )
    set( SQLite_LIBRARIES  ${SQLite_LIBRARY} )
    set( SQLite_FOUND  "YES" )
    if ( NOT SQLite_FIND_QUIETLY )
        message( STATUS  "Found SQLite: ${SQLite_LIBRARIES}" )
    endif ( NOT SQLite_FIND_QUIETLY )
else ( SQLite_INCLUDE_DIR  AND  SQLite_LIBRARY )
    set( SQLite_FOUND  "NO" )
    if ( SQLite_FIND_REQUIRED )
        message( FATAL_ERROR  "Could not find SQLite library" )
    endif ( SQLite_FIND_REQUIRED )
endif ( SQLite_INCLUDE_DIR  AND  SQLite_LIBRARY )

if ( SQLite_FOUND )
    set( SQLite_LIBRARIES  ${SQLite_LIBRARY} )
endif ( SQLite_FOUND )

# Deprecated declarations.
set( NATIVE_SQLite_INCLUDE_PATH  ${SQLite_INCLUDE_DIR} )
if ( SQLite_LIBRARY )
    get_filename_component( NATIVE_SQLite_LIB_PATH  ${SQLite_LIBRARY} PATH )
endif ( SQLite_LIBRARY )

mark_as_advanced( SQLite_LIBRARY  SQLite_INCLUDE_DIR )
