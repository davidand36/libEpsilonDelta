# - Find WiiYourself
# Find the native WiiYourself! includes and library
# This module defines
#  WiiYourself_FOUND, If false, do not try to use WiiYourself.
#  WiiYourself_INCLUDE_DIR, where to find sqlite3.h, etc.
#  WiiYourself_LIBRARIES, the libraries needed to use WiiYourself.

find_path( WiiYourself_INCLUDE_DIR wiimote.h)

set( WiiYourself_NAMES  ${WiiYourself_NAMES} WiiYourself! )
find_library( WiiYourself_LIBRARY NAMES ${WiiYourself_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set WiiYourself_FOUND to TRUE
# if all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs )
find_package_handle_standard_args( WiiYourself  DEFAULT_MSG
                                  WiiYourself_LIBRARY  WiiYourself_INCLUDE_DIR )

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
