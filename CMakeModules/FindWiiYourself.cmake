# - FindWiiYourself
# Find the native WiiYourself! includes and library
# This module defines
#  WiiYourself_FOUND, If false, do not try to use WiiYourself.
#  WiiYourself_INCLUDE_DIR, where to find wiimote.h, etc.
#  WiiYourself_LIBRARIES, the libraries needed to use WiiYourself.

find_path( WiiYourself_INCLUDE_DIR wiimote.h)

set( WiiYourself_NAMES  ${WiiYourself_NAMES} WiiYourself! )
find_library( WiiYourself_LIBRARIES  NAMES ${WiiYourself_NAMES} )

set( WinDDK_NAMES  ${WinDDK_NAMES} hid )
find_library( WinDDK_LIBRARIES  NAMES ${WinDDK_NAMES} )

find_package_handle_standard_args( WiiYourself  DEFAULT_MSG
                                WiiYourself_LIBRARIES  WiiYourself_INCLUDE_DIR )
set( WiiYourself_FOUND  ${WIIYOURSELF_FOUND} )

mark_as_advanced( WiiYourself_LIBRARIES  WiiYourself_INCLUDE_DIR
                  WinDDK_LIBRARIES )
