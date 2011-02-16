/*
  NestedReader.cpp
  Copyright (C) 2011 David M. Anderson

  NestedReader class: a "file" that is embedded inside another "file" at some
  specified offset.
*/

#include "NestedReader.hpp"
#include "FileException.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "FileReader.hpp"
#include "FileWriter.hpp"
#include "DirUtil.hpp"
#include "Array.hpp"
#include <iostream>
#endif
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


NestedReader::NestedReader( shared_ptr< Reader > spReader,
                            int offset, int size,
                            const string & name )
    :   m_spReader( spReader ),
        m_offset( offset ),
        m_size( size ),
        m_name( name == ""  ?  "(nested reader)"  :  name )
{
    int parentSize = spReader->Seek( 0, RandomAccess::End );
    if ( (m_offset < 0) || (m_offset + m_size) > parentSize )
        throw FileOpenException( m_name, File::ReadMode );
    m_curPos = spReader->Seek( m_offset );
}

//=============================================================================

int
NestedReader::Seek( int offset, Origin origin )
{
    const int bases[] = { m_offset, m_curPos, m_offset + m_size };
    int loc = bases[ origin ] + offset;
    if ( (loc < m_offset) || (loc > m_offset + m_size) )
        throw FileSeekException( m_name, offset, origin );
    m_curPos = m_spReader->Seek( loc );
    return m_curPos - m_offset;
}

//=============================================================================

void
NestedReader::Read( char * buffer, int bufferSize )
{
    Assert( m_curPos >= m_offset );
    if ( m_curPos + bufferSize > m_offset + m_size )
        throw FileReadException( m_name, bufferSize );
    m_spReader->Read( buffer, bufferSize );
    m_curPos = m_spReader->Seek( 0, RandomAccess::Current );
}

//=============================================================================

#ifdef DEBUG

bool
NestedReader::Test( )
{
    bool ok = true;
    cout << "Testing NestedReader" << endl;

    try
    {
        const string fileName = "TestFile.dat";
        {
            FileWriter writer( fileName );
            for ( int i = 0; i < 20; ++i )
                writer.Write( i );
        }
        {
            shared_ptr< Reader > spReader( new FileReader( fileName ) );

            cout << "NestedReader constructor" << endl;
            NestedReader nested1( spReader, (int) (10 * sizeof( int )),
                                  (int) (5 * sizeof( int )), "Nested1" );
            int ii;
            for ( int i = 0; i < 5; ++i )
            {
                cout << "Read( int *)" << endl;
                nested1.Read( &ii );
                TESTCHECK( ii, i + 10, &ok );
            }
            try
            {
                cout << "Read( int *)" << endl;
                nested1.Read( &ii );
                cout << "Read should have thrown an exception." << endl;
            }
            catch ( FileReadException & except )
            {
                cout << "Exception here is OK" << endl;
                cout << except.Description() << endl;
            }
            cout << "Seek( ..., End )" << endl;
            nested1.Seek( -5 * (int) sizeof( int ), RandomAccess::End );
            cout << "Read( int *)" << endl;
            nested1.Read( &ii );
            TESTCHECK( ii, 10, &ok );
            cout << "Seek( ..., Current )" << endl;
            nested1.Seek( 2 * (int) sizeof( int ), RandomAccess::Current );
            cout << "Read( int *)" << endl;
            nested1.Read( &ii );
            TESTCHECK( ii, 13, &ok );
            cout << "Seek( ..., Beginning )" << endl;
            nested1.Seek( 4 * (int) sizeof( int ), RandomAccess::Beginning );
            cout << "Read( int *)" << endl;
            nested1.Read( &ii );
            TESTCHECK( ii, 14, &ok );

            try
            {
                cout << "NestedReader constructor" << endl;
                NestedReader nested2( spReader, (int) (15 * sizeof( int )),
                                      (int) (10 * sizeof( int )), "Nested2" );
                cout << "Constructor should have thrown an exception." << endl;
            }
            catch ( FileOpenException & except )
            {
                cout << "Exception here is OK" << endl;
                cout << except.Description() << endl;
            }
        }
        DeleteFile( fileName );
    }
    catch ( FileException & except )
    {
        cout << except.Description() << endl;
        ok = false;
    }
    
    if ( ok )
        cout << "NestedReader PASSED." << endl << endl;
    else
        cout << "NestedReader FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta
