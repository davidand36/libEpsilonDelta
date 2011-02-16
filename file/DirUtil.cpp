/*
  DirUtil.cpp
  Copyright (C) 2007 David M. Anderson

  Some useful utility functions for directories.
*/


#include "DirUtil.hpp"
#include "Platform.hpp"
#include "FileException.hpp"
#include "StringUtil.hpp"
#include <vector>
#include <errno.h>
#if defined(OS_UNIX) || defined(OS_ANDROID)
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#elif defined(OS_WINDOWS)
#include <direct.h>
#include <sys/stat.h>
#include <ctime>
#include <windows.h>
#endif
#ifdef DEBUG
#include "TestCheck.hpp"
#include "File.hpp"
#include "FileWriter.hpp"
#include <iostream>
#include <algorithm>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void 
MakeDirectory( const std::string & dirName, int mode )
{
#if defined(OS_UNIX) || defined(OS_ANDROID)
    string dir = Replace( dirName, "\\", "/" );
    bool absolute = (dir[0] == '/');
    vector< string > dirs = Split( dir, '/' );
    dir = "";
    if ( absolute )
        dir += '/';
    for ( vector<string>::iterator p = dirs.begin(); p != dirs.end(); ++p )
    {
        if ( p->empty() )
            continue;
        dir += *p;
        int mkRslt = mkdir( dir.c_str(), mode );
        if ( mkRslt != 0 )
        {
            if ( errno != EEXIST )
                throw MkdirException( dir );
        }
        dir += "/";
    }

#elif defined(OS_WINDOWS)
    string dir = Replace( dirName, "\\", "/" );
    char drive = 0;
    if ( dir[1] == ':' )
    {
        drive = dir[0];
        dir = dir.substr( 2 );
    }
    bool absolute = (dir[0] == '/');
    vector< string > dirs = Split( dir, '/' );
    dir = "";
    if ( drive )
    {
        dir += drive;
        dir += ':';
    }
    if ( absolute )
        dir += '/';
    for ( vector<string>::iterator p = dirs.begin(); p != dirs.end(); ++p )
    {
        if ( p->empty() )
            continue;
        dir += *p;
        int mkRslt = _mkdir( dir.c_str() );
        if ( mkRslt != 0 )
        {
            if ( errno != EEXIST )
                throw MkdirException( dir );
        }
        dir += "/";
    }

#endif
}

//-----------------------------------------------------------------------------

void 
RemoveDirectory( const std::string & dirName )
{
#if defined(OS_UNIX) || defined(OS_ANDROID)
    int rmRslt = rmdir( dirName.c_str() );
#elif defined(OS_WINDOWS)
    int rmRslt = _rmdir( dirName.c_str() );
#endif
    if ( rmRslt != 0 )
    {
        throw RmdirException( dirName );
    }
}

//=============================================================================

vector< string > 
ListFiles( const string & dirName )
{
    vector< string > files;

#if defined(OS_UNIX) || defined(OS_ANDROID)
    ::DIR * pDirStrm = ::opendir( dirName.c_str() );
    if ( pDirStrm == 0 )
        throw ReadDirException( dirName );
    ::dirent * pDirEnt;
    while ( (pDirEnt = ::readdir( pDirStrm )) != 0 )
    {
        string entryName( pDirEnt->d_name );
        if ( (entryName == ".") || (entryName == "..") )
            continue;
        string entrySpec = dirName + "/" + entryName;
        struct ::stat entryStatus;
        int statRslt = ::stat( entrySpec.c_str(), &entryStatus );
        if ( statRslt != 0 )
            throw FileStatusException( entrySpec );
        if ( S_ISREG( entryStatus.st_mode ) )
            files.push_back( entryName );
    }
    ::closedir( pDirStrm );
    
#elif defined(OS_WINDOWS)
    string searchString = dirName + "/*";
    WIN32_FIND_DATA entryData;
    HANDLE hFind = ::FindFirstFile( searchString.c_str(), &entryData );
    if ( hFind == INVALID_HANDLE_VALUE )
        return files;
    do
    {
        string entryName( entryData.cFileName );
        if ( (entryName == ".") || (entryName == "..") )
            continue;
        if ( (entryData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
            files.push_back( entryName );
    } while ( ::FindNextFile( hFind, &entryData ) );
    ::FindClose( hFind );

#endif

    return files;
}

//-----------------------------------------------------------------------------

vector< string > 
ListSubdirectories( const string & dirName )
{
    vector< string > subdirs;

#if defined(OS_UNIX) || defined(OS_ANDROID)
    ::DIR * pDirStrm = opendir( dirName.c_str() );
    if ( pDirStrm == 0 )
        throw ReadDirException( dirName );
    ::dirent * pDirEnt;
    while ( (pDirEnt = ::readdir( pDirStrm )) != 0 )
    {
        string entryName( pDirEnt->d_name );
        if ( (entryName == ".") || (entryName == "..") )
            continue;
        string entrySpec = dirName + "/" + entryName;
        struct ::stat entryStatus;
        int statRslt = ::stat( entrySpec.c_str(), &entryStatus );
        if ( statRslt != 0 )
            throw FileStatusException( entrySpec );
        if ( S_ISDIR( entryStatus.st_mode ) )
            subdirs.push_back( entryName );
    }
    ::closedir( pDirStrm );
    
#elif defined(OS_WINDOWS)
    string searchString = dirName + "/*";
    WIN32_FIND_DATA entryData;
    HANDLE hFind = ::FindFirstFile( searchString.c_str(), &entryData );
    if ( hFind == INVALID_HANDLE_VALUE )
        return subdirs;
    do
    {
        string entryName( entryData.cFileName );
        if ( (entryName == ".") || (entryName == "..") )
            continue;
        if ( (entryData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
            subdirs.push_back( entryName );
    } while ( ::FindNextFile( hFind, &entryData ) );
    ::FindClose( hFind );

#endif

    return subdirs;
}

//=============================================================================

bool
FileExists( const string & fileName )
{
#if defined(OS_UNIX) || defined(OS_ANDROID) || defined(OS_WINDOWS)
    FILE * file = fopen( fileName.c_str(), "r" );
    if ( file != 0 )
    {
        fclose( file );
        return true;
    }
    return false;
#endif
}

//-----------------------------------------------------------------------------

int
FileSize( const string & fileName )
{
#if defined(OS_UNIX) || defined(OS_ANDROID) || defined(OS_WINDOWS)
    struct ::stat fileStatus;
    int statRslt = ::stat( fileName.c_str(), &fileStatus );
    if ( statRslt != 0 )
        throw FileStatusException( fileName );
    return static_cast< int >( fileStatus.st_size );
#endif
}

//-----------------------------------------------------------------------------

DateTime
FileModDate( const string & fileName )
{
#if defined(OS_UNIX) || defined(OS_ANDROID) || defined(OS_WINDOWS)
    struct ::stat fileStatus;
    int statRslt = ::stat( fileName.c_str(), &fileStatus );
    if ( statRslt != 0 )
        throw FileStatusException( fileName );
    std::time_t modDate = fileStatus.st_mtime;
    return DateTime( modDate );
#endif
}

//=============================================================================

void
DeleteFile( const string & fileName )
{
    if ( ! FileExists( fileName ) )
        return;
#if defined(OS_UNIX) || defined(OS_ANDROID) || defined(OS_WINDOWS)
    int removeRslt = remove( fileName.c_str() );
    if (removeRslt != 0)
        throw FileDeleteException( fileName );
#endif
}

//=============================================================================

#ifdef DEBUG

bool 
TestDirUtil( )
{
    bool ok = true;
    cout << "Testing DirUtil" << endl;

    try
    {
#if defined( OS_UNIX ) || defined(OS_ANDROID) //???
        string baseDirs[2] = { "", "/tmp/" };
#elif defined( OS_WINDOWS )
        string baseDirs[2] = { "", "C:/Windows/Temp/" };
#endif
        for ( int i = 0; i < 2; ++i )
        {
            string newDir = baseDirs[i] + "DirUtilTest1/DirUtilTest2/";
            cout << "MakeDirectory( " << newDir << " )" << endl;
            MakeDirectory( newDir );
            string fileSpec = newDir + "Mine";
            vector< char > chars( 1000 );
            DataBuffer buff;
            buff.Add( &chars[0], 1000 );
            {
                FileWriter writer( fileSpec );
                writer.Save( buff );
            }
            fileSpec = newDir + "Dave";
            buff.Clear();
            buff.Add( &chars[0], 100 );
            {
                FileWriter writer( fileSpec );
                writer.Save( buff );
            }
            string dir = baseDirs[i] + "DirUtilTest1";
            cout << "ListFiles( " << dir << " )" << endl;
            vector< string > files = ListFiles( dir );
            TESTCHECK( files.size(), 0, &ok );
            cout << "ListDirs( " << dir << " )" << endl;
            vector< string > dirs = ListSubdirectories( dir );
            TESTCHECK( dirs.size(), 1, &ok );
            TESTCHECK( dirs[0], string( "DirUtilTest2" ), &ok );
            dir = newDir;
            cout << "ListFiles( " << dir << " )" << endl;
            files = ListFiles( dir );
            TESTCHECK( files.size(), 2, &ok );
            sort( files.begin(), files.end() );
            TESTCHECK( files[0], string( "Dave" ), &ok );
            TESTCHECK( files[1], string( "Mine" ), &ok );
            cout << "ListDirs( " << dir << " )" << endl;
            dirs = ListSubdirectories( dir );
            TESTCHECK( dirs.size(), 0, &ok );
            fileSpec = newDir + "Dave";
            DeleteFile( fileSpec );
            cout << "ListFiles( " << dir << " )" << endl;
            files = ListFiles( dir );
            TESTCHECK( files.size(), 1, &ok );
            TESTCHECK( files[0], string( "Mine" ), &ok );
            try
            {
                cout << "RemoveDirectory( " << dir << " )" << endl;
                RemoveDirectory( dir );
                cout << "Remove() should have thrown an exception." << endl;
                ok = false;
            }
            catch ( FileException & except )
            {
                cout << "Excepttion here is OK" << endl;
                cout << except.Description( ) << endl;
            }
            fileSpec = newDir + "Mine";
            DeleteFile( fileSpec );
            cout << "ListFiles( " << dir << " )" << endl;
            files = ListFiles( dir );
            TESTCHECK( files.size(), 0, &ok );
            dir = baseDirs[i] + "DirUtilTest1";
            try
            {
                cout << "RemoveDirectory( " << dir << " )" << endl;
                RemoveDirectory( dir );
                cout << "Remove() should have thrown an exception." << endl;
                ok = false;
            }
            catch ( FileException & except )
            {
                cout << "Exception here is OK" << endl;
                cout << except.Description( ) << endl;
            }
            dir = newDir;
            cout << "RemoveDirectory( " << dir << " )" << endl;
            RemoveDirectory( dir );
            dir = baseDirs[i] + "DirUtilTest1";
            cout << "RemoveDirectory( " << dir << " )" << endl;
            RemoveDirectory( dir );
        }
    }
    catch ( Exception & except )
    {
        cout << except.Description() << endl;
        ok = false;
    }

    if ( ok )
        cout << "DirUtil PASSED." << endl << endl;
    else
        cout << "DirUtil FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta
