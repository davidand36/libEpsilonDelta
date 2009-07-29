#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# Top-level CMake script for EpsilonDelta libraries.
#
#
cmake_minimum_required( VERSION 2.4 )

project( libEpsilonDelta )

set( libraries
     util math time file sql cgi
     astro calendar
     mmbase graphics2d input
     )

set( tests
     util math time file sql cgi
     astro calendar
     mmbase graphics2d input
     )


set( CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules )
include( CheckIncludeFile )
include( CheckLibraryExists )

if ( ${MSVC} )
    add_definitions( -D_USE_MATH_DEFINES )
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
    add_definitions( -D_SCL_SECURE_NO_WARNINGS )
endif ( ${MSVC} )

option( release  "Build for release"  false )
if ( release )
   add_definitions( -DNDEBUG )
   #!!!optimizations
else ( release )
   add_definitions( -DDEBUG )
   #!!!debug data
endif ( release )

option( profile  "Build for profiling"  false )
if ( profile )
    #!!!compiler settings
endif ( profile )

#!!! STLport

set( useBoost true )  #required, for now at least
if ( useBoost )
    find_package( Boost  REQUIRED )
    if ( Boost_FOUND )
        add_definitions( -DUSE_BOOST )
        include_directories( "${Boost_INCLUDE_DIRS}" )
    endif ( Boost_FOUND )
endif ( useBoost )

option( useFastCGI  "True to use FastCGI"  true )
if ( useFastCGI )
    find_package( FastCGI  REQUIRED )
    if ( FastCGI_FOUND )
        add_definitions( -DUSE_FASTCGI )
        include_directories( ${FastCGI_INCLUDE_DIR} )
    endif ( FastCGI_FOUND )
endif ( useFastCGI )

option( useSQLite  "True to use SQLite"  true )
if ( useSQLite )
    find_package( SQLite  REQUIRED )
    if ( SQLite_FOUND )
        add_definitions( -DUSE_SQLITE )
        include_directories( ${SQLite_INCLUDE_DIR} )
    endif ( SQLite_FOUND )
endif ( useSQLite )

option( useMySQL  "True to use MySQL"  false )
if ( useMySQL )
    find_package( MySQL  REQUIRED )
    if ( MYSQL_FOUND )
        add_definitions( -DUSE_MYSQL )
        include_directories( ${MYSQL_INCLUDE_DIR} )
    endif ( MYSQL_FOUND )
endif ( useMySQL )

option( usePostgreSQL  "True to use PostgreSQL"  false )
if ( usePostgreSQL )
    find_package( PostgreSQL  REQUIRED )
    if ( PostgreSQL_FOUND )
        add_definitions( -DUSE_POSTGRESQL )
        include_directories( ${PostgreSQL_INCLUDE_DIR} )
    endif ( PostgreSQL_FOUND )
endif ( usePostgreSQL )

option( useSDL  "True to use SDL"  true )
if ( useSDL )
    find_package( SDL  REQUIRED )
    if ( SDL_FOUND )
        find_package( SDL_image  REQUIRED )
        if ( SDLIMAGE_FOUND )
            add_definitions( -DUSE_SDL )
            include_directories( "${SDL_INCLUDE_DIR}" )
            include_directories( "${SDLIMAGE_INCLUDE_DIR}" )
        endif ( SDLIMAGE_FOUND )
    endif ( SDL_FOUND )
endif ( useSDL )

option( useOpenGL  "True to use OpenGL"  false )
if ( useOpenGL )
    find_package( OpenGL  REQUIRED )
    if ( OPENGL_FOUND )
        add_definitions( -USE_OPENGL )
        include_directories( ${OPENGL_INCLUDE_DIR} )
    endif ( OPENGL_FOUND )
endif ( useOpenGL )

option( supportWiimote  "True to support Wii remote"  false )
if ( supportWiimote )
    find_package( CWiiD )
    if ( CWiiD_FOUND )
        set( useCWiiD  true )
        add_definitions( -DSUPPORT_WIIMOTE )
        add_definitions( -DUSE_CWIID )
        include_directories( ${CWiiD_INCLUDE_DIR} )
    else ( CWiiD_FOUND )
        find_package( WiiYourself  REQUIRED )
        if ( WiiYourself_FOUND )
            set( useWiiYourself  true )
            add_definitions( -DSUPPORT_WIIMOTE )
            add_definitions( -DUSE_WIIYOURSELF )
            include_directories( ${WiiYourself_INCLUDE_DIR} )
        endif ( WiiYourself_FOUND )
    endif ( CWiiD_FOUND )
endif ( supportWiimote )

add_definitions( -DBITSPERPIXEL=32 )    #!!!


set( LIBRARY_OUTPUT_PATH  ${CMAKE_SOURCE_DIR} )
set( EXECUTABLE_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/tests )

foreach ( lib ${libraries} )
    add_subdirectory( ${lib} )
endforeach( lib )

foreach ( test ${tests} )
    add_subdirectory( ${test}/test )
endforeach( test )
