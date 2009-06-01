#ifndef STDCLOCK_HPP
#define STDCLOCK_HPP
/*
  StdClock.hpp
  Copyright (C) 2007 David M. Anderson

  StdClock class: A time source giving the real time elapsed, based on the
  std::clock() function.
  NOTES:
  1. The theoretical minimum granularity is 1 microsecond. In practice it is
     probably more.
  2. On many systems this cannot be used to time intervals longer than
     about 35 minutes.
*/


#include "TimeSource.hpp"
#include "StdInt.hpp"
#include "Singleton.hpp"
#include <limits>
#include <ctime>

namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class StdClock
    :   public TimeSource,
        public Singleton< StdClock >
{
public:
    virtual double Seconds( ) const;
    virtual double MaxSeconds( ) const;

#ifdef DEBUG
    static bool Test( );
#endif

private:
    StdClock( );
    virtual ~StdClock( );

    friend class Singleton< StdClock >;
};


//*****************************************************************************


inline 
double 
StdClock::MaxSeconds( ) const
{
    return (std::numeric_limits< std::clock_t >::max() / CLOCKS_PER_SEC);
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //STDCLOCK_HPP
