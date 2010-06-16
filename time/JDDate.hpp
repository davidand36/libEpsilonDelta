#ifndef JDDATE_HPP
#define JDDATE_HPP
/*
  JDDate.hpp
  Copyright (C) 2007 David M. Anderson

  JDDate class, representing dates with a Julian Day equivalent,
  i.e. a base class for all dates.

  Julian Day numbering was developed by John F.W. Herschel in 1849 in his
  "Outlines of Astronomy", and counts the days since
  (noon UT) 1 January 4713 B.C., the epoch established
  by Joseph Justus Scaliger in 1583 in his "De emendatione temporum".
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class JDDate
{
public:
    explicit JDDate( bool today = false );
    explicit JDDate( long julianDay );
    virtual ~JDDate( );
    virtual void Set( bool today );
    virtual void Set( long julianDay );
    virtual bool Valid( ) const;
    virtual long JulianDay( ) const;
    virtual void Increment( long days );
#ifdef DEBUG
    static bool Test( );
#endif

    static const long INVALID = -10000000000000;

protected:
    mutable long m_julianDay; /*allows for lazy evaluation in derived classes*/

private:
    static long Today( );
};

//.............................................................................

bool operator==( const JDDate & lhs, const JDDate & rhs );
bool operator<( const JDDate & lhs, const JDDate & rhs );


//*****************************************************************************


inline
JDDate::JDDate( bool today )
    :    m_julianDay( today ? Today( ) : INVALID )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
JDDate::JDDate( long julianDay )
    :    m_julianDay( julianDay )
{
}

//-----------------------------------------------------------------------------

inline
JDDate::~JDDate( )
{
}

//=============================================================================

inline
void
JDDate::Set( long julianDay )
{
    m_julianDay = julianDay;
}

//=============================================================================

inline
bool
JDDate::Valid( ) const
{
    return ( m_julianDay != INVALID );
}

//=============================================================================

inline
long
JDDate::JulianDay( ) const
{
    return m_julianDay;
}

//=============================================================================

inline
void
JDDate::Increment( long days )
{
    if ( m_julianDay != INVALID )
        m_julianDay += days;
}

//=============================================================================

inline 
bool
operator==( const JDDate & lhs, const JDDate & rhs )
{
    return ( lhs.JulianDay( ) == rhs.JulianDay( ) );
}

//-----------------------------------------------------------------------------

inline 
bool
operator<( const JDDate & lhs, const JDDate & rhs )
{
    return ( lhs.JulianDay( ) < rhs.JulianDay( ) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //JDDATE_HPP
