#ifndef MAYANLONGCOUNTDATE_HPP
#define MAYANLONGCOUNTDATE_HPP
/*
  MayanLongCountDate.hpp
  Copyright (C) 2007 David M. Anderson

  MayanLongCountDate class, which defines a date in the Mayan long count
  calendar.
*/


#include "JDDate.hpp"
#include "DateFixupMethod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MayanLongCountDate
    :   public JDDate
{
public:
    explicit MayanLongCountDate( bool today = false );
    explicit MayanLongCountDate( long julianDay );
    MayanLongCountDate( int kin, int uinal, int tun, int katun, int baktun,
                        int pictun = 0, int calabtun = 0,
                        int kinchiltun = 0, int alautun = 0 );
    explicit MayanLongCountDate( const JDDate & date );
    MayanLongCountDate( const MayanLongCountDate & date );
    virtual ~MayanLongCountDate( );

    MayanLongCountDate & operator=( const JDDate & rhs );
    MayanLongCountDate & operator=( const MayanLongCountDate & rhs );

    virtual void Set( bool today );
    virtual void Set( long julianDay );
    void Set( int kin, int uinal, int tun, int katun, int baktun,
              int pictun = 0, int calabtun = 0,
              int kinchiltun = 0, int alautun = 0 );

    virtual bool Valid( ) const;
    void MakeValid( DateFixup::EMethod fixupMethod = DateFixup::Clamp );

    virtual long JulianDay( ) const;
    int Kin( ) const;
    int Uinal( ) const;
    int Tun( ) const;
    int Katun( ) const;
    int Baktun( ) const;
    int Pictun( ) const;
    int Calabtun( ) const;
    int Kinchiltun( ) const;
    int Alautun( ) const;

    virtual void Increment( int days );
    void Increment( int kin, int uinal, int tun, int katun, int baktun,
                    int pictun = 0, int calabtun = 0,
                    int kinchiltun = 0, int alautun = 0,
                    DateFixup::EMethod fixupMethod = DateFixup::Carry );

    std::string ToString( const std::string & format
                          = DefaultFormat() ) const;
    
    static void SetDefaultFormat( const std::string & format );
    static const std::string & DefaultFormat( );

    typedef class MayanLongCountCalendar Calendar;


private:
    int m_kin;
    int m_uinal;
    int m_tun;
    int m_katun;
    int m_baktun;
    int m_pictun;
    int m_calabtun;
    int m_kinchiltun;
    int m_alautun;

    static std::string m_defaultFormat;
};

//.............................................................................

bool operator==( const MayanLongCountDate & lhs, const MayanLongCountDate & rhs );
bool operator<( const MayanLongCountDate & lhs, const MayanLongCountDate & rhs );

#ifdef DEBUG
bool TestMayanLongCountDate( );
#endif


//*****************************************************************************


inline 
int 
MayanLongCountDate::Kin( ) const
{
    return m_kin;
}

//-----------------------------------------------------------------------------

inline 
int 
MayanLongCountDate::Uinal( ) const
{
    return m_uinal;
}

//-----------------------------------------------------------------------------

inline 
int 
MayanLongCountDate::Tun( ) const
{
    return m_tun;
}

//-----------------------------------------------------------------------------

inline 
int 
MayanLongCountDate::Katun( ) const
{
    return m_katun;
}

//-----------------------------------------------------------------------------

inline 
int 
MayanLongCountDate::Baktun( ) const
{
    return m_baktun;
}

//-----------------------------------------------------------------------------

inline 
int 
MayanLongCountDate::Pictun( ) const
{
    return m_pictun;
}

//-----------------------------------------------------------------------------

inline 
int 
MayanLongCountDate::Calabtun( ) const
{
    return m_calabtun;
}

//-----------------------------------------------------------------------------

inline 
int 
MayanLongCountDate::Kinchiltun( ) const
{
    return m_kinchiltun;
}

//-----------------------------------------------------------------------------

inline 
int 
MayanLongCountDate::Alautun( ) const
{
    return m_alautun;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MAYANLONGCOUNTDATE_HPP
