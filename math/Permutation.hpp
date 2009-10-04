#ifndef PERMUTATION_HPP
#define PERMUTATION_HPP
/*
  Permutation.hpp
  Copyright (C) 2007 David M. Anderson

  Permutation class: Represents a permutation, i.e., a one-to-one mapping of a
  finite set onto itself.
  NOTES:
  1. A permutation of a set of size N (the degree of the permutation) is
     represented as an ordering of the integers 0,...,N-1. So if a set
     S = { s[0],...,s[N-1] }, the permutation maps s[i] to s[ perm[i] ].
  2. Given two permutations, perm0 and perm1, perm0 * perm1 is the permutation
     that maps s[i] to s[ perm1[ perm0[i] ] ]. Under this operation, the
     set of permutations (of a given degree) is a non-abelian group.
  3. The default constructor, Permutation(), produces the identity, which
     maps s[i] to s[i].
  4. Permutation( true ) yields a pseudo-random permutation.
  5. There is a canonical ordering of the N! permutations of degree N, which
     is lexicographic. The constructor Permutation( int, uint64_t ) yields the
     kth permutation in this sequence. However, the degree must be <= 32 to
     avoid integer overflow (using uint64_t for intermediate calculations).
  6. 
*/


#include "StdInt.hpp"
#include "Assert.hpp"
#include <vector>
#include <tr1/array>
#include <algorithm>
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Permutation
{
public:

//.............................................................................

    class Cycle;
    class Transposition;

//.............................................................................

    Permutation( );
    Permutation( int degree, bool random = false );
    Permutation( int degree, uint64_t rank );
    Permutation( int degree, int rank );
    Permutation( const std::vector< int > & perm );
    template <typename In> Permutation( In first, In last );
    Permutation( int degree, const Cycle & cycle );
    Permutation( int degree, const Transposition & trans );
    void Set( int degree, bool random = false );
    void Set( int degree, uint64_t rank );
    void Set( int degree, int rank );
    void Set( const std::vector< int > & perm );
    template <typename In> void Set( In first, In last );
    void Set( int degree, const Cycle & cycle );
    void Set( int degree,  const Transposition & trans );

    bool Previous( );
    bool Next( );

    bool Valid( ) const;
    int Degree( ) const;
    int operator[]( int index ) const;
    Permutation Inverse( ) const;
    std::vector< Cycle > FactorIntoCycles( ) const;
    std::vector< Transposition > FactorIntoTranspositions( ) const;
    bool Even( ) const;
    uint64_t Rank( ) const;
    template < typename RSC >
    RSC Apply( const RSC & set ) const;
    template < typename T, size_t N >
    std::tr1::array< T, N > Apply( const std::tr1::array< T, N > & set ) const;

    bool operator==( const Permutation & rhs ) const;
    bool operator<( const Permutation & rhs ) const;

#ifdef DEBUG
    static bool Test( );
#endif

//.............................................................................

    class Cycle
    {
    public:
        Cycle( const std::vector< int > & cycle );
        void Set( const std::vector< int > & cycle );

        int MinDegree( ) const;
        int Length( ) const;
        int operator[]( int index ) const;
        Cycle Inverse( ) const;
        std::vector< Transposition > FactorIntoTranspositions( ) const;
        template < typename RC >
        void Apply( RC * pSet ) const;

    private:
        std::vector< int >  m_cycle;

        friend std::string ToJSON( const Cycle & cycle );
        friend void FromJSON( const std::string & json, Cycle * pCycle );
    };

//.............................................................................

    class Transposition
    {
    public:
        Transposition( const std::tr1::array< int, 2 > indices );
        Transposition( int index0, int index1 );
        void Set( const std::tr1::array< int, 2 > indices );
        void Set( int index0, int index1 );

        int MinDegree( ) const;
        int operator[]( int index ) const;
        template < typename RC >
        void Apply( RC * pSet ) const;

    private:
        std::tr1::array< int, 2 > m_indices;

        friend std::string ToJSON( const Transposition & trans );
        friend void FromJSON( const std::string & json,
                              Transposition * pTrans );
    };

//.............................................................................

private:
    std::vector< int >  m_perm;

    friend Permutation operator*( const Permutation & lhs,
                                  const Permutation & rhs );
    friend std::string ToJSON( const Permutation & perm );
    friend void FromJSON( const std::string & json, Permutation * pPerm );
};

//.............................................................................

Permutation operator*( const Permutation & lhs, const Permutation & rhs );
std::ostream & operator<<( std::ostream & out, const Permutation & perm );
std::string ToJSON( const Permutation & perm );
void FromJSON( const std::string & json, Permutation * pPerm );
std::ostream & operator<<( std::ostream & out,
                           const Permutation::Cycle & cycle );
std::string ToJSON( const Permutation::Cycle & cycle );
void FromJSON( const std::string & json, Permutation::Cycle * pPerm );
std::ostream & operator<<( std::ostream & out,
                           const Permutation::Transposition & trans );
std::string ToJSON( const Permutation::Transposition & trans );
void FromJSON( const std::string & json, Permutation::Transposition * pPerm );
template < typename RC >
Permutation ComputePermutation( const RC & before,
                                const RC & after );


//#############################################################################


template <typename In> 
Permutation::Permutation( In first, In last )
    :   m_perm( first, last )
{
}

//-----------------------------------------------------------------------------

template <typename In> 
void 
Permutation::Set( In first, In last )
{
    m_perm.assign( first, last );
}

//=============================================================================

inline
int 
Permutation::Degree( ) const
{
    return m_perm.size();
}

//-----------------------------------------------------------------------------

inline
int 
Permutation::operator[]( int index ) const
{
    return m_perm[ index ];
}

//=============================================================================

template < typename RSC >
RSC 
Permutation::Apply( const RSC & set ) const
{
    Assert( static_cast<int>(set.size()) == Degree() );
    typedef typename RSC::value_type T;
    RSC permuted( Degree(), T() );
    for ( int i = 0; i < Degree(); ++i )
        permuted[i] = set[ m_perm[i] ];
    return permuted;
}

//-----------------------------------------------------------------------------

template < typename T, size_t N >
std::tr1::array< T, N > 
Permutation::Apply( const std::tr1::array< T, N > & set ) const
{
    Assert( N == Degree() );
    std::tr1::array< T, N > permuted;
    for ( int i = 0; i < N; ++i )
        permuted[i] = set[ m_perm[i] ];
    return permuted;
}

//=============================================================================

template < typename RC >
Permutation 
ComputePermutation( const RC & before,
                    const RC & after )
{
    int degree = before.size();
    Assert( degree == static_cast<int>(after.size()) );
    std::vector< int > perm( degree );
    std::vector< bool > seen( degree, false );
    for ( int i = 0; i < degree; ++i )
    {
        typename RC::const_reference a = after[i];
        int j = 0;
        for ( ; j < degree; ++j )
        {
            if ( (before[j] == a) && (! seen[j]) )
            {
                perm[i] = j;
                seen[j] = true;
                break;
            }
        }
        Assert( j < degree );
    }
    return Permutation( perm );
}


//*****************************************************************************


inline
int 
Permutation::Cycle::Length( ) const
{
    return m_cycle.size();
}

//-----------------------------------------------------------------------------

inline
int 
Permutation::Cycle::operator[]( int index ) const
{
    return m_cycle[ index ];
}

//=============================================================================

template < typename RC >
void 
Permutation::Cycle::Apply( RC * pSet ) const
{
    Assert( static_cast<int>(pSet->size()) >= MinDegree() );
    typename RC::value_type s0 = (*pSet)[ m_cycle[0] ];
    for ( size_t i = 0; i < m_cycle.size() - 1; ++i )
        (*pSet)[ m_cycle[i] ] = (*pSet)[ m_cycle[ i + 1 ] ];
    (*pSet)[ m_cycle[ m_cycle.size() - 1 ] ] = s0;
}


//*****************************************************************************


inline
void 
Permutation::Transposition::Set( const std::tr1::array< int, 2 > indices )
{
    m_indices = indices;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
void 
Permutation::Transposition::Set( int index0, int index1 )
{
    m_indices[0] = index0;
    m_indices[1] = index1;
}

//-----------------------------------------------------------------------------

inline
Permutation::Transposition::Transposition(
    const std::tr1::array< int, 2 > indices )
    :   m_indices( indices )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Permutation::Transposition::Transposition( int index0, int index1 )
{
    Set( index0, index1 );
}

//=============================================================================

inline
int 
Permutation::Transposition::MinDegree( ) const
{
    return ( std::max( m_indices[0], m_indices[1] ) + 1 );
}

//-----------------------------------------------------------------------------

inline
int 
Permutation::Transposition::operator[]( int index ) const
{
    return m_indices[ index ];
}

//=============================================================================

template < typename RC >
void 
Permutation::Transposition::Apply( RC * pSet ) const
{
    Assert( static_cast<int>(pSet->size()) >= MinDegree() );
    std::swap( (*pSet)[ m_indices[0] ], (*pSet)[ m_indices[1] ] );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //PERMUTATION_HPP
