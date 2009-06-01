#ifndef TIMER_HPP
#define TIMER_HPP
/*
  Timer.hpp
  Copyright (C) 2007 David M. Anderson

  Timer class: Tracks time elapsed and can be reset, paused, and resumed.
  NOTES:
  1. A Timer uses an underlying TimeSource (which could be another Timer) as
     its basis. By default this is the RealTicker source.
  2. Timers can be used for relatively short time-scale measurements, such as
     the time elapsed between graphics or animation frames, or for such
     purposes as establishing a game-time that runs continuously except when
     the game is paused for menus, etc.
*/


#include "TimeSource.hpp"
#include "StdClock.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Timer
    :   public TimeSource
{
public:
    Timer( const TimeSource * basis = &(StdClock::Instance()) );
    virtual ~Timer( );

    virtual double Seconds( ) const;
    virtual double MaxSeconds( ) const;
    void Reset( );
    void Pause( );
    void Resume( );

#ifdef DEBUG
    static bool Test( );
#endif

private:
    const TimeSource * m_basis;
    double m_start;
    double m_pause;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //TIMER_HPP


