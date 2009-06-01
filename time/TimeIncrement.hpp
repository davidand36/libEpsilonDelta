#ifndef TIMEINCREMENT_HPP
#define TIMEINCREMENT_HPP
/*
  TimeIncrement.hpp
  Copyright (C) 2007 David M. Anderson

  TimeIncrement class, representing the difference between two Time values.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class TimeIncrement
{
public:
    explicit TimeIncrement( int hours = 0, int minutes = 0,
                            double seconds = 0. );
    explicit TimeIncrement( double days );
    void Set( int hours = 0, int minutes = 0, double seconds = 0. );
    void Set( double days );
    void Normalize( );
    int Hours( ) const;
    int Minutes( ) const;
    double Seconds( ) const;
    double Days( ) const;
    TimeIncrement & operator+=( const TimeIncrement & rhs );
    TimeIncrement & operator-=( const TimeIncrement & rhs );
    TimeIncrement operator-( ) const;
#ifdef DEBUG
    static bool Test( );
#endif

private:
    int m_hours;
    int m_minutes;
    double m_seconds;
};

//.............................................................................

bool operator==( const TimeIncrement & lhs,
                 const TimeIncrement & rhs );
bool operator<( const TimeIncrement & lhs,
                 const TimeIncrement & rhs );
TimeIncrement operator+( const TimeIncrement & lhs,
                         const TimeIncrement & rhs );
TimeIncrement operator-( const TimeIncrement & lhs,
                         const TimeIncrement & rhs );


//*****************************************************************************


inline
TimeIncrement::TimeIncrement( int hours, int minutes, double seconds )
    :    m_hours( hours ),
         m_minutes( minutes ),
         m_seconds( seconds )
{
}

//=============================================================================

inline
void
TimeIncrement::Set( int hours, int minutes, double seconds )
{
    m_hours = hours;
    m_minutes = minutes;
    m_seconds = seconds;
}

//=============================================================================

inline
int
TimeIncrement::Hours( ) const
{
    return m_hours;
}

//-----------------------------------------------------------------------------

inline
int
TimeIncrement::Minutes( ) const
{
    return m_minutes;
}

//-----------------------------------------------------------------------------

inline
double
TimeIncrement::Seconds( ) const
{
    return m_seconds;
}

//-----------------------------------------------------------------------------

inline
double
TimeIncrement::Days( ) const
{
    return ( (m_hours + (m_minutes + m_seconds / 60.) / 60.) / 24. );
}

//=============================================================================

inline
TimeIncrement &
TimeIncrement::operator+=( const TimeIncrement & rhs )
{
    m_hours += rhs.m_hours;
    m_minutes += rhs.m_minutes;
    m_seconds += rhs.m_seconds;
    return *this;
}

//-----------------------------------------------------------------------------

inline
TimeIncrement &
TimeIncrement::operator-=( const TimeIncrement & rhs )
{
    m_hours -= rhs.m_hours;
    m_minutes -= rhs.m_minutes;
    m_seconds -= rhs.m_seconds;
    return *this;
}

//-----------------------------------------------------------------------------

inline
TimeIncrement 
TimeIncrement::operator-( ) const
{
    return TimeIncrement( - m_hours, - m_minutes, - m_seconds );
}

//=============================================================================

inline 
bool 
operator==( const TimeIncrement & lhs,
            const TimeIncrement & rhs )
{
    return ( (lhs.Hours() == rhs.Hours()) && (lhs.Minutes() == rhs.Minutes())
             && (lhs.Seconds() == rhs.Seconds()) );
}

//-----------------------------------------------------------------------------

inline 
bool 
operator<( const TimeIncrement & lhs,
           const TimeIncrement & rhs )
{
    return( (lhs.Hours() < rhs.Hours())
            || ((lhs.Hours() == rhs.Hours())
                && (lhs.Minutes() < rhs.Minutes()))
            || ((lhs.Hours() == rhs.Hours())
                && (lhs.Minutes() == rhs.Minutes())
                && (lhs.Seconds() < rhs.Seconds())) );
}

//=============================================================================

inline
TimeIncrement
operator+( const TimeIncrement & lhs, const TimeIncrement & rhs )
{
    TimeIncrement sum = lhs;
    return (sum += rhs);
}

//-----------------------------------------------------------------------------

inline
TimeIncrement
operator-( const TimeIncrement & lhs, const TimeIncrement & rhs )
{
    TimeIncrement diff = lhs;
    return (diff -= rhs);
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //TIMEINCREMENT_HPP
