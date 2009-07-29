/*
  IndexedVector.cpp
  Copyright (C) 2007 David M. Anderson

  IndexedVector template class: a utility class for accessing elements of a
  vector (or other random-access container, such as an array) through a
  separate vector of indices.
*/


#include "IndexedVector.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Array.hpp"
#include <string>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestIndexedVector( )
{
    bool ok = true;
    cout << "Testing IndexedVector" << endl;

    string names[] = { "five", "four", "three", "two", "one", "zero" };
    string types[] = { "even", "odd" };
    int nameIndices[] = { 5, 4, 3, 2 };
    array< unsigned int, 6 > typeIndices = { { 0, 1, 0, 1, 0, 1 } };
    vector< string > namesVec( names, names + ARRAY_LENGTH( names ) );
    vector< int > nameIndicesVec( nameIndices,
                                  nameIndices + ARRAY_LENGTH( nameIndices ) );
    cout << "IndexedVector< string >( namesVec, nameIndicesVec )" << endl;
    IndexedVector< string > namesIV( namesVec, nameIndicesVec );
    TESTCHECK( namesIV.size(), 4, &ok );
    TESTCHECK( namesIV[ 0 ], string( "zero" ), &ok );
    TESTCHECK( namesIV[ 1 ], string( "one" ), &ok );
    TESTCHECK( namesIV[ 2 ], string( "two" ), &ok );
    TESTCHECK( namesIV[ 3 ], string( "three" ), &ok );
    cout << "begin()" << endl;
    IndexedVector< string >::iterator pName = namesIV.begin();
    TESTCHECK( *pName++, string( "zero" ), &ok );
    TESTCHECK( *++pName, string( "two" ), &ok );
    TESTCHECK( *pName++, string( "two" ), &ok );
    TESTCHECK( *pName++, string( "three" ), &ok );
    TESTCHECK( (pName == namesIV.end()), true, &ok );
    TESTCHECK( *--pName, string( "three" ), &ok );
    TESTCHECK( *pName--, string( "three" ), &ok );
    cout << "*pName = \"zwei\"" << endl;
    *pName = "zwei";
    TESTCHECK( *pName, string( "zwei" ), &ok );
    TESTCHECK( pName->size(), 4, &ok );
    TESTCHECK( namesIV[ 2 ], namesVec[ 3 ], &ok );
    typedef IndexedVector< string, unsigned int,
            string[2], array< unsigned int, 6 > >  TypesIVType;
    cout << "const IndexedVector< string, unsigned int, string[], array< unsigned int, 6 > > typesIV( types, typeIndices )" << endl;
    const TypesIVType typesIV( types, typeIndices );
    TESTCHECK( typesIV.size(), 6, &ok );
    int i = 0;
    for ( TypesIVType::const_iterator pType = typesIV.begin();
          pType != typesIV.end(); ++pType, ++i )
    {
        TESTCHECK( *pType, string( (i & 1) ? "odd" : "even" ), &ok );
        TESTCHECK( pType->size(), ( (i & 1) ? 3 : 4 ), &ok );
    }

    if ( ok )
        cout << "IndexedVector PASSED." << endl << endl;
    else
        cout << "IndexedVector FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta
