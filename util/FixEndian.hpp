#ifndef FIXENDIAN_HPP
#define FIXENDIAN_HPP
/*
  FixEndian.hpp
  Copyright (C) 2007 David M. Anderson

  Routines for swapping bytes to correct wrong-endian data.
  NOTES:
  1. These routines will swap bytes as appropriate for the standard integer
     and floating point types.
  2. If the datum is known to be little endian (or big endian), call
     FixLittleEndian() (or FixBigEndian()). It will do nothing on little-
     (big-) endian processors. If the datum is known to be "wrong" endian,
     call SwapEndian() directly.
  3. If you want to convert the datum to little endian (or big endian), call
     MakeLittleEndian() (or MakeBigEndian()).
*/


#include "StdInt.hpp"
#include "Assert.hpp"


#if defined(__GNUC__)
#   include <endian.h>
#   if (__BYTE_ORDER == __BIG_ENDIAN)
#       undef LITTLE_ENDIAN
#       define BIG_ENDIAN __BIG_ENDIAN
#   else
#       undef BIG_ENDIAN
#       define LITTLE_ENDIAN __LITTLE_ENDIAN
#   endif

#elif defined(_MSC_VER)
#   if (defined(_M_IX86) || defined(_M_IA64))
#       undef BIG_ENDIAN
#       define LITTLE_ENDIAN 1234
#   else
#       undef LITTLE_ENDIAN
#       define BIG_ENDIAN 4321
#   endif

#endif //compiler


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename T >
void FixLittleEndian( T * pT );
template < typename T >
void FixBigEndian( T * pT );
template < typename T >
void MakeLittleEndian( T * pT );
template < typename T >
void MakeBigEndian( T * pT );

#ifdef DEBUG
bool TestFixEndian( );
#endif


//*****************************************************************************


template < typename T >
inline 
void 
#ifdef BIG_ENDIAN
FixLittleEndian( T * pT )
{
    SwapEndian( pT );
}
#else
FixLittleEndian( T * /*pT*/ )
{
}
#endif

//-----------------------------------------------------------------------------

template < typename T >
inline 
void 
#ifdef LITTLE_ENDIAN
FixBigEndian( T * pT )
{
    SwapEndian( pT );
}
#else
FixBigEndian( T * /*pT*/ )
{
}
#endif

//=============================================================================

template < typename T >
inline 
void 
#ifdef BIG_ENDIAN
MakeLittleEndian( T * pT )
{
    SwapEndian( pT );
}
#else
MakeLittleEndian( T * /*pT*/ )
{
}
#endif

//-----------------------------------------------------------------------------

template < typename T >
inline 
void 
#ifdef LITTLE_ENDIAN
MakeBigEndian( T * pT )
{
    SwapEndian( pT );
}
#else
MakeBigEndian( T * /*pT*/ )
{
}
#endif

//=============================================================================

template < int size >
inline
void 
SwapBytes( unsigned char * /*pC*/ )
{
    Assert( 0 && "Unexpected size for SwapBytes" );
}

//.............................................................................

template <>
inline 
void 
SwapBytes< 2 >( unsigned char * pC )
{
    unsigned char c = pC[0];
    pC[0] = pC[1];
    pC[1] = c;
}

//.............................................................................

template <>
inline 
void 
SwapBytes< 4 >( unsigned char * pC )
{
    unsigned char c = pC[0];
    pC[0] = pC[3];
    pC[3] = c;
    c = pC[1];
    pC[1] = pC[2];
    pC[2] = c;
}

//.............................................................................

template <>
inline 
void 
SwapBytes< 8 >( unsigned char * pC )
{
    unsigned char c = pC[0];
    pC[0] = pC[7];
    pC[7] = c;
    c = pC[1];
    pC[1] = pC[6];
    pC[6] = c;
    c = pC[2];
    pC[2] = pC[5];
    pC[5] = c;
    c = pC[3];
    pC[3] = pC[4];
    pC[4] = c;
}

//.............................................................................

template <>
inline 
void 
SwapBytes< 10 >( unsigned char * pC )
{
    unsigned char c = pC[0];
    pC[0] = pC[9];
    pC[9] = c;
    c = pC[1];
    pC[1] = pC[8];
    pC[8] = c;
    c = pC[2];
    pC[2] = pC[7];
    pC[7] = c;
    c = pC[3];
    pC[3] = pC[6];
    pC[6] = c;
    c = pC[4];
    pC[4] = pC[5];
    pC[5] = c;
}

//=============================================================================

inline
void 
SwapEndian( char * /*pT*/ )
{
}

//.............................................................................

inline
void 
SwapEndian( wchar_t * pT )
{
    SwapBytes< sizeof( *pT ) >( reinterpret_cast<unsigned char *>( pT ) );
}

//-----------------------------------------------------------------------------

inline
void 
SwapEndian( uint8_t * /*pT*/ )
{
}

//.............................................................................

inline
void 
SwapEndian( int8_t * /*pT*/ )
{
}

//-----------------------------------------------------------------------------

inline
void 
SwapEndian( uint16_t * pT )
{
    SwapBytes< sizeof( *pT ) >( reinterpret_cast<unsigned char *>( pT ) );
}

//.............................................................................

inline
void 
SwapEndian( int16_t * pT )
{
    SwapBytes< sizeof( *pT ) >( reinterpret_cast<unsigned char *>( pT ) );
}

//-----------------------------------------------------------------------------

inline
void 
SwapEndian( uint32_t * pT )
{
    SwapBytes< sizeof( *pT ) >( reinterpret_cast<unsigned char *>( pT ) );
}

//.............................................................................

inline
void 
SwapEndian( int32_t * pT )
{
    SwapBytes< sizeof( *pT ) >( reinterpret_cast<unsigned char *>( pT ) );
}

//.............................................................................

inline
void 
SwapEndian( float * pT )
{
    SwapBytes< sizeof( *pT ) >( reinterpret_cast<unsigned char *>( pT ) );
}

//-----------------------------------------------------------------------------

inline
void 
SwapEndian( uint64_t * pT )
{
    SwapBytes< sizeof( *pT ) >( reinterpret_cast<unsigned char *>( pT ) );
}

//.............................................................................

inline
void 
SwapEndian( int64_t * pT )
{
    SwapBytes< sizeof( *pT ) >( reinterpret_cast<unsigned char *>( pT ) );
}

//-----------------------------------------------------------------------------

inline
void 
SwapEndian( double * pT )
{
    SwapBytes< sizeof( *pT ) >( reinterpret_cast<unsigned char *>( pT ) );
}

//.............................................................................

inline
void 
SwapEndian( long double * pT )
{
    SwapBytes< sizeof( *pT ) >( reinterpret_cast<unsigned char *>( pT ) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FIXENDIAN_HPP
