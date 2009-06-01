#
# SConstruct
# Copyright (C) 2007 David M. Anderson
#
# Top-level SCons "makefile" for EpsilonDelta libraries.
#

opts = Options( 'scons_options.py', ARGUMENTS )
opts.Add( ListOption( 'libraries', 'List of libraries to build', 'none',
    [ 'util', 'math', 'time', 'file', 'cgi', 'sql',
        'astro', 'calendar', 
        'mmbase', 'graphics2d', 'input' ] ) )
opts.Add( ListOption( 'noTest', 'List of libraries not to test', 'none',
    [ 'util', 'math', 'time', 'file', 'cgi', 'sql',
        'astro', 'calendar', 
        'mmbase', 'graphics2d', 'input' ] ) )
opts.Add( BoolOption( 'release', 'True (1) to build for release.', 0 ) )
opts.Add( BoolOption( 'profile', 'True (1) to build for profiling.', 0 ) )
opts.Add( BoolOption( 'useSTLport', 'True (1) to use STLport', 0 ) )
opts.Add( PathOption( 'stlportIncludeDir', 'Path to STLport headers', '/' ) )
opts.Add( BoolOption( 'useBoost', 'True (1) to use Boost library', 1 ) )
opts.Add( PathOption( 'boostDir', 'Path to Boost files', '/' ) )
opts.Add( BoolOption( 'useLoki', 'True (1) to use Loki library', 0 ) )
opts.Add( PathOption( 'lokiDir', 'Path to Loki files', '/' ) )
opts.Add( BoolOption( 'useFastCGI', 'True (1) to use FastCGI', 0 ) );
opts.Add( PathOption( 'fastCGIIncludeDir', 'Path to FastCGI headers', '/' ) )
opts.Add( PathOption( 'fastCGILibDir', 'Path to FastCGI library', '/' ) )
opts.Add( BoolOption( 'useSQLite', 'True (1) to use SQLite', 0 ) )
opts.Add( BoolOption( 'buildSQLite', 'True (1) to build SQLite', 0 ) )
opts.Add( PathOption( 'sqliteIncludeDir', 'Path to SQLite headers', '/' ) )
opts.Add( PathOption( 'sqliteLibDir', 'Path to SQLite library', '/' ) )
opts.Add( PathOption( 'sqliteSrcDir', 'Path to SQLite source', '/' ) )
opts.Add( BoolOption( 'useMySQL', 'True (1) to use MySQL', 0 ) )
opts.Add( PathOption( 'mysqlIncludeDir', 'Path to MySQL headers', '/' ) )
opts.Add( PathOption( 'mysqlLibDir', 'Path to MySQL library', '/' ) )
opts.Add( BoolOption( 'useEmbeddedMySQL', 'True (1) to use Embedded MySQL Server', 0 ) )
opts.Add( BoolOption( 'usePostgreSQL', 'True (1) to use PostgreSQL', 0 ) )
opts.Add( PathOption( 'postgresqlIncludeDir', 'Path to PostgreSQL headers', '/' ) )
opts.Add( PathOption( 'postgresqlLibDir', 'Path to PostgreSQL library', '/' ) )
opts.Add( BoolOption( 'useSDL', 'True (1) to use SDL library', 0 ) )
opts.Add( PathOption( 'sdlIncludeDir', 'Path to SDL headers', '/' ) )
opts.Add( PathOption( 'sdlLibDir', 'Path to SDL libraries', '/' ) )
opts.Add( BoolOption( 'useOpenGL', 'True (1) to use OpenGL', 0 ) )
opts.Add( PathOption( 'openGLIncludeDir', 'Path to openGL headers', '/' ) )
opts.Add( BoolOption( 'useCWiiD', 'True (1) to use CWiiD Wiimote library', 0 ) )
opts.Add( PathOption( 'cwiidIncludeDir', 'Path to CWiiD headers', '/' ) )
opts.Add( PathOption( 'cwiidLibDir', 'Path to CWiiD libraries', '/' ) )
opts.Add( BoolOption( 'useWiiYourself', 'True (1) to use WiiYourself library', 0 ) )
opts.Add( PathOption( 'wiiYourselfIncludeDir', 'Path to WiiYourself headers', '/' ) )
opts.Add( PathOption( 'wiiYourselfLibDir', 'Path to WiiYourself libraries', '/' ) )
opts.Add( 'bitsPerPixel', 'Preferred color depth (8, 16, or 32)', 32 )

