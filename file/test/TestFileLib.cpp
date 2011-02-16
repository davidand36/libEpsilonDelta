/*
  TestUtilLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta file library.
*/


#include "Exception.hpp"
#include "TestCheck.hpp"
#include "DataBuffer.hpp"
#include "File.hpp"
#include "NestedReader.hpp"
#include "FileName.hpp"
#include "DirUtil.hpp"
#include "ConfigFile.hpp"
#include <cstdio>
using namespace std;
using namespace EpsilonDelta;


int Main( int /*argc*/, char ** /*argv*/ );


//*****************************************************************************


int main( int argc, char ** argv )
{
    try
    {
        int res = Main( argc, argv );
        return res;
    }
    catch ( Exception & except )
    {
        cout << except.Description( ) << endl;
    }
    catch ( exception & except )
    {
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    catch ( ... )
    {
        Exception exc( "Unknown exception!" ); //!!!
        cout << exc.Description( ) << endl;
    }
    return  2;
}

//-----------------------------------------------------------------------------

int Main( int /*argc*/, char ** /*argv*/ )
{
    bool ok = true;

#ifdef DEBUG
    if ( ! DataBuffer::Test( ) )
        ok = false;
    if ( ! File::Test( ) )
        ok = false;
    if ( ! NestedReader::Test( ) )
        ok = false;
    if ( ! FileName::Test( ) )
        ok = false;
    if ( ! TestDirUtil( ) )
        ok = false;
    if ( ! ConfigFile::Test( ) )
        ok = false;
#endif //DEBUG

    if ( ok )
    {
        cout << endl << "All tests PASSED!" << endl;
        return  0;
    }
    else
    {
        cout << endl << "Some test(s) FAILED!" << endl;
        return  1;
    }
}


//*****************************************************************************
