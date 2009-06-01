#ifndef TIMESOURCE_HPP
#define TIMESOURCE_HPP
/*
  TimeSource.hpp
  Copyright (C) 2007 David M. Anderson

  TimeSource abstract base class: represents a source which measures the
  passage of time.
  NOTES:
  1. Seconds() returns the number of seconds since some possible arbitrary or
     user-specified origin. It will generally be monotonically increasing.
  2. Because the underlying source may wrap around after reaching an
     implementation-dependent maximum, the difference between one reading and
     a previous one might be negative. The function SecondsSince() corrects
     for this (possibly very rare) situation.
  3. Depending on the granularity of the time source, consecutive readings
     might return the same value. Of course this is also possible if the
     source is pausable.
     
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class TimeSource
{
public:
    virtual ~TimeSource( );
    virtual double Seconds( ) const = 0;
    virtual double SecondsSince( double start ) const;
    virtual double MaxSeconds( ) const = 0;
};


//*****************************************************************************


inline 
TimeSource::~TimeSource( )
{
}

//=============================================================================

inline 
double 
TimeSource::SecondsSince( double start ) const
{
    double now = Seconds( );
    double diff = now - start;
    if ( diff >= 0. )
        return diff;
    else
        return diff + MaxSeconds( );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta


#endif //TIMESOURCE_HPP