env = Environment( options = opts )
Export( 'env' )

libraries = env[ 'libraries' ]
noTest = env[ 'noTest' ]
platform = env[ 'PLATFORM' ]
compiler = env[ 'CC' ]
release = env[ 'release' ]
profile = env[ 'profile' ]
useSTLport = env[ 'useSTLport' ]
useBoost = env[ 'useBoost' ]
useLoki = env[ 'useLoki' ]
useFastCGI = env[ 'useFastCGI' ]
useSQLite = env[ 'useSQLite' ]
buildSQLite = env[ 'buildSQLite' ]
usePostgreSQL = env[ 'usePostgreSQL' ]
useMySQL = env[ 'useMySQL' ]
if useMySQL:
    useEmbeddedMySQL = env[ 'useEmbeddedMySQL' ]
useSDL = env[ 'useSDL' ]
useOpenGL = env[ 'useOpenGL' ]
useCWiiD = env[ 'useCWiiD' ]
useWiiYourself = env[ 'useWiiYourself' ]
bitsPerPixel = env[ 'bitsPerPixel' ]
Export( 'platform' )
Export( 'compiler' )
Export( 'release' )
Export( 'profile' )
Export( 'useSTLport' )
Export( 'useBoost' )
Export( 'useLoki' )
Export( 'useFastCGI' )
Export( 'useSQLite' )
Export( 'buildSQLite' )
Export( 'usePostgreSQL' )
Export( 'useMySQL' )
if useMySQL:
    Export( 'useEmbeddedMySQL' )
Export( 'useSDL' )
Export( 'useOpenGL' )
Export( 'useCWiiD' )
Export( 'useWiiYourself' )
Export( 'bitsPerPixel' )

if ( compiler == 'gcc' ):   # GNU C/C++
    cppLang = ''

    cppWarnings = ( '-Wall -W -Woverloaded-virtual'
        + ' -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings'
	    + ' -Wconversion -Wmissing-noreturn -Winline'
    	+ ' -Wno-sign-compare -Wno-long-long -Wno-missing-noreturn'
#        + ' -Wshadow'
#        + ' -Wunreachable-code'
#	     + ' -Wundef -Wold-style-cast'
#        + '  -Weffc++'
        )
    if not useCWiiD:
        cppWarnings += ' -pedantic'

    cppDefines = ''
    if ( release == 0 ):
        cppDefines = ( cppDefines + ' -DDEBUG' )
        cppOptimization = '-g'
    else:
        cppDefines = ( cppDefines + ' -DNDEBUG' )
        if ( profile == 0 ):
            cppOptimization = '-O3 -fomit-frame-pointer'
        else:
            cppOptimization = '-O3'
    if useBoost:
        cppDefines = ( cppDefines + ' -DUSE_BOOST' )
    if useLoki:
        cppDefines = ( cppDefines + ' -DUSE_LOKI' )
    if useFastCGI:
        cppDefines = ( cppDefines + ' -DUSE_FASTCGI' )
    if useSQLite:
        cppDefines = ( cppDefines + ' -DUSE_SQLITE' )
    if usePostgreSQL:
        cppDefines = ( cppDefines + ' -DUSE_POSTGRESQL' )
    if useMySQL:
        cppDefines = ( cppDefines + ' -DUSE_MYSQL' )
        if useEmbeddedMySQL:
            cppDefines = ( cppDefines + ' -DUSE_EMBEDDEDMYSQL' )
    if useSDL:
        cppDefines = ( cppDefines + ' -DUSE_SDL' )
    if useOpenGL:
        cppDefines = ( cppDefines + ' -DUSE_OPENGL' )
    if useCWiiD:
        cppDefines = ( cppDefines + ' -DSUPPORT_WIIMOTE' )
        cppDefines = ( cppDefines + ' -DUSE_CWIID' )
    if useWiiYourself:
        cppDefines = ( cppDefines + ' -DSUPPORT_WIIMOTE' )
        cppDefines = ( cppDefines + ' -DUSE_WIIYOURSELF' )
    cppDefines = ( cppDefines + (' -DBITSPERPIXEL=%d' % ( bitsPerPixel)) )

    cppProfile = ''
    if ( profile == 1 ):
        cppProfile = '-pg'

    cppCPU = ''

    linkFlags = ''
    if ( profile == 1 ):
        linkFlags = '-pg'

