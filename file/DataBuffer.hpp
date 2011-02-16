#ifndef DATABUFFER_HPP
#define DATABUFFER_HPP
/*
  DataBuffer.hpp
  Copyright (C) 2009 David M. Anderson

  DataBuffer class: Holds a buffer of data, primarily for purposes such as
  file I/O.
  NOTES:
  1. When adding, the data are appended to the end of the buffer.
  2. Reads begin at the beginning of the buffer and proceed sequentially; an
     offset is updated after each read to prepare for the next.
  3. Since the template form of Read() takes no arguments, the template
     parameter needs to be specified by the caller.
     (E.g., const Foo * foo = buff.Read< Foo >( ); )
*/


#include "Exception.hpp"
#include <vector>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class DataBuffer
{
public:
    DataBuffer( );

    void Clear( );

    void Add( const char * data, int dataSize );
    template <typename T>
    void Add( const T & t );

    const char * Read( int dataSize );
    template <typename T>
    const T * Read( );

    const std::vector< char > & Buffer( ) const;
    char * Data( );
    const char * Data( ) const;
    int Size( ) const;

#ifdef DEBUG
    static bool Test( );
#endif

private:
    std::vector< char > & GetBuffer( );

    std::vector< char > m_buffer;
    size_t              m_offset;

    friend class Reader;
};


//*****************************************************************************


inline
DataBuffer::DataBuffer( )
    :   m_offset( 0 )
{
}

//=============================================================================

inline
void 
DataBuffer::Clear( )
{
    m_buffer.clear( );
    m_offset = 0;
}

//=============================================================================

inline
void 
DataBuffer::Add( const char * data, int dataSize )
{
    m_buffer.insert( m_buffer.end(), data, data + dataSize );
}

//-----------------------------------------------------------------------------

template <typename T>
inline
void 
DataBuffer::Add( const T & t )
{
    Add( reinterpret_cast< const char * >( &t ), (int)sizeof( t ) );
}

//=============================================================================

inline
const char * 
DataBuffer::Read( int dataSize )
{
    if ( m_offset + (size_t)dataSize <= m_buffer.size() )
    {
        const char * data = &m_buffer[ m_offset ];
        m_offset += dataSize;
        return data;
    }
    else
    {
        throw RuntimeError( "Read past end of DataBuffer" );
    }
}

//-----------------------------------------------------------------------------

template <typename T>
inline
const T * 
DataBuffer::Read( )
{
    if ( m_offset + sizeof( T ) <= m_buffer.size() )
    {
        const T * pT = reinterpret_cast< const T * >( &m_buffer[ m_offset ] );
        m_offset += sizeof( T );
        return pT;
    }
    else
    {
        throw RuntimeError( "Read past end of DataBuffer" );
    }
}

//=============================================================================

inline
const std::vector< char > & 
DataBuffer::Buffer( ) const
{
    return m_buffer;
}

//.............................................................................

inline
std::vector< char > & 
DataBuffer::GetBuffer( )
{
    return m_buffer;
}

//=============================================================================

inline
char * 
DataBuffer::Data( )
{
    if ( m_buffer.size() == 0 )
        return 0;
    return &m_buffer[ 0 ];
}

//.............................................................................

inline
const char * 
DataBuffer::Data( ) const
{
    if ( m_buffer.size() == 0 )
        return 0;
    return &m_buffer[ 0 ];
}

//-----------------------------------------------------------------------------

inline
int 
DataBuffer::Size( ) const
{
    return (int)m_buffer.size();
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //DATABUFFER_HPP
