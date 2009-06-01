/*
  Timer.cpp
  Copyright (C) 2007 David M. Anderson

  Timer class: Tracks time elapsed and can be reset, paused, and resumed.
*/


#include "Timer.hpp"
#include "Assert.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Timer::Timer( const TimeSource * basis )
    :   m_basis( basis ),
        m_start( basis->Seconds() ),
        m_pause( 0. )
{
    Assert( basis );
}

//-----------------------------------------------------------------------------

Timer::~Timer( )
{
}

//=============================================================================

double
Timer::Seconds( ) const
{
    if ( m_pause > 0. )
    {
        double diff = m_pause - m_start;
        if ( diff >= 0. )
            return diff;
        else
            return diff + m_basis->MaxSeconds( );
    }
    return m_basis->SecondsSince( m_start );
}

//=============================================================================

double 
Timer::MaxSeconds( ) const
{
    return m_basis->MaxSeconds( );
}

//=============================================================================

void 
Timer::Reset( )
{
    m_start = m_basis->Seconds( );
    m_pause = 0.;
}

//-----------------------------------------------------------------------------

void 
Timer::Pause( )
{
    if ( m_pause != 0. )
        return;
    m_pause = m_basis->Seconds( );
}

//-----------------------------------------------------------------------------

void 
Timer::Resume( )
{
    if ( m_pause == 0. )
        return;
    m_start += m_basis->SecondsSince( m_pause );
    if ( m_start > m_basis->MaxSeconds( ) )
        m_start -= m_basis->MaxSeconds( );
    m_pause = 0.;
}

//=============================================================================

#ifdef DEBUG

bool
Timer::Test( )
{
    bool ok = true;
    cout << "Testing Timer" << endl;

    cout << "Timer realTime" << endl;
    Timer realTime;
    cout << "Timer appTime" << endl;
    Timer appTime;
    cout << "Timer timer0" << endl;
    Timer timer0;
    cout << "Timer timer1( &appTime )" << endl;
    Timer timer1( &appTime );
    cout << "Timer delayTimer" << endl;
    Timer delayTimer;

    cout << "wait until 0.75" << endl;
    while ( realTime.Seconds() < 0.75 )
        ;
    TESTCHECKF( realTime.Seconds(), 0.75, &ok );
    TESTCHECKF( appTime.Seconds(), 0.75, &ok );
    TESTCHECKF( timer0.Seconds(), 0.75, &ok );
    TESTCHECKF( timer1.Seconds(), 0.75, &ok );

    cout << "timer0.Pause()" << endl;
    timer0.Pause( );
    cout << "wait until 1.25" << endl;
    while ( realTime.Seconds() < 1.25 )
        ;
    TESTCHECKF( realTime.Seconds(), 1.25, &ok );
    TESTCHECKF( appTime.Seconds(), 1.25, &ok );
    TESTCHECKF( timer0.Seconds(), 0.75, &ok );
    TESTCHECKF( timer1.Seconds(), 1.25, &ok );
    cout << "timer0.Resume()" << endl;
    timer0.Resume( );
    TESTCHECKF( realTime.Seconds(), 1.25, &ok );
    TESTCHECKF( appTime.Seconds(), 1.25, &ok );
    TESTCHECKF( timer0.Seconds(), 0.75, &ok );
    TESTCHECKF( timer1.Seconds(), 1.25, &ok );
    
    cout << "appTime.Pause()" << endl;
    appTime.Pause( );
    cout << "delay 1s" << endl;
    delayTimer.Reset( );
    while ( delayTimer.Seconds() < 1.0 )
        ;
    cout << "appTime.Resume()" << endl;
    appTime.Resume( );
    TESTCHECKF( realTime.Seconds(), 2.25, &ok );
    TESTCHECKF( appTime.Seconds(), 1.25, &ok );
    TESTCHECKF( timer0.Seconds(), 1.75, &ok );
    TESTCHECKF( timer1.Seconds(), 1.25, &ok );
    
    cout << "timer1.Pause()" << endl;
    timer1.Pause( );
    cout << "wait until 2.5" << endl;
    while ( realTime.Seconds() < 2.5 )
        ;
    TESTCHECKF( realTime.Seconds(), 2.5, &ok );
    TESTCHECKF( appTime.Seconds(), 1.5, &ok );
    TESTCHECKF( timer0.Seconds(), 2.0, &ok );
    TESTCHECKF( timer1.Seconds(), 1.25, &ok );
    cout << "timer1.Resume()" << endl;
    timer1.Resume( );
    TESTCHECKF( timer1.Seconds(), 1.25, &ok );

    cout << "delay 0.5s" << endl;
    delayTimer.Reset( );
    while ( delayTimer.Seconds() < 0.5 )
        ;
    TESTCHECKF( realTime.Seconds(), 3.0, &ok );
    TESTCHECKF( appTime.Seconds(), 2.0, &ok );
    TESTCHECKF( timer0.Seconds(), 2.5, &ok );
    TESTCHECKF( timer1.Seconds(), 1.75, &ok );
            
    if ( ok )
        cout << "Timer PASSED." << endl << endl;
    else
        cout << "Timer FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta
