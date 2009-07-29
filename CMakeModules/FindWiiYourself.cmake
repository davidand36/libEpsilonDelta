# - FindWiiYourself
# Find the native WiiYourself! includes and library
# This module defines
#  WiiYourself_FOUND, If false, do not try to use WiiYourself.
#  WiiYourself_INCLUDE_DIR, where to find sqlite3.h, etc.
#  WiiYourself_LIBRARIES, the libraries needed to use WiiYourself.

find_path( WiiYourself_INCLUDE_DIR wiimote.h)

set( WiiYourself_NAMES  ${WiiYourself_NAMES} WiiYourself! )
find_library( WiiYourself_LIBRARY NAMES ${WiiYourself_NAMES} )

#This is for CMake 2.6:
## handle the QUIETLY and REQUIRED arguments and set WiiYourself_FOUND to TRUE
## if all listed variables are TRUE
#INCLUDE( FindPackageHandleStandardArgs )
#find_package_handle_standard_args( WiiYourself  DEFAULT_MSG
#                                  WiiYourself_LIBRARY  WiiYourself_INCLUDE_DIR )
#This is for CMake 2.4:
if ( WiiYourself_INCLUDE_DIR  AND  WiiYourself_LIBRARY )
    set( WiiYourself_LIBRARIES  ${WiiYourself_LIBRARY} )
    set( WiiYourself_FOUND  "YES" )
    if ( NOT WiiYourself_FIND_QUIETLY )
        message( STATUS  "Found WiiYourself: ${WiiYourself_LIBRARIES}" )
    endif ( NOT WiiYourself_FIND_QUIETLY )
else ( WiiYourself_INCLUDE_DIR  AND  WiiYourself_LIBRARY )
    set( WiiYourself_FOUND  "NO" )
    if ( WiiYourself_FIND_REQUIRED )
        message( FATAL_ERROR  "Could not find WiiYourself library" )
    endif ( WiiYourself_FIND_REQUIRED )
endif ( WiiYourself_INCLUDE_DIR  AND  WiiYourself_LIBRARY )

if( WiiYourself_FOUND )
    set( WiiYourself_LIBRARIES  ${WiiYourself_LIBRARY} )
endif( WiiYourself_FOUND )

# Deprecated declarations.
set( NATIVE_WiiYourself_INCLUDE_PATH  ${WiiYourself_INCLUDE_DIR} )
if( WiiYourself_LIBRARY )
    get_filename_component( NATIVE_WiiYourself_LIB_PATH
                            ${WiiYourself_LIBRARY} PATH )
endif( WiiYourself_LIBRARY )

mark_as_advanced( WiiYourself_LIBRARY  WiiYourself_INCLUDE_DIR )
