An Overview of EpsilonDelta libraries.
Copyright (C) 2007 David M. Anderson

libEpsilonDelta is a collection of C++ classes and routines. Many are general purpose, and some are specific to domains of interest to me. It is still very much under development; I have a number of improvements in mind as I write this.

libEpsilonDelta is divided into several directories by subject matter:
util -- basic utility classes, routines, and definitions, used all over.
math -- mathematical and numerical analysis classes and functions.
time -- date and time classes.
file -- file and directory routines.
cgi -- some classes for Web apps using the Common Gateway Interface.
sql -- fairly uniform interface to several SQL database systems.
astro -- astronomy routines.
calendar -- classes representing the world's major calendars.
mmbase -- foundation for system-specific multimedia applications.
input -- keyboard, mouse, and joystick/gamepad input.
graphics2d -- a 2-D graphics system.
graphics3d -- a 3-D graphics system.

Dependencies:
Subversion (subversion.tigris.org) -- version control system.
    Only needed to obtain and/or modify the code.
Python (www.python.org) -- interpreted language. SCons is written in this.
    Your path should include this.
SCons (www.scons.org) -- build ("make") system. Generally this is installed in
    your Python directory.
STLport (www.stlport.org) -- this is now optional, since most compilers
    (GCC, MSVC, etc.) now have generally standard-compliant STL libraries.
boost (www.boost.org) -- near-standard C++ libraries. Required.
Loki (loki-lib.sourceforge.net) -- no longer using this library, which
    originated with Andrei Alexandrescu's excellent book, "Modern C++ Design".
SQLite (www.sqlite.org) -- optional, but excellent embedded database system.
PostgreSQL (www.postgresql.org) -- optional, but advanced database system.
MySQL (www.mysql.org) -- optional, but fast database system.
JPL Planetary and Lunar Ephemerides
    (http://iau-comm4.jpl.nasa.gov/access2ephs.html) -- data files needed
    for many of the astronomical and calendric computations. DE405 and DE406
    are the ones currently used. They belong in the astrodata subdirectory of
    this library, at least for running the tests.
SDL (www.libsdl.org) -- Simple Directmedia Layer, a cross-platform multimedia
    library. Required.
    NOTE: For Windows, this needs to be built with statically-linked runtime
    library (Properties->C/C++->CodeGeneration->Runtime Library): /MT or /MTd.
OpenGL (www.opengl.org) -- this 3-D graphics library needs to be available for
    the graphics3d library.

Configuration:
The build environment is configured by editing a file called scons_options.py
in the base directory (the one with SConstruct). It specifies which optional
dependencies to use, where their files are located, and a few other things. A
sample for Windows might be like this:

libraries = 'all'
noTest = [ 'cgi' ]
release = 0
profile = 0
useSTLport = 0
stlportIncludeDir = 'C:/Program Files/STLport'
useBoost = 1
boostDir = 'C:/Boost/include/boost-1_33_1'
useLoki = 0
useSQLite = 1
buildSQLite = 1
sqliteIncludeDir = 'D:/WXP/Common/Libs/SQLite'
sqliteSrcDir = 'D:/WXP/Common/Libs/SQLite'
usePostgreSQL = 0
useMySQL = 0
useSDL = 1
sdlIncludeDir = 'C:/Program Files/SDL/include'
sdlLibDir = 'C:/Program Files/SDL/lib'
useOpenGL = 1
openGLIncludeDir = 'C:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/PlatformSDK/Include/gl'
bitsPerPixel = 32

(I need to document this more fully!!!)
