# - Find PostgreSQL
# Find the PostgreSQL includes and library
# This module defines
#  PostgreSQL_FOUND, If false, do not try to use PostgreSQL.
#  PostgreSQL_INCLUDE_DIR, where to find sqlite3.h, etc.
#  PostgreSQL_LIBRARIES, the libraries needed to use PostgreSQL3.

find_path( PostgreSQL_INCLUDE_DIR postgresql/libpq-fe.h )

set( PostgreSQL_NAMES  ${PostgreSQL_NAMES} pq )
find_library( PostgreSQL_LIBRARY  NAMES ${PostgreSQL_NAMES} )


#This is for CMake 2.6:
## handle the QUIETLY and REQUIRED arguments and set PostgreSQL_FOUND to TRUE 
## if all listed variables are TRUE
#include( FindPackageHandleStandardArgs )
#find_package_handle_standard_args( PostgreSQL  DEFAULT_MSG
#                                   PostgreSQL_LIBRARY  PostgreSQL_INCLUDE_DIR )
#This is for CMake 2.4:
if ( PostgreSQL_INCLUDE_DIR  AND  PostgreSQL_LIBRARY )
    set( PostgreSQL_LIBRARIES  ${PostgreSQL_LIBRARY} )
    set( PostgreSQL_FOUND  "YES" )
    if ( NOT PostgreSQL_FIND_QUIETLY )
        message( STATUS  "Found PostgreSQL: ${PostgreSQL_LIBRARIES}" )
    endif ( NOT PostgreSQL_FIND_QUIETLY )
else ( PostgreSQL_INCLUDE_DIR  AND  PostgreSQL_LIBRARY )
    set( PostgreSQL_FOUND  "NO" )
    if ( PostgreSQL_FIND_REQUIRED )
        message( FATAL_ERROR  "Could not find PostgreSQL library" )
    endif ( PostgreSQL_FIND_REQUIRED )
endif ( PostgreSQL_INCLUDE_DIR  AND  PostgreSQL_LIBRARY )

if ( PostgreSQL_FOUND )
    set( PostgreSQL_LIBRARIES  ${PostgreSQL_LIBRARY} )
endif ( PostgreSQL_FOUND )

# Deprecated declarations.
set( NATIVE_PostgreSQL_INCLUDE_PATH  ${PostgreSQL_INCLUDE_DIR} )
if ( PostgreSQL_LIBRARY )
    get_filename_component( NATIVE_PostgreSQL_LIB_PATH
                            ${PostgreSQL_LIBRARY} PATH )
endif ( PostgreSQL_LIBRARY )

mark_as_advanced( PostgreSQL_LIBRARY  PostgreSQL_INCLUDE_DIR )
