/*
  SmartPtr.cpp
  Copyright (C) 2007 David M. Anderson

  SmartPtr template class: reference-counted pointer-like object.
*/


#include "SmartPtr.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <list>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

#ifdef DEBUG

//=============================================================================

class Foo
{
public:
    Foo( int number );
    ~Foo( );

    static int ms_count;

private:
    int  m_number;
};

//.............................................................................

int Foo::ms_count = 0;

//.............................................................................

Foo::Foo( int number )
    :   m_number( number )
{
    cout << "Foo constructor (" << m_number << ")" << endl;
    ++ms_count;
}

//.............................................................................

Foo::~Foo( )
{
    cout << "Foo destructor (" << m_number << ")" << endl;
    --ms_count;
}

//-----------------------------------------------------------------------------

bool 
TestSmartPtr( )
{
    bool ok = true;
    cout << "Testing SmartPtr" << endl;

    for ( int i = 0; i < 2; ++i )
    {
        typedef shared_ptr< Foo >  FooPtr;
        typedef list< FooPtr >  FooList;
        typedef shared_ptr< FooList > FooListPtr;
        cout << "FooListPtr( new FooList )" << endl;
        FooListPtr spList( new FooList );
        //Can't do this:
//        cout << "push_back( new Foo( 1 ) )" << endl;
//        spList->push_back( new Foo( 1 ) );
        //Do this instead:
        cout << "push_back( FooPtr( new Foo( 1 ) ) )" << endl;
        spList->push_back( FooPtr( new Foo( 1 ) ) );
        //Not good style, just for testing:
        Foo * pFoo2 = new Foo( 2 );
        //Again, can't do this:
//        cout << "push_back( pFoo2 )" << endl;
//        spList->push_back( pFoo2 );
        //Do this instead:
        cout << "push_back( FooPtr( pFoo2 ) )" << endl;
        spList->push_back( FooPtr( pFoo2 ) );
        FooPtr spFoo3( new Foo( 3 ) );
        cout << "push_back( spFoo3 )" << endl;
        spList->push_back( spFoo3 );
        //Again, poor style:
        Foo * pFoo4 = new Foo( 4 ); 
        FooPtr spFoo4( pFoo4 );
        cout << "push_back( spFoo4 )" << endl;
        spList->push_back( spFoo4 );
        //Again, can't do this:
//        cout << "push_back( new Foo( 5 ) )" << endl;
//        spList->push_back( new Foo( 5 ) );
        //Do this instead:
        cout << "push_back( FooPtr( new Foo( 5 ) ) )" << endl;
        spList->push_back( FooPtr( new Foo( 5 ) ) );
        cout << "remove( spFoo3 )" << endl;
        spList->remove( spFoo3 );
        //The following results in a crash.
//        spList->remove( pFoo2 );
        //So does this.
//        FooPtr spFoo2( pFoo2 );
        //In each case a new smart pointer referencing the same dumb pointer is
        // created, but it doesn't share the reference counter with the
        // smart pointer in the list.
        spList->remove( spFoo4 );
        cout << "End of block" << endl;
    }
    TESTCHECK( Foo::ms_count, 0, &ok );
    
    if ( ok )
        cout << "SmartPtr PASSED." << endl << endl;
    else
        cout << "SmartPtr FAILED." << endl << endl;
    return ok;
}

//=============================================================================

#endif //DEBUG

//*****************************************************************************

}                                                      //namespace EpsilonDelta