elif ( compiler == 'cl' ):  # Microsoft Visual C/C++
    cppLang = ( '/EHsc /GR /Zc:forScope'
#        + ' /Za'
        + ' /D_CRT_SECURE_NO_DEPRECATE'
        )

    cppWarnings = ( '/W4 /WL /wd4127 /nologo' )

    cppDefines = '/D_USE_MATH_DEFINES'
    if ( release == 0 ):
        cppDefines = ( cppDefines + ' /DDEBUG' )
        cppOptimization = '/Od /Zi /MTd'
    else:
        cppDefines = ( cppDefines + ' /DNDEBUG' )
        cppOptimization = '/Ox /MT'
    if useBoost:
        cppDefines = ( cppDefines + ' /DUSE_BOOST' )
    if useLoki:
        cppDefines = ( cppDefines + ' /DUSE_LOKI' )
    if useFastCGI:
        cppDefines = ( cppDefines + ' /DUSE_FASTCGI' )
    if useSQLite:
        cppDefines = ( cppDefines + ' /DUSE_SQLITE' )
    if usePostgreSQL:
        cppDefines = ( cppDefines + ' /DUSE_POSTGRESQL' )
    if useMySQL:
        cppDefines = ( cppDefines + ' /DUSE_MYSQL' )
        if useEmbeddedMySQL:
            cppDefines = ( cppDefines + ' /DUSE_EMBEDDEDMYSQL' )
    if useSDL:
        cppDefines = ( cppDefines + ' /DUSE_SDL' )
    if useOpenGL:
        cppDefines = ( cppDefines + ' /DUSE_OPENGL' )
    if useCWiiD:
        cppDefines = ( cppDefines + ' /DSUPPORT_WIIMOTE' )
        cppDefines = ( cppDefines + ' /DUSE_CWIID' )
    if useWiiYourself:
        cppDefines = ( cppDefines + ' /DSUPPORT_WIIMOTE' )
        cppDefines = ( cppDefines + ' /DUSE_WIIYOURSELF' )
    cppDefines = ( cppDefines + (' /DBITSPERPIXEL=%d' % ( bitsPerPixel)) )

    cppProfile = ''
    if ( profile == 1 ):
        cppProfile = ''

    cppCPU = ''

    linkFlags = '/SUBSYSTEM:CONSOLE /NODEFAULTLIB:msvcrt'
    if ( release == 0 ):
        linkFlags = ( linkFlags + ' /DEBUG' )

env.Replace( CPPFLAGS = cppDefines );
env.Replace( CXXFLAGS = ( cppLang + ' ' + cppWarnings + ' ' + cppOptimization + ' ' + cppProfile + ' ' + cppCPU ) )
env.Append( LINKFLAGS = linkFlags )


if useSTLport:
    stlportIncludeDir = env[ 'stlportIncludeDir' ]
    Export( 'stlportIncludeDir' )

if useBoost:
    boostDir = env[ 'boostDir' ]
    Export( 'boostDir' )
    boostIncludeDir = boostDir
    Export( 'boostIncludeDir' )

if useLoki:
    lokiDir = env[ 'lokiDir' ]
    Export( 'lokiDir' )
    lokiIncludeDir = '%s/include' % lokiDir
    Export( 'lokiIncludeDir' )
    lokiSrcDir = '%s/src' % lokiDir
    Export( 'lokiSrcDir' )

if useSQLite:
    sqliteIncludeDir = env[ 'sqliteIncludeDir' ]
    Export( 'sqliteIncludeDir' )
    sqliteLibDir = env[ 'sqliteLibDir' ]
    Export( 'sqliteLibDir' )

if useFastCGI:
    fastCGIIncludeDir = env[ 'fastCGIIncludeDir' ]
    Export( 'fastCGIIncludeDir' )
    fastCGILibDir = env[ 'fastCGILibDir' ]
    Export( 'fastCGILibDir' )

if buildSQLite:
    sqliteSrcDir = env[ 'sqliteSrcDir' ]
    Export( 'sqliteSrcDir' )

if useMySQL:
    mysqlIncludeDir = env[ 'mysqlIncludeDir' ]
    Export( 'mysqlIncludeDir' )
    mysqlLibDir = env[ 'mysqlLibDir' ]
    Export( 'mysqlLibDir' )

if usePostgreSQL:
    postgresqlIncludeDir = env[ 'postgresqlIncludeDir' ]
    Export( 'postgresqlIncludeDir' )
    postgresqlLibDir = env[ 'postgresqlLibDir' ]
    Export( 'postgresqlLibDir' )

if useSDL:
    sdlIncludeDir = env[ 'sdlIncludeDir' ]
    Export( 'sdlIncludeDir' )
    sdlLibDir = env[ 'sdlLibDir' ]
    Export( 'sdlLibDir' )

if useOpenGL:
    openGLIncludeDir = env[ 'openGLIncludeDir' ]
    Export( 'openGLIncludeDir' )

if useCWiiD:
    cwiidIncludeDir = env[ 'cwiidIncludeDir' ]
    Export( 'cwiidIncludeDir' )
    cwiidLibDir = env[ 'cwiidLibDir' ]
    Export( 'cwiidLibDir' )

if useWiiYourself:
    wiiYourselfIncludeDir = env[ 'wiiYourselfIncludeDir' ]
    Export( 'wiiYourselfIncludeDir' )
    wiiYourselfLibDir = env[ 'wiiYourselfLibDir' ]
    Export( 'wiiYourselfLibDir' )

if useLoki:
    libraries.append( 'Loki' )
if buildSQLite:
    libraries.append( 'SQLite' )
    sqliteLibDir = '#'

libs = []
for dir in libraries:
    script = '%s/SConscript' % dir
    buildDir = 'build/%s' % dir
    lib = SConscript( script, build_dir = buildDir, duplicate = 0 )
    libs = libs + lib

env.Install( '.', libs )

if useLoki:
    noTest.append( 'Loki' )
if buildSQLite:
    noTest.append( 'SQLite' )
testdirs = []
for dir in libraries:
    if not (dir in noTest):
        testdirs = testdirs + [ '%s/test' % dir ]

allTests = []
for dir in testdirs:
    script = '%s/SConscript' % dir
    buildDir = 'build/%s' % dir
    tests = SConscript( script, build_dir = buildDir, duplicate = 0 )
    allTests.append( tests )

env.Install( 'tests', allTests )

Help( opts.GenerateHelpText( env ) )


# Lists all of the construction variables
#dict = env.Dictionary()
#keys = dict.keys()
#keys.sort()
#for key in keys:
#    if ( key[0] != '_' ):    
#        print "'%s' = '%s'" % (key, dict[key])
