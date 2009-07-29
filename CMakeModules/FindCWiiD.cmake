# - FindCWiiD
# Find the native CWiiD includes and library
# This module defines
#  CWiiD_FOUND, If false, do not try to use CWiiD.
#  CWiiD_INCLUDE_DIR, where to find sqlite3.h, etc.
#  CWiiD_LIBRARIES, the libraries needed to use CWiiD.

find_path( CWiiD_INCLUDE_DIR cwiid.h)

set( CWiiD_NAMES  ${CWiiD_NAMES} cwiid )
find_library( CWiiD_LIBRARY NAMES ${CWiiD_NAMES} )

#This is for CMake 2.6:
## handle the QUIETLY and REQUIRED arguments and set CWiiD_FOUND to TRUE
## if all listed variables are TRUE
#include( FindPackageHandleStandardArgs )
#find_package_handle_standard_args( CWiiD  DEFAULT_MSG
#                                   CWiiD_LIBRARY  CWiiD_INCLUDE_DIR )
#This is for CMake 2.4:
if ( CWiiD_INCLUDE_DIR  AND  CWiiD_LIBRARY )
    set( CWiiD_LIBRARIES  ${CWiiD_LIBRARY} )
    set( CWiiD_FOUND  "YES" )
    if ( NOT CWiiD_FIND_QUIETLY )
        message( STATUS  "Found CWiiD: ${CWiiD_LIBRARIES}" )
    endif ( NOT CWiiD_FIND_QUIETLY )
else ( CWiiD_INCLUDE_DIR  AND  CWiiD_LIBRARY )
    set( CWiiD_FOUND  "NO" )
    if ( CWiiD_FIND_REQUIRED )
        message( FATAL_ERROR  "Could not find CWiiD library" )
    endif ( CWiiD_FIND_REQUIRED )
endif ( CWiiD_INCLUDE_DIR  AND  CWiiD_LIBRARY )

if( CWiiD_FOUND )
    set( CWiiD_LIBRARIES  ${CWiiD_LIBRARY} )
endif( CWiiD_FOUND )

# Deprecated declarations.
set( NATIVE_CWiiD_INCLUDE_PATH  ${CWiiD_INCLUDE_DIR} )
if( CWiiD_LIBRARY )
    get_filename_component( NATIVE_CWiiD_LIB_PATH
                            ${CWiiD_LIBRARY} PATH )
endif( CWiiD_LIBRARY )

mark_as_advanced( CWiiD_LIBRARY  CWiiD_INCLUDE_DIR )
