/*
  Permutation.cpp
  Copyright (C) 2007 David M. Anderson

  Permutation class: Represents a permutation, i.e., a one-to-one mapping of a
  finite set onto itself.
*/


#include "Permutation.hpp"
#include "Factorial.hpp"
#include "Random.hpp"
#include "JSON.hpp"
#include <exception>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <sstream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Permutation::Permutation( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Permutation::Permutation( int degree, bool random )
{
    Set( degree, random );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Permutation::Permutation( int degree, uint64_t rank )
{
    Set( degree, rank );
}

//.............................................................................

Permutation::Permutation( int degree, int rank )
{
    Set( degree, static_cast< uint64_t >( rank ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Permutation::Permutation( const vector< int > & perm )
    :   m_perm( perm )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Permutation::Permutation( int degree, const Cycle & cycle )
{
    Set( degree, cycle );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Permutation::Permutation( int degree, const Transposition & trans )
{
    Set( degree, trans );
}

//-----------------------------------------------------------------------------

void
Permutation::Set( int degree, bool random )
{
    m_perm.resize( degree );
    for ( int i = 0; i < degree; ++i )
        m_perm[i] = i;
    if ( random )
        random_shuffle( m_perm.begin(), m_perm.end(), Random );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Permutation::Set( int degree, uint64_t rank )
{
    //This operation is sometimes called "unrank".
    m_perm.resize( degree );
    uint64_t fact = 1;
    for ( int i = 0; i < degree; ++i )
    {
        if ( i > 1 )
            fact *= i;
        uint64_t x = rank % (fact * (i + 1));
        rank -= x;
        int v = static_cast< int >( x / fact );
        m_perm[ degree - 1 - i ] = v;
        for ( int j = (degree - i); j < degree; ++j )
            if ( m_perm[j] >= v )
                ++m_perm[j];
    }
}

//.............................................................................

void
Permutation::Set( int degree, int rank )
{
    Set( degree, static_cast< uint64_t >( rank ) );
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Permutation::Set( const vector< int > & perm )
{
    m_perm = perm;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Permutation::Set( int degree, const Cycle & cycle )
{
    Assert( degree >= cycle.MinDegree() );
    Set( degree );
    int cycleLen = cycle.Length();
    if ( cycleLen < 2 )
        return;
    for ( int i = 0; i < cycleLen - 1; ++i )
        m_perm[ cycle[i] ] = cycle[ i + 1 ];
    m_perm[ cycle[ cycleLen - 1 ] ] = cycle[ 0 ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Permutation::Set( int degree, const Transposition & trans )
{
    Assert( degree >= trans.MinDegree() );
    Set( degree );
    m_perm[ trans[0] ] = trans[1];
    m_perm[ trans[1] ] = trans[0];
}

//=============================================================================

bool 
Permutation::Previous( )
{
    return prev_permutation( m_perm.begin(), m_perm.end() );
}

//-----------------------------------------------------------------------------

bool 
Permutation::Next( )
{
    return next_permutation( m_perm.begin(), m_perm.end() );
}

//=============================================================================

bool 
Permutation::Valid( ) const
{
    try
    {
        Permutation inverse = Inverse( );
        for ( int i = 0; i < Degree(); ++i )
            if ( inverse[i] < 0 )
                return false;
        return true;
    }
    catch ( out_of_range )
    {
        return false;
    }
}

//=============================================================================

Permutation 
Permutation::Inverse( ) const
{
    vector< int > inverse( Degree(), -1 );
    for ( int i = 0; i < Degree(); ++i )
    {
        if ( (m_perm[i] < 0) || (m_perm[i] >= Degree()) )
            throw out_of_range( "Invalid permutation" );
        inverse[ m_perm[i] ] = i;
    }
    return Permutation( inverse );
}

//=============================================================================

vector< Permutation::Cycle > 
Permutation::FactorIntoCycles( ) const
{
    vector< bool > seen( Degree(), false );
    vector< Cycle > cycles;
    bool done = (Degree() <= 0);
    int i = 0;
    while ( ! done )
    {
        while ( (i < Degree()) && seen[i] )
            ++i;
        if ( i >= Degree() )
            done = true;
        else
        {
            vector< int > cycle;
            int j = i;
            do
            {
                cycle.push_back( j );
                seen[j] = true;
                j = m_perm[j];
            } while ( j != i );
            cycles.push_back( Cycle( cycle ) );
        }
    }
    return cycles;
}

//-----------------------------------------------------------------------------

vector< Permutation::Transposition > 
Permutation::FactorIntoTranspositions( ) const
{
    vector< Cycle > cycles = FactorIntoCycles( );
    vector< Transposition > transpositions;
    for ( size_t i = 0; i < cycles.size(); ++i )
    {
        vector< Transposition > transps
                = cycles[i].FactorIntoTranspositions( );
        transpositions.insert( transpositions.end(),
                               transps.begin(), transps.end() );
    }
    return transpositions;
}

//-----------------------------------------------------------------------------

bool 
Permutation::Even( ) const
{
    vector< Cycle > cycles = FactorIntoCycles( );
    int numTrans = 0;
    for ( size_t i = 0; i < cycles.size(); ++i )
        numTrans += cycles[i].Length() - 1;
    return ( (numTrans & 1) == 0 );
}

//=============================================================================

uint64_t 
Permutation::Rank( ) const
{
    uint64_t rank = 0;
    int degree = Degree();
    vector< int > perm = m_perm;
    for ( int i = 0; i < degree; ++i )
    {
        int v = perm[i];
        rank += v * Factorial( degree - 1 - i );
        for ( int j = (i + 1); j < degree; ++j )
            if ( perm[j] > v )
                --perm[j];
    }
    return rank;
}

//=============================================================================

bool 
Permutation::operator==( const Permutation & rhs ) const
{
    return ( m_perm == rhs.m_perm );
}

//-----------------------------------------------------------------------------

bool 
Permutation::operator<( const Permutation & rhs ) const
{
    return ( m_perm < rhs.m_perm );
}

//=============================================================================

Permutation
operator*( const Permutation & lhs, const Permutation & rhs )
{
    int degree = lhs.Degree();
    Assert( degree == rhs.Degree() );
    vector< int > product( degree );
    for ( int i = 0; i < degree; ++i )
        product[i] = rhs[ lhs[i] ];
    return product;
}

//=============================================================================

ostream & 
operator<<( ostream & out, const Permutation & perm )
{
    out << "( ";
    for ( int i = 0; i < perm.Degree(); ++i )
        out << ((i == 0) ? "" : ", ") << perm[i];
    return out << " )";
}

//=============================================================================

string 
ToJSON( const Permutation & perm )
{
    return ToJSON( perm.m_perm );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const string & json, Permutation * pPerm )
{
    FromJSON( json, &(pPerm->m_perm) );
}


//*****************************************************************************


Permutation::Cycle::Cycle( const vector< int > & cycle )
    :   m_cycle( cycle )
{
}

//-----------------------------------------------------------------------------

void 
Permutation::Cycle::Set( const vector< int > & cycle )
{
    m_cycle = cycle;
}

//=============================================================================

int 
Permutation::Cycle::MinDegree( ) const
{
    Assert( m_cycle.size() > 0 );
    vector<int>::const_iterator maxIt
            = max_element( m_cycle.begin(), m_cycle.end() );
    return (*maxIt + 1);
}

//=============================================================================

Permutation::Cycle 
Permutation::Cycle::Inverse( ) const
{
    vector< int > inverse = m_cycle;
    reverse( inverse.begin(), inverse.end() );
    return Cycle( inverse );
}

//=============================================================================

vector< Permutation::Transposition > 
Permutation::Cycle::FactorIntoTranspositions( ) const
{
    vector< Transposition > transpositions;
    for ( size_t i = 1; i < m_cycle.size(); ++i )
        transpositions.push_back( Transposition( m_cycle[0], m_cycle[i] ) );
    return transpositions;
}

//=============================================================================

ostream & 
operator<<( ostream & out, const Permutation::Cycle & cycle )
{
    out << "( ";
    for ( int i = 0; i < cycle.Length(); ++i )
        out << ((i == 0) ? "" : ", ") << cycle[i];
    return out << " )";
}

//=============================================================================

string 
ToJSON( const Permutation::Cycle & cycle )
{
    return ToJSON( cycle.m_cycle );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const std::string & json, Permutation::Cycle * pCycle )
{
    FromJSON( json, &(pCycle->m_cycle) );
}


//*****************************************************************************


ostream & 
operator<<( ostream & out, const Permutation::Transposition & trans )
{
    return out << "( " << trans[0] << ", " << trans[1] << " )";
}

//=============================================================================

string 
ToJSON( const Permutation::Transposition & trans )
{
    return ToJSON( trans.m_indices );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const std::string & json, Permutation::Transposition * pTrans )
{
    FromJSON( json, &(pTrans->m_indices) );
}


//*****************************************************************************


#ifdef DEBUG

bool 
Permutation::Test( )
{
    bool ok = true;
    cout << "Testing Permutation" << endl;

    cout << "Permutation( ) [default constructor]" << endl;
    Permutation perm0;
    TESTCHECK( perm0.Valid(), true, &ok );
    TESTCHECK( perm0.Degree(), 0, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[  ]" ), &ok );
    TESTCHECK( (perm0.Inverse() == perm0), true, &ok );
    TESTCHECK( (perm0.Inverse() < perm0), false, &ok );
    TESTCHECK( perm0.Next(), false, &ok );
    TESTCHECK( perm0.Degree(), 0, &ok );
    TESTCHECK( perm0.Previous(), false, &ok );
    TESTCHECK( perm0.Degree(), 0, &ok );
    TESTCHECK( perm0.FactorIntoCycles().size(), 0, &ok );
    TESTCHECK( perm0.FactorIntoTranspositions().size(), 0,
               &ok );
    TESTCHECK( perm0.Even(), true, &ok );
    vector< string > strVec0;
    TESTCHECK( perm0.Apply( strVec0 ).size(), 0, &ok );

    cout << "Set( 3 ) [degree 3 identity]" << endl;
    perm0.Set( 3 );
    TESTCHECK( perm0.Valid(), true, &ok );
    TESTCHECK( perm0.Degree(), 3, &ok );
    TESTCHECK( perm0[2], 2, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 0, 1, 2 ]" ), &ok );
    TESTCHECK( (perm0.Inverse() == perm0), true, &ok );
    TESTCHECK( (perm0.Inverse() < perm0), false, &ok );
    TESTCHECK( perm0.Even(), true, &ok );
    TESTCHECK( perm0.Previous(), false, &ok );
    TESTCHECK( perm0.Degree(), 3, &ok );
    TESTCHECK( perm0[2], 0, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 2, 1, 0 ]" ), &ok );
    TESTCHECK( perm0.Even(), false, &ok );
    TESTCHECK( perm0.Next(), false, &ok );
    TESTCHECK( perm0.Degree(), 3, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 0, 1, 2 ]" ), &ok );
    TESTCHECK( perm0.Next(), true, &ok );
    TESTCHECK( perm0.Degree(), 3, &ok );
    TESTCHECK( perm0[2], 1, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 0, 2, 1 ]" ), &ok );
    TESTCHECK( perm0.Even(), false, &ok );
    cout << "FromJSON( \"[ 0, 2, 1 ]\", &perm0 )" << endl;
    FromJSON( "[ 0, 2, 1 ]", &perm0 );
    TESTCHECK( perm0.Degree(), 3, &ok );
    TESTCHECK( perm0[2], 1, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 0, 2, 1 ]" ), &ok );
    TESTCHECK( perm0.Even(), false, &ok );
    cout << "FactorIntoCycles( )" << endl;
    vector< Cycle > cycles0 = perm0.FactorIntoCycles( );
    TESTCHECK( cycles0.size(), 2, &ok );
    TESTCHECK( cycles0[0].Length(), 1, &ok );
    TESTCHECK( cycles0[0][0], 0, &ok );
    TESTCHECK( cycles0[1][1], 2, &ok );
    TESTCHECK( cycles0[0].MinDegree(), 1, &ok );
    TESTCHECK( cycles0[1].MinDegree(), 3, &ok );
    ostringstream ost0;
    cout << "operator<<" << endl;
    ost0 << cycles0[1];
    TESTCHECK( ost0.str(), string( "( 1, 2 )" ), &ok );
    TESTCHECK( ToJSON( cycles0[1] ), string( "[ 1, 2 ]" ), &ok );
    cout << "FactorIntoTranspositions( )" << endl;
    vector< Transposition > transps0 = perm0.FactorIntoTranspositions( );
    TESTCHECK( transps0.size(), 1, &ok );
    TESTCHECK( transps0[0][0], 1, &ok );
    TESTCHECK( ToJSON( transps0[0] ), string( "[ 1, 2 ]" ), &ok );
    TESTCHECK( transps0[0].MinDegree( ), 3, &ok );
    array< string, 3 > strings0 = {{ "one", "two", "three" }};
    cout << "strings0" << ToJSON( strings0 ) << endl;
    TESTCHECK( ToJSON( perm0.Apply( strings0 ) ),
               string( "[ \"one\", \"three\", \"two\" ]" ), &ok );

    cout << "Set( 6, true) [random]" << endl;
    perm0.Set( 6, true );
    cout << ToJSON( perm0 ) << endl;
    TESTCHECK( perm0.Valid(), true, &ok );
    TESTCHECK( perm0.Degree(), 6, &ok );
    cout << "Set( 6, true) [random]" << endl;
    perm0.Set( 6, true );
    cout << ToJSON( perm0 ) << endl;
    TESTCHECK( perm0.Valid(), true, &ok );
    TESTCHECK( perm0.Degree(), 6, &ok );

    cout << "Set( 4, 5 ) [5th permutation]" << endl;
    perm0.Set( 4, 5 );
    TESTCHECK( perm0.Valid(), true, &ok );
    TESTCHECK( perm0.Degree(), 4, &ok );
    TESTCHECK( perm0[1], 3, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 0, 3, 2, 1 ]" ), &ok );
    TESTCHECK( perm0.Rank( ), 5ULL, &ok );
    TESTCHECK( ToJSON( perm0.Inverse() ), string( "[ 0, 3, 2, 1 ]" ), &ok );
    cout << "Set( 10, 999999 ) [999,999th permutation]" << endl;
    perm0.Set( 10, 999999 );
    TESTCHECK( perm0.Valid(), true, &ok );
    TESTCHECK( perm0.Degree(), 10, &ok );
    TESTCHECK( perm0[4], 9, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 2, 7, 8, 3, 9, 1, 5, 4, 6, 0 ]" ),
                                        &ok );
    TESTCHECK( perm0.Rank( ), 999999ULL, &ok );
    TESTCHECK( ToJSON( perm0.Inverse() ),
                       string( "[ 9, 5, 0, 3, 7, 6, 8, 1, 2, 4 ]" ), &ok );
    TESTCHECK( ( perm0 * perm0.Inverse() == Permutation( 10 ) ), true, &ok );
    TESTCHECK( ( perm0.Inverse() * perm0 == Permutation( 10 ) ), true, &ok );
    
    array< int, 6 > arr0 = {{ 2, 1, 3, 5, 4, 0 }};
    vector< int > vec0( arr0.begin(), arr0.end() );
    cout << "Set( " << ToJSON( vec0 ) << " ) [vector]" << endl;
    perm0.Set( vec0 );
    TESTCHECK( perm0.Valid(), true, &ok );
    TESTCHECK( perm0.Degree(), 6, &ok );
    TESTCHECK( perm0[2], 3, &ok );
    ostringstream ost1;
    cout << "operator<<" << endl;
    ost1 << perm0;
    TESTCHECK( ost1.str(), string( "( 2, 1, 3, 5, 4, 0 )" ), &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 2, 1, 3, 5, 4, 0 ]" ), &ok );
    cout << "FromJSON( \"[ 2, 1, 3, 5, 4, 0 ]\", &perm0 )" << endl;
    FromJSON( "[ 2, 1, 3, 5, 4, 0 ]", &perm0 );
    TESTCHECK( perm0.Valid(), true, &ok );
    TESTCHECK( perm0.Degree(), 6, &ok );
    TESTCHECK( perm0[2], 3, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 2, 1, 3, 5, 4, 0 ]" ), &ok );
    TESTCHECK( perm0.Next(), true, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 2, 1, 4, 0, 3, 5 ]" ), &ok );
    TESTCHECK( perm0.Previous(), true, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 2, 1, 3, 5, 4, 0 ]" ), &ok );

    array< int, 4 > arr1 = {{ 2, 0, 4, 1 }};
    vec0.assign( arr1.begin(), arr1.end() );
    cout << "Permutation( " << ToJSON( vec0 ) << " ) [vector constructor]"
         << endl;
    Permutation perm1( vec0 );
    TESTCHECK( perm1.Valid(), false, &ok );
    arr1[2] = 2;
    cout << "Set ( " << ToJSON( arr1 ) << " ) [iterators]"
         << endl;
    perm1.Set( arr1.begin(), arr1.end() );
    TESTCHECK( perm1.Valid(), false, &ok );

    arr1[2] = 3;
    cout << "Permutation( " << ToJSON( arr1 ) << " ) [iterator constructor]"
         << endl;
    Permutation perm2( arr1.begin(), arr1.end() );
    TESTCHECK( perm2.Valid(), true, &ok );
    TESTCHECK( perm2.Degree(), 4, &ok );
    TESTCHECK( ToJSON( perm2 ), string( "[ 2, 0, 3, 1 ]" ), &ok );
    string str0 = "abcd";
    TESTCHECK( perm2.Apply( str0 ), string( "cadb" ), &ok );
    TESTCHECK( perm2.Inverse().Apply( str0 ), string( "bdac" ), &ok );

    array< int, 3 > arr2 = {{ 0, 1, 2 }};
    vec0.assign( arr2.begin(), arr2.end() );
    cout << "Permutation::Cycle( " << ToJSON( arr2 )
         << " ) [vector constructor]" << endl;
    Permutation::Cycle cycle0( vec0 );
    TESTCHECK( cycle0.MinDegree( ), 3, &ok );
    TESTCHECK( cycle0.Length(), 3, &ok );
    TESTCHECK( cycle0[2], 2, &ok );
    TESTCHECK( ToJSON( cycle0 ), string( "[ 0, 1, 2 ]" ), &ok );
    array< string, 9 > strings1 = {{ "one", "two", "three", "four", "five",
                                     "six", "seven", "eight", "nine" }};
    cout << "Apply( " << ToJSON( strings1 ) << " ) [array]" << endl;
    cycle0.Apply( &strings1 );
    TESTCHECK( ToJSON( strings1 ),
               string( "[ \"two\", \"three\", \"one\", \"four\", \"five\", "
                       "\"six\", \"seven\", \"eight\", \"nine\" ]" ), &ok );
    cout << "Inverse().Apply( )" << endl;
    cycle0.Inverse( ).Apply( &strings1 );
    TESTCHECK( ToJSON( strings1 ),
               string( "[ \"one\", \"two\", \"three\", \"four\", \"five\", "
                       "\"six\", \"seven\", \"eight\", \"nine\" ]" ), &ok );
    cout << "Permutation::Set( 9, cycle0 )" << endl;
    perm0.Set( 9, cycle0 );
    TESTCHECK( perm0.Valid(), true, &ok );
    TESTCHECK( perm0.Degree(), 9, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 1, 2, 0, 3, 4, 5, 6, 7, 8 ]" ),
               &ok );
    TESTCHECK( ToJSON( perm0.Apply( strings1 ) ), 
               string( "[ \"two\", \"three\", \"one\", \"four\", \"five\", "
                       "\"six\", \"seven\", \"eight\", \"nine\" ]" ), &ok );

    array< int, 5 > arr3 = {{ 4, 5, 3, 0, 7 }};
    vec0.assign( arr3.begin(), arr3.end() );
    cout << "Permutation::Cycle::Set( " << ToJSON( arr3 )
         << " ) [vector]" << endl;
    Permutation::Cycle cycle1 = cycle0;
    cycle1.Set( vec0 );
    TESTCHECK( cycle1.MinDegree(), 8, &ok );
    TESTCHECK( cycle1.Length(), 5, &ok );
    TESTCHECK( cycle1[3], 0, &ok );
    TESTCHECK( ToJSON( cycle1 ), string( "[ 4, 5, 3, 0, 7 ]" ), &ok );
    cout << "FromJSON( \"[ 4, 5, 3, 0, 7 ]\", &cycle1 )" << endl;
    FromJSON( "[ 4, 5, 3, 0, 7 ]", &cycle1 );
    TESTCHECK( cycle1.MinDegree(), 8, &ok );
    TESTCHECK( cycle1.Length(), 5, &ok );
    TESTCHECK( cycle1[3], 0, &ok );
    TESTCHECK( ToJSON( cycle1 ), string( "[ 4, 5, 3, 0, 7 ]" ), &ok );
    cout << "Apply( &strings1 )" << endl;
    cycle1.Apply( &strings1 );
    TESTCHECK( ToJSON( strings1 ),
               string( "[ \"eight\", \"two\", \"three\", \"one\", \"six\", "
                       "\"four\", \"seven\", \"five\", \"nine\" ]" ), &ok );
    cout << "Inverse().Apply( )" << endl;
    cycle1.Inverse( ).Apply( &strings1 );
    TESTCHECK( ToJSON( strings1 ),
               string( "[ \"one\", \"two\", \"three\", \"four\", \"five\", "
                       "\"six\", \"seven\", \"eight\", \"nine\" ]" ), &ok );
    cout << "Permutation::Set( 9, cycle1 )" << endl;
    perm1.Set( 9, cycle1 );
    TESTCHECK( perm1.Valid(), true, &ok );
    TESTCHECK( perm1.Degree(), 9, &ok );
    TESTCHECK( ToJSON( perm1 ), string( "[ 7, 1, 2, 0, 5, 3, 6, 4, 8 ]" ),
               &ok );
    TESTCHECK( ToJSON( perm1.Apply( strings1 ) ),
               string( "[ \"eight\", \"two\", \"three\", \"one\", \"six\", "
                       "\"four\", \"seven\", \"five\", \"nine\" ]" ), &ok );
    TESTCHECK( ToJSON( perm0 * perm1 ),
               string( "[ 1, 2, 7, 0, 5, 3, 6, 4, 8 ]" ), &ok );
    cout << "(perm0 * perm1).Apply( strings1 )" << endl;
    array< string, 9 > strings2 = (perm0 * perm1).Apply( strings1 );
    cout << "cycle1.Apply( &strings1 )" << endl;
    cycle1.Apply( &strings1 );
    cout << "cycle0.Apply( &strings1 )" << endl;
    cycle0.Apply( &strings1 );
    TESTCHECK( ToJSON( strings1 ), ToJSON( strings2 ), &ok );
    cout << "(perm0 * perm1).Inverse().Apply( strings1 )" << endl;
    strings1 = (perm0 * perm1).Inverse().Apply( strings1 );
    TESTCHECK( ToJSON( strings1 ),
               string( "[ \"one\", \"two\", \"three\", \"four\", \"five\", "
                       "\"six\", \"seven\", \"eight\", \"nine\" ]" ), &ok );
    TESTCHECK( ToJSON( perm0.Apply( perm1.Apply( strings1 ) ) ),
               ToJSON( strings2 ), &ok );

    array< int, 2 > arr4 = {{ 1, 2 }};
    cout << "Permutation::Transposition( " << ToJSON( arr4 )
         << " ) [array constructor]" << endl;
    Permutation::Transposition trans0( arr4 );
    TESTCHECK( trans0.MinDegree(), 3, &ok );
    TESTCHECK( trans0[1], 2, &ok );
    ostringstream ost2;
    cout << "operator<<" << endl;
    ost2 << trans0;
    TESTCHECK( ost2.str(), string( "( 1, 2 )" ), &ok );
    TESTCHECK( ToJSON( trans0 ), string( "[ 1, 2 ]" ), &ok );
    cout << "FromJSON( \"[ 1, 2 ]\", &trans0 )" << endl;
    FromJSON( "[ 1, 2 ]", &trans0 );
    TESTCHECK( trans0.MinDegree(), 3, &ok );
    TESTCHECK( trans0[1], 2, &ok );
    TESTCHECK( ToJSON( trans0 ), string( "[ 1, 2 ]" ), &ok );
    array< string, 4 > strings3 = {{ "one", "two", "three", "four" }};
    cout << "Apply( " << ToJSON( strings3 ) << " )" << endl;
    trans0.Apply( &strings3 );
    TESTCHECK( ToJSON( strings3 ),
               string( "[ \"one\", \"three\", \"two\", \"four\" ]" ), &ok );
    cout << "Apply( " << ToJSON( strings3 ) << " )" << endl;
    trans0.Apply( &strings3 );
    TESTCHECK( ToJSON( strings3 ),
               string( "[ \"one\", \"two\", \"three\", \"four\" ]" ), &ok );
    cout << "Permutation::Set( 4, trans0 )" << endl;
    perm0.Set( 4, trans0 );
    TESTCHECK( perm0.Valid(), true, &ok );
    TESTCHECK( perm0.Degree(), 4, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 0, 2, 1, 3 ]" ), &ok );
    TESTCHECK( ToJSON( perm0.Apply( strings3 ) ),
               string( "[ \"one\", \"three\", \"two\", \"four\" ]" ), &ok );
    TESTCHECK( ToJSON( perm0.Inverse() ), string( "[ 0, 2, 1, 3 ]" ), &ok );

    cout << "Permutation::Transposition( 1, 3 ) [index constructor]" << endl;
    Permutation::Transposition trans1( 1, 3 );
    TESTCHECK( trans1.MinDegree(), 4, &ok );
    TESTCHECK( trans1[1], 3, &ok );
    cout << "Apply( " << ToJSON( strings3 ) << " )" << endl;
    trans1.Apply( &strings3 );
    TESTCHECK( ToJSON( strings3 ),
               string( "[ \"one\", \"four\", \"three\", \"two\" ]" ), &ok );
    cout << "Apply( " << ToJSON( strings3 ) << " )" << endl;
    trans1.Apply( &strings3 );
    TESTCHECK( ToJSON( strings3 ),
               string( "[ \"one\", \"two\", \"three\", \"four\" ]" ), &ok );
    cout << "Permutation::Set( 4, trans1 )" << endl;
    perm0.Set( 4, trans1 );
    TESTCHECK( perm0.Valid(), true, &ok );
    TESTCHECK( perm0.Degree(), 4, &ok );
    TESTCHECK( ToJSON( perm0 ), string( "[ 0, 3, 2, 1 ]" ), &ok );
    TESTCHECK( ToJSON( perm0.Apply( strings3 ) ),
               string( "[ \"one\", \"four\", \"three\", \"two\" ]" ), &ok );
    TESTCHECK( ToJSON( perm0.Inverse() ), string( "[ 0, 3, 2, 1 ]" ), &ok );
    cout << "Set( arr4 )" << endl;
    trans1.Set( arr4 );
    TESTCHECK( trans1.MinDegree(), 3, &ok );
    TESTCHECK( trans1[1], 2, &ok );
    cout << "Apply( " << ToJSON( strings3 ) << " )" << endl;
    trans1.Apply( &strings3 );
    TESTCHECK( ToJSON( strings3 ),
               string( "[ \"one\", \"three\", \"two\", \"four\" ]" ), &ok );
    cout << "Apply( " << ToJSON( strings3 ) << " )" << endl;
    trans1.Apply( &strings3 );
    TESTCHECK( ToJSON( strings3 ),
               string( "[ \"one\", \"two\", \"three\", \"four\" ]" ), &ok );

    perm0 = ComputePermutation( string( "MINE ANDERSON" ),
                                string( "MODERN SIENNA" ) );
    TESTCHECK( perm0.Valid( ), true, &ok );
    TESTCHECK( perm0.Degree( ), 13, &ok );
    TESTCHECK( ToJSON( perm0 ),
               string( "[ 0, 11, 7, 3, 9, 2, 4, 10, 1, 8, 6, 12, 5 ]" ), &ok );
    TESTCHECK( perm0.Apply( string( "MINE ANDERSON" ) ),
               string( "MODERN SIENNA" ), &ok );
    TESTCHECK( perm0.Inverse().Apply( string( "MODERN SIENNA" ) ),
               string( "MINE ANDERSON" ), &ok );
    
    if ( ok )
        cout << "Permutation PASSED." << endl << endl;
    else
        cout << "Permutation FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta
