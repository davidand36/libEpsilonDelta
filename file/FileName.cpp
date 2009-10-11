/*
  FileName.cpp
  Copyright (C) 2009 David M. Anderson

  FileName handling routines
*/


#include "FileName.hpp"
#include "Platform.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace FileName
{                                                          //namespace FileName

//-----------------------------------------------------------------------------


namespace
{                                                                   //namespace

#if defined(OS_UNIX) || defined(OS_WINDOWS)
char pathSeparator = '/';
char extensionSeparator = '.';
#endif

}                                                                   //namespace


//=============================================================================


void 
Split( const string & fileSpec,
       string * pPath, string * pName, string * pExtension )
{
    string path = "";
    string name = "";
    string extension = "";
    size_t pathEndPos = fileSpec.rfind( pathSeparator );
    if ( pathEndPos == string::npos )
        name = fileSpec;
    else
    {
        path = fileSpec.substr( 0, pathEndPos + 1 );
        name = fileSpec.substr( pathEndPos + 1 );
    }
    size_t extPos = name.find( extensionSeparator );
    if ( extPos != string::npos )
    {
        extension = name.substr( extPos + 1 );
        name = name.substr( 0, extPos );
    }
    if ( pPath )
        *pPath = path;
    if ( pName )
        *pName = name;
    if ( pExtension )
        *pExtension = extension;
}

//-----------------------------------------------------------------------------

string 
Make( const string & path, const string & name, const string & extension )
{
    string spec = path;
    if ( (spec.length() > 0) && (spec[ spec.length() - 1 ] != pathSeparator) )
        spec += pathSeparator;
    spec += name;
    if ( (extension.length() > 0) && (extension[ 0 ] != extensionSeparator) )
        spec += extensionSeparator;
    spec += extension;
    return spec;
}

//=============================================================================

#ifdef DEBUG

bool Test( )
{
    bool ok = true;
    cout << "Testing FileName" << endl;


    string spec = "dir1/dir2/name.ext1.ext2";
    string path;
    string name;
    string ext;
    cout << "Split( \"" << spec << "\", 0, 0, 0 ) [shouldn't crash]" << endl;
    FileName::Split( spec, 0, 0, 0 );
    cout << "Split( \"" << spec << "\", &path, &name, &ext )" << endl;
    FileName::Split( spec, &path, &name, &ext );
    TESTCHECK( path, string( "dir1/dir2/" ), &ok );
    TESTCHECK( name, string( "name" ), &ok );
    TESTCHECK( ext, string( "ext1.ext2" ), &ok );
    TESTCHECK( FileName::Make( path, name, ext ), spec, &ok );
    spec = "/name1";
    cout << "Split( \"" << spec << "\", &path, &name, &ext )" << endl;
    FileName::Split( spec, &path, &name, &ext );
    TESTCHECK( path, string( "/" ), &ok );
    TESTCHECK( name, string( "name1" ), &ok );
    TESTCHECK( ext, string( "" ), &ok );
    TESTCHECK( FileName::Make( path, name, ext ), spec, &ok );
    spec = "name2";
    cout << "Split( \"" << spec << "\", &path, &name, &ext )" << endl;
    FileName::Split( spec, &path, &name, &ext );
    TESTCHECK( path, string( "" ), &ok );
    TESTCHECK( name, string( "name2" ), &ok );
    TESTCHECK( ext, string( "" ), &ok );
    TESTCHECK( FileName::Make( path, name, ext ), spec, &ok );
    path = "path2";
    ext = ".ext2";
    TESTCHECK( FileName::Make( path, name, ext ), string( "path2/name2.ext2" ),
               &ok );
    
    if ( ok )
        cout << "FileName PASSED." << endl << endl;
    else
        cout << "FileName FAILED." << endl << endl;
    return ok;
}

#endif


//-----------------------------------------------------------------------------

}                                                          //namespace FileName


//*****************************************************************************

}                                                      //namespace EpsilonDelta
