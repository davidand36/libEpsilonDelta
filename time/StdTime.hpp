#ifndef STDTIME_HPP
#define STDTIME_HPP
/*
  StdTime.hpp
  Copyright (C) 2007 David M. Anderson

  StdTime class: A time source giving the real time elapsed, based on the
  std::time() function.
  NOTES:
  1. The granularity of this time source is 1 second, quite coarse.
  2. On the other hand, it can be used to time intervals as long as 68 years.
*/


#include "TimeSource.hpp"
#include "StdInt.hpp"
#include "Singleton.hpp"
#include <limits>
#include <ctime>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class StdTime
    :   public TimeSource,
        public Singleton< StdTime >
{
public:
    virtual double Seconds( ) const;
    virtual double MaxSeconds( ) const;

#ifdef DEBUG
    static bool Test( );
#endif

private:
    StdTime( );
    virtual ~StdTime( );

    friend class Singleton< StdTime >;
};


//*****************************************************************************


inline 
double 
StdTime::MaxSeconds( ) const
{
    return (double)std::numeric_limits< std::time_t >::max();
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //STDTIME_HPP
