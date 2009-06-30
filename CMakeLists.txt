#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# Top-level CMake script for EpsilonDelta libraries.
#
#

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


include( CheckIncludeFile )
include( CheckLibraryExists )

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
#(not needed)        link_directories( ${Boost_LIBRARY_DIRS} )
    endif ( Boost_FOUND )
endif ( useBoost )

option( useFastCGI  "True to use FastCGI"  true )
if ( useFastCGI )
    check_include_file( fastcgi.h  haveFastCGI_H )
    if ( haveFastCGI_H )
        add_definitions( -DUSE_FASTCGI )
        #? include_directories
        #? link_directories
    endif ( haveFastCGI_H )
endif ( useFastCGI )

option( useSQLite  "True to use SQLite"  true )
if ( useSQLite )
    check_include_file( sqlite3.h  haveSQLite_H )
    if ( haveSQLite_H )
        add_definitions( -DUSE_SQLITE )
        #? include_directories
        #? link_directories
    endif ( haveSQLite_H )
endif ( useSQLite )

option( useMySQL  "True to use MySQL"  false )
if ( useMySQL )
    check_include_file( mysql.h  haveMySQL_H )
    if ( haveMySQL_H )
        add_definitions( -DUSE_MYSQL )
        #? include_directories
        #? link_directories
    endif ( haveMySQL_H )
endif ( useMySQL )

option( usePostgreSQL  "True to use PostgreSQL"  false )
if ( usePostgreSQL )
    check_include_file( postgres.h  havePostgreSQL_H )
    if ( havePostgreSQL_H )
        add_definitions( -DUSE_POSTGRESQL )
        #? include_directories
        #? link_directories
    endif ( havePostgreSQL_H )
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
    check_include_file( cwiid.h  haveCWiiD_H )
    if ( haveCWiiD_H )
        set( useCWiiD  true )
        add_definitions( -DSUPPORT_WIIMOTE )
        add_definitions( -DUSE_CWIID )
        #? include_directories
        #? link_directories
    else ( haveCWiiD_H )
        check_include_file( wiimote.h  haveWiiYourself_H )
        if ( haveWiiYourself_H )
            set( useWiiYourself  true )
            add_definitions( -DSUPPORT_WIIMOTE )
            add_definitions( -DUSE_WIIYOURSELF )
            #? include_directories
            #? link_directories
        endif ( haveWiiYourself_H )
    endif ( haveCWiiD_H )
endif ( supportWiimote )

add_definitions( -DBITSPERPIXEL=32 )    #!!!


foreach ( lib ${libraries} )
    add_subdirectory( ${lib} )
endforeach( lib )

foreach ( test ${tests} )
    add_subdirectory( ${test}/test )
endforeach( test )
