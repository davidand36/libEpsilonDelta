#ifndef DATEJD_HPP
#define DATEJD_HPP
/*
  DateJD.hpp
  Copyright (C) 2007 David M. Anderson

  DateJD class, representing dates with a Julian Day equivalent,
  i.e. a base class for all dates.

  Julian Day numbering was developed by John F.W. Herschel in 1849 in his
  "Outlines of Astronomy", and counts the days since
  (noon UT) 1 January 4713 B.C., the epoch established
  by Joseph Justus Scaliger in 1583 in his "De emendatione temporum".
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class DateJD
{
public:
    explicit DateJD( bool today = false );
    explicit DateJD( int julianDay );
    virtual ~DateJD( );
    virtual void Set( bool today );
    virtual void Set( int julianDay );
    virtual bool Valid( ) const;
    virtual int JulianDay( ) const;
    virtual void Increment( int days );
#ifdef DEBUG
    static bool Test( );
#endif

    static const int INVALID = -1000000000;

protected:
    mutable int m_julianDay; /*allows for lazy evaluation in derived classes*/

private:
    static int Today( );
};

//.............................................................................

bool operator==( const DateJD & lhs, const DateJD & rhs );
bool operator<( const DateJD & lhs, const DateJD & rhs );


//*****************************************************************************


inline
DateJD::DateJD( bool today )
    :    m_julianDay( today ? Today( ) : INVALID )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
DateJD::DateJD( int julianDay )
    :    m_julianDay( julianDay )
{
}

//-----------------------------------------------------------------------------

inline
DateJD::~DateJD( )
{
}

//=============================================================================

inline
void
DateJD::Set( int julianDay )
{
    m_julianDay = julianDay;
}

//=============================================================================

inline
bool
DateJD::Valid( ) const
{
    return ( m_julianDay != INVALID );
}

//=============================================================================

inline
int
DateJD::JulianDay( ) const
{
    return m_julianDay;
}

//=============================================================================

inline
void
DateJD::Increment( int days )
{
    if ( m_julianDay != INVALID )
        m_julianDay += days;
}

//=============================================================================

inline 
bool
operator==( const DateJD & lhs, const DateJD & rhs )
{
    return ( lhs.JulianDay( ) == rhs.JulianDay( ) );
}

//-----------------------------------------------------------------------------

inline 
bool
operator<( const DateJD & lhs, const DateJD & rhs )
{
    return ( lhs.JulianDay( ) < rhs.JulianDay( ) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //DATEJD_HPP
