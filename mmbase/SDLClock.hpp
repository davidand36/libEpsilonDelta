#ifndef SDLCLOCK_HPP
#define SDLCLOCK_HPP
/*
  SDLClock.hpp
  Copyright (C) 2007 David M. Anderson

  SDLClock class: A time source giving the real time elapsed, based on the
  Simple DirectMedia Layer (SDL) library.
  NOTES:
  1. Init() needs to be called before making any other calls.
  2. Shutdown() is optional; it will be called automatically if needed.
  3. Seconds() gives the time since Init() is called.
  4. The theoretical minimum granularity is 1 millisecond. In practice it is
     probably more.
  5. It can be used to time intervals of up to 49 days.
*/


#ifdef USE_SDL


#include "TimeSource.hpp"
#include "StdInt.hpp"
#include "Singleton.hpp"
#include <limits>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class SDLClock
    :   public TimeSource,
        public Singleton< SDLClock >
{
public:
    void Init( );
    void Shutdown( );

    virtual double Seconds( ) const;
    virtual double MaxSeconds( ) const;

#ifdef DEBUG
    static bool Test( );
#endif

private:
    SDLClock( );
    virtual ~SDLClock( );

    friend class Singleton< SDLClock >;
};


//*****************************************************************************


inline 
double 
SDLClock::MaxSeconds( ) const
{
    return  (std::numeric_limits< uint32_t >::max() / 1000.);
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SDL
#endif //SDLCLOCK_HPP
