/*
  DataBuffer.cpp
  Copyright (C) 2009 David M. Anderson

  DataBuffer class: Holds a buffer of data, primarily for purposes such as
  file I/O.
*/


#include "DataBuffer.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

namespace
{                                                                   //namespace

struct Foo
{
    int     i;
    float   f;
};

}                                                                   //namespace

//-----------------------------------------------------------------------------

bool 
DataBuffer::Test( )
{
    bool ok = true;
    cout << "Testing DataBuffer" << endl;

    DataBuffer buff;
    TESTCHECK( buff.Data() == 0, true, &ok );
    TESTCHECK( buff.Size(), 0, &ok );
    char sIn[ 10 ] = "123456789";
    Foo fIn = { 127, 2.5f };
    cout << "Add( char *, 10 )" << endl;
    buff.Add( sIn, 10 );
    TESTCHECK( *buff.Data(), '1', &ok );
    TESTCHECK( buff.Size(), 10, &ok );
    cout << "Add( Foo )" << endl;
    buff.Add( fIn );
    cout << "Read( 10 )" << endl;
    const char * sOut = buff.Read( 10 );
    TESTCHECK( (string( sIn ) == string( sOut )), true, &ok );
    cout << "Read< Foo >()" << endl;
    const Foo * pFoo = buff.Read< Foo >( );
    TESTCHECK( pFoo->i, 127, &ok );
    TESTCHECK( pFoo->f, 2.5f, &ok );
    
    if ( ok )
        cout << "DataBuffer PASSED." << endl << endl;
    else
        cout << "DataBuffer FAILED." << endl << endl;
    return ok;
}

#endif

//*****************************************************************************

}                                                      //namespace EpsilonDelta
