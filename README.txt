An Overview of EpsilonDelta libraries.
Copyright (C) 2007 David M. Anderson

INTRODUCTION

libEpsilonDelta is a collection of C++ classes and routines. Many are general
purpose, and some are specific to domains of interest to me. It is still very
much under development; I have a number of improvements in mind as I write this.
Extensive unit tests are included.

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
graphics2d -- a 2-D graphics system.
input -- keyboard, mouse, and joystick/gamepad input.


REQUIREMENTS

In modern Linux distributions the required tools and libraries
are available as packages in the standard repositories. I've listed the package
names for Ubuntu where they are not obvious. These are generally the same for
Debian, Fedora, etc.
For Windows (and Mac, as far as I know) you will need to obtain them
individually. I've tried to provide useful URLs for this purpose as well as
for additional documentation and information.

Build system:
My current build system uses CMake (www.cmake.org). You will also want
    cmake-qt-gui and/or cmake-curses-gui.
Previously I used SCons (www.scons.org). I've left the SCons scripts in the
repository, but haven't kept them up to date.

Libraries:
General:
boost (www.boost.org, libboost-dev) -- Near-standard C++ libraries. Now that
    the boost smart pointers and array are incorporated in std::tr1, and
    modern C++ implementations include this, Boost is only required for
    random and bind in the math library.

SQL:
Any or all of these libraries may be used, as desired:
SQLite (www.sqlite.org, libsqlite3-dev) -- Excellent embedded database system.
PostgreSQL (www.postgresql.org, libpq-dev) -- Advanced database system.
MySQL (www.mysql.org, libmysqlclient-dev) -- High performance database system.

Graphics & Input:
SDL (www.libsdl.org) -- Simple Directmedia Layer, a cross-platform multimedia
    library. Required for mmbase, input, and grpahics2d.
    NOTE: For Windows, this needs to be built with statically-linked runtime
    library (Properties->C/C++->CodeGeneration->Runtime Library): /MT or /MTd.
PNG (www.libpng.org, libpng-dev) and JPEG (www.ijg.org, libjpeg-dev)
    -- Optional. Used by graphics2d for creation of image files.
Freetype (www.freetype.org) -- Optional. Used by graphics2d for rendering text.
CWiiD (abstrakraft.org/cwiid, libcwiid) (Linux)
    or WiiYourself! (wiiyourself.gl.tter.org) (Windows) -- Optional. Used by
    input for support of Wii remote controller.

Astronomy & Calendar:
JPL Planetary and Lunar Ephemerides
    (http://iau-comm4.jpl.nasa.gov/access2ephs.html) -- Data files needed
    for many of the astronomical and calendric computations. DE405 and DE406
    are the ones currently used, but DE200 is included in tests.
    The library currently expects the big-endian binary files provided by JPL,
    named as follows: astrodata/JPL_DExxx.be, where xxx = 200, 405, and 406,
    and the .be extension stands for big-endian.


BUILD INSTRUCTIONS

CMake:
1) If you don't want to build all of the EpsilonDelta libraries, edit
   CMakeLists.txt to change the libraries and tests lists.
2) mkdir build
   cd build
3) cmake ..
4) ccmake .. or cmake-gui ..
   These provide interfaces where you can set various options. In particular,
   you can specify which of the optional libraries to use. You can also
   provide paths to these libraries if CMake was unable to find them. (On Linux
   CMake can usually find them if they are in the standard locations. On Windows
   you will probably need to help out CMake.) Finally, you can also set
   CMAKE_BUILD_TYPE to Debug, which will also allow for testing.
5) CMake has now created makefiles or IDE project files, depending on the
   platform. (Note that cmake has a -G option which allows you to specify a
   target.) So you can type "make" or run the IDE to build the libraries and
   test programs.


TESTS

Tests programs are built in the tests directory, and are designed to run from
a terminal (a.k.a. console). Most of them are straightforward: simply run
TestUtilLib, for example and make sure it reports that all tests passed.
(If not, search the output for "FAILED" and investigate further.) 
A few tests are special, requiring some setup. Please read the "README.txt"
files in the test subdirectories for details.

