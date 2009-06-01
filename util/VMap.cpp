/*
  VMap.cpp
  Copyright (C) 2007 David M. Anderson

  VMap template container class: like std::map, but implemented using a
  sorted std::vector.
*/


#include "VMap.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <string>
#include <iostream>
#include <list>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestVMap( )
{
    bool ok = true;
    cout << "Testing VMap" << endl;

    cout << "VMap< string, int, greater< string > >() [default constructor]" << endl;
    typedef VMap< string, int, greater< string > >  VMapSIG;
    typedef VMapSIG::iterator  VMapSIGIter;
    typedef VMapSIG::const_reverse_iterator  VMapSIGcrIter;
    VMapSIG vmap;
    TESTCHECK( vmap.size(), 0, &ok );
    cout << "vmap[\"one\"]=1" << endl;
    vmap["one"] = 1;
    TESTCHECK( vmap.size(), 1, &ok );
    VMapSIGIter pSI = vmap.begin();
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    TESTCHECK( (++pSI == vmap.end()), true, &ok );
    cout << "x = vmap[\"two\"]" << endl;
    int x = vmap["two"];
    TESTCHECK( x, 0, &ok );
    TESTCHECK( vmap.size(), 2, &ok );
    pSI = vmap.begin();
    TESTCHECK( pSI->first, string( "two" ), &ok );
    TESTCHECK( pSI->second, 0, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    TESTCHECK( (++pSI == vmap.end()), true, &ok );
    cout << "vmap[\"two\"]=2" << endl;
    vmap["two"] = 2;
    TESTCHECK( vmap.size(), 2, &ok );
    TESTCHECK( vmap["one"], 1, &ok );
    TESTCHECK( vmap["two"], 2, &ok );
    pSI = vmap.begin();
    TESTCHECK( pSI->first, string( "two" ), &ok );
    TESTCHECK( pSI->second, 2, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    TESTCHECK( (++pSI == vmap.end()), true, &ok );
    cout << "vmap[\"three\"]=3" << endl;
    vmap["three"] = 3;
    TESTCHECK( vmap.size(), 3, &ok );
    TESTCHECK( vmap["one"], 1, &ok );
    TESTCHECK( vmap["two"], 2, &ok );
    TESTCHECK( vmap["three"], 3, &ok );
    pSI = vmap.begin();
    TESTCHECK( pSI->first, string( "two" ), &ok );
    TESTCHECK( pSI->second, 2, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "three" ), &ok );
    TESTCHECK( pSI->second, 3, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    TESTCHECK( (++pSI == vmap.end()), true, &ok );
    VMapSIGcrIter crpSI = vmap.rbegin();
    TESTCHECK( crpSI->first, string( "one" ), &ok );
    TESTCHECK( crpSI->second, 1, &ok );
    ++crpSI;
    TESTCHECK( crpSI->first, string( "three" ), &ok );
    TESTCHECK( crpSI->second, 3, &ok );
    ++crpSI;
    TESTCHECK( crpSI->first, string( "two" ), &ok );
    TESTCHECK( crpSI->second, 2, &ok );
    TESTCHECK( (++crpSI == static_cast< VMapSIGcrIter >( vmap.rend() )),
               true, &ok );
    cout << "find(\"three\")" << endl;
    pSI = vmap.find( "three" );
    TESTCHECK( pSI->first, string( "three" ), &ok );
    TESTCHECK( pSI->second, 3, &ok );
    cout << "find(\"two\")" << endl;
    pSI = vmap.find( "two" );
    TESTCHECK( pSI->first, string( "two" ), &ok );
    TESTCHECK( pSI->second, 2, &ok );
    cout << "find(\"one\")" << endl;
    pSI = vmap.find( "one" );
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    cout << "find(\"eight\")" << endl;
    pSI = vmap.find( "eight" );
    TESTCHECK( (pSI == vmap.end()), true, &ok );
    TESTCHECK( vmap.count( "two" ), 1, &ok );
    TESTCHECK( vmap.count( "ten" ), 0, &ok );
    cout << "lower_bound(\"one\")" << endl;
    pSI = vmap.lower_bound( "one" );
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    cout << "lower_bound(\"seven\")" << endl;
    pSI = vmap.lower_bound( "seven" );
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    cout << "upper_bound(\"one\")" << endl;
    pSI = vmap.upper_bound( "one" );
    TESTCHECK( (pSI == vmap.end()), true, &ok );
    cout << "upper_bound(\"seven\")" << endl;
    pSI = vmap.upper_bound( "seven" );
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    cout << "upper_bound(\"two\")" << endl;
    pSI = vmap.upper_bound( "two" );
    TESTCHECK( pSI->first, string( "three" ), &ok );
    TESTCHECK( pSI->second, 3, &ok );
    cout << "equal_range(\"three\")" << endl;
    std::pair< VMapSIGIter, VMapSIGIter > iterPair
            = vmap.equal_range( "three" );
    TESTCHECK( iterPair.first->first, string( "three" ), &ok );
    TESTCHECK( iterPair.first->second, 3, &ok );
    TESTCHECK( iterPair.second->first, string( "one" ), &ok );
    TESTCHECK( iterPair.second->second, 1, &ok );
    cout << "equal_range(\"one\")" << endl;
    iterPair = vmap.equal_range( "one" );
    TESTCHECK( iterPair.first->first, string( "one" ), &ok );
    TESTCHECK( iterPair.first->second, 1, &ok );
    TESTCHECK( (iterPair.second == vmap.end()), true, &ok );
    cout << "equal_range(\"six\")" << endl;
    iterPair = vmap.equal_range( "six" );
    TESTCHECK( iterPair.first->first, string( "one" ), &ok );
    TESTCHECK( iterPair.first->second, 1, &ok );
    TESTCHECK( iterPair.second->first, string( "one" ), &ok );
    TESTCHECK( iterPair.second->second, 1, &ok );
    cout << "pair<\"one\", 1>" << endl;
    VMapSIG::value_type siPair = std::make_pair( "one", 1 );
    cout << "insert( siPair )" << endl;
    std::pair< VMapSIGIter, bool > ibPair = vmap.insert( siPair );
    TESTCHECK( ibPair.first->first, string( "one" ), &ok );
    TESTCHECK( ibPair.first->second, 1, &ok );
    TESTCHECK( ibPair.second, false, &ok );
    cout << "pair<\"two\", 2>" << endl;
    siPair = std::make_pair( "two", 2 );
    cout << "insert( siPair )" << endl;
    ibPair = vmap.insert( siPair );
    TESTCHECK( ibPair.first->first, string( "two" ), &ok );
    TESTCHECK( ibPair.first->second, 2, &ok );
    TESTCHECK( ibPair.second, false, &ok );
    cout << "pair<\"three\", 3>" << endl;
    siPair = std::make_pair( "three", 3 );
    cout << "insert( siPair )" << endl;
    ibPair = vmap.insert( siPair );
    TESTCHECK( ibPair.first->first, string( "three" ), &ok );
    TESTCHECK( ibPair.first->second, 3, &ok );
    TESTCHECK( ibPair.second, false, &ok );
    cout << "pair<\"six\", 6>" << endl;
    siPair = std::make_pair( "six", 6 );
    cout << "insert( siPair )" << endl;
    ibPair = vmap.insert( siPair );
    TESTCHECK( ibPair.first->first, string( "six" ), &ok );
    TESTCHECK( ibPair.first->second, 6, &ok );
    TESTCHECK( ibPair.second, true, &ok );
    x = vmap[ "six" ];
    TESTCHECK( x, 6, &ok );
    cout << "list< VMapSIG::value_type >" << endl;
    std::list< VMapSIG::value_type > lst;
    siPair = std::make_pair( "four", 4 );
    lst.push_back( siPair );
    siPair = std::make_pair( "five", 5 );
    lst.push_back( siPair );
    siPair = std::make_pair( "six", 6 );
    lst.push_back( siPair );
    cout << "insert( lst.begin(), lst.end() )" << endl;
    vmap.insert( lst.begin(), lst.end() );
    pSI = vmap.begin();
    TESTCHECK( pSI->first, string( "two" ), &ok );
    TESTCHECK( pSI->second, 2, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "three" ), &ok );
    TESTCHECK( pSI->second, 3, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "six" ), &ok );
    TESTCHECK( pSI->second, 6, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "four" ), &ok );
    TESTCHECK( pSI->second, 4, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "five" ), &ok );
    TESTCHECK( pSI->second, 5, &ok );
    TESTCHECK( (++pSI == vmap.end()), true, &ok );
    cout << "find(\"six\")" << endl;
    pSI = vmap.find( "six" );
    cout << "erase(pSI)" << endl;
    vmap.erase( pSI );
    TESTCHECK( (vmap.find( "six" ) == vmap.end()), true, &ok );
    cout << "find(\"four\")" << endl;
    pSI = vmap.find( "four" );
    cout << "erase(pSI,end())" << endl;
    vmap.erase( pSI, vmap.end() );
    TESTCHECK( vmap.size(), 3, &ok );
    pSI = vmap.begin();
    TESTCHECK( pSI->first, string( "two" ), &ok );
    TESTCHECK( pSI->second, 2, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "three" ), &ok );
    TESTCHECK( pSI->second, 3, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    TESTCHECK( (++pSI == vmap.end()), true, &ok );
    cout << "erase(\"three\")" << endl;
    vmap.erase( "three" );
    TESTCHECK( vmap.size(), 2, &ok );
    pSI = vmap.begin();
    TESTCHECK( pSI->first, string( "two" ), &ok );
    TESTCHECK( pSI->second, 2, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    TESTCHECK( (++pSI == vmap.end()), true, &ok );
    cout << "VMap<...>( vmap ) [copy constructor]" << endl;
    VMapSIG vmap2 = vmap;
    TESTCHECK( vmap2.size(), 2, &ok );
    pSI = vmap2.begin();
    TESTCHECK( pSI->first, string( "two" ), &ok );
    TESTCHECK( pSI->second, 2, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "one" ), &ok );
    TESTCHECK( pSI->second, 1, &ok );
    TESTCHECK( (++pSI == vmap2.end()), true, &ok );
    cout << "VMap<...>(lst.begin(),lst.end()) [\"insert\" constructor]" << endl;
    VMapSIG vmap3( lst.begin(), lst.end() );
    pSI = vmap3.begin();
    TESTCHECK( pSI->first, string( "six" ), &ok );
    TESTCHECK( pSI->second, 6, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "four" ), &ok );
    TESTCHECK( pSI->second, 4, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "five" ), &ok );
    TESTCHECK( pSI->second, 5, &ok );
    TESTCHECK( (++pSI == vmap3.end()), true, &ok );
    cout << "operator=" << endl;
    vmap2 = vmap3;
    pSI = vmap2.begin();
    TESTCHECK( pSI->first, string( "six" ), &ok );
    TESTCHECK( pSI->second, 6, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "four" ), &ok );
    TESTCHECK( pSI->second, 4, &ok );
    ++pSI;
    TESTCHECK( pSI->first, string( "five" ), &ok );
    TESTCHECK( pSI->second, 5, &ok );
    TESTCHECK( (++pSI == vmap2.end()), true, &ok );
    cout << "clear()" << endl;
    vmap3.clear( );
    TESTCHECK( vmap3.size(), 0, &ok );

    if ( ok )
        cout << "VMap PASSED." << endl << endl;
    else
        cout << "VMap FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta
