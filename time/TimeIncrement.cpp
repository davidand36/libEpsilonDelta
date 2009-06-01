/*
  TimeIncrement.cpp
  Copyright (C) 2007 David M. Anderson

  TimeIncrement class, representing the difference between two Time values.
*/


#include "TimeIncrement.hpp"
#include "Assert.hpp"
#include <cmath>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <cstdio>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


TimeIncrement::TimeIncrement( double days )
{
    Set( days );
}

//=============================================================================

void
TimeIncrement::Set( double days )
{
    days *= 24.;
    double whole;
    double fraction = modf( days, &whole );
    m_hours = static_cast<int>( whole );
    fraction *= 60.;
    fraction = modf( fraction, &whole );
    m_minutes = static_cast<int>( whole );
    m_seconds = fraction * 60.;
    Normalize( );
}

//=============================================================================

void
TimeIncrement::Normalize( )
{
    while ( m_seconds <= -60. )
    {
        m_seconds += 60.;
        --m_minutes;
    }
    while ( m_seconds >= 60. )
    {
        m_seconds -= 60.;
        ++m_minutes;
    }
    while ( m_minutes <= -60 )
    {
        m_minutes += 60;
        --m_hours;
    }
    while ( m_minutes >= 60 )
    {
        m_minutes -= 60;
        ++m_hours;
    }
    bool positive = true;
    if ( m_hours < 0 )
        positive = false;
    else if ( m_hours > 0 )
        positive = true;
    else if ( m_minutes < 0 )
        positive = false;
    else if ( m_minutes > 0 )
        positive = true;
    else if ( m_seconds < 0. )
        positive = false;
    if ( positive )
    {
        while ( m_seconds < 0. )
        {
            m_seconds += 60.;
            --m_minutes;
        }
        while ( m_minutes < 0 )
        {
            m_minutes += 60;
            --m_hours;
        }
        Assert( m_hours >= 0 );
    }
    else
    {
        while ( m_seconds > 0. )
        {
            m_seconds -= 60.;
            ++m_minutes;
        }
        while ( m_minutes > 0 )
        {
            m_minutes -= 60;
            ++m_hours;
        }
        Assert( m_hours <= 0 );
    }
}

//=============================================================================

#ifdef DEBUG

bool
TimeIncrement::Test( )
{
    bool ok = true;
    cout << "Testing TimeIncrement" << endl;

    cout << "TimeIncrement() [default constructor]" << endl;
    TimeIncrement incr;
    TESTCHECK( incr.Hours( ), 0, &ok );
    TESTCHECK( incr.Minutes( ), 0, &ok );
    TESTCHECK( incr.Seconds( ), 0., &ok );
    TESTCHECK( incr.Days( ), 0., &ok );

    cout << "Normalize(): " << endl;
    incr.Normalize( );
    TESTCHECK( incr.Hours( ), 0, &ok );
    TESTCHECK( incr.Minutes( ), 0, &ok );
    TESTCHECK( incr.Seconds( ), 0., &ok );
    TESTCHECK( incr.Days( ), 0., &ok );

    double d = 0.05;
    cout << "Set(" << d << ") :" << endl;
    incr.Set( d );
    TESTCHECK( incr.Hours( ), 1, &ok );
    TESTCHECK( incr.Minutes( ), 12, &ok );
    TESTCHECKF( incr.Seconds( ), 0., &ok );
    TESTCHECKF( incr.Days( ), 0.05, &ok );

    cout << "Set( )" << endl;
    incr.Set( );
    TESTCHECK( incr.Hours( ), 0, &ok );
    TESTCHECK( incr.Minutes( ), 0, &ok );
    TESTCHECK( incr.Seconds( ), 0., &ok );
    TESTCHECK( incr.Days( ), 0., &ok );

    int h = 1; int m = 12; double s = 0.;
    cout << "Set(" << h << ", " << m << ", " << s << ") :" << endl;
    incr.Set( h, m, s );
    TESTCHECK( incr.Hours( ), 1, &ok );
    TESTCHECK( incr.Minutes( ), 12, &ok );
    TESTCHECK( incr.Seconds( ), 0., &ok );
    TESTCHECKF( incr.Days( ), 0.05, &ok );

    h = 1; m = 30; s = 0.;
    cout << "TimeIncrement(" << h << ", " << m << ", " << s
         << ") [h,m,s constructor]" << endl;
    TimeIncrement incrHMS( h, m, s );
    TESTCHECK( incrHMS.Hours( ), 1, &ok );
    TESTCHECK( incrHMS.Minutes( ), 30, &ok );
    TESTCHECK( incrHMS.Seconds( ), 0., &ok );
    TESTCHECK( incrHMS.Days( ), 1./16., &ok );

    cout << "incrSum = incr + incrHMS" << endl;
    TimeIncrement incrSum = incr + incrHMS;
    TESTCHECK( incrSum.Hours( ), 2, &ok );
    TESTCHECK( incrSum.Minutes( ), 42, &ok );
    TESTCHECK( incrSum.Seconds( ), 0., &ok );
    TESTCHECKF( incrSum.Days( ), 0.1125, &ok );

    cout << "incrDiff = incrSum - incrHMS" << endl;
    TimeIncrement incrDiff = incrSum - incrHMS;
    TESTCHECK( incrDiff.Hours( ), 1, &ok );
    TESTCHECK( incrDiff.Minutes( ), 12, &ok );
    TESTCHECK( incrDiff.Seconds( ), 0., &ok );
    TESTCHECKF( incrDiff.Days( ), 0.05, &ok );

    h = 0; m = -72; s = -30.1;
    cout << "Set(" << h << ", " << m << ", " << s << ") :" << endl;
    incr.Set( h, m, s );
    TESTCHECK( incr.Hours( ), 0, &ok );
    TESTCHECK( incr.Minutes( ), -72, &ok );
    TESTCHECKF( incr.Seconds( ), -30.1, &ok );
    TESTCHECKF( incr.Days( ), -0.0503484, &ok );

    cout << "Normalize(): " << endl;
    incr.Normalize( );
    TESTCHECK( incr.Hours( ), -1, &ok );
    TESTCHECK( incr.Minutes( ), -12, &ok );
    TESTCHECKF( incr.Seconds( ), -30.1, &ok );
    TESTCHECKF( incr.Days( ), -0.0503484, &ok );

    h = -1; m = -14; s = 89.9;
    cout << "Set(" << h << ", " << m << ", " << s << ") :" << endl;
    incr.Set( h, m, s );
    TESTCHECK( incr.Hours( ), -1, &ok );
    TESTCHECK( incr.Minutes( ), -14, &ok );
    TESTCHECKF( incr.Seconds( ), 89.9, &ok );
    TESTCHECKF( incr.Days( ), -0.0503484, &ok );

    cout << "Normalize(): " << endl;
    incr.Normalize( );
    TESTCHECK( incr.Hours( ), -1, &ok );
    TESTCHECK( incr.Minutes( ), -12, &ok );
    TESTCHECKF( incr.Seconds( ), -30.1, &ok );
    TESTCHECKF( incr.Days( ), -0.0503484, &ok );

    h = -2; m = 48; s = -30.1;
    cout << "Set(" << h << ", " << m << ", " << s << ") :" << endl;
    incr.Set( h, m, s );
    TESTCHECK( incr.Hours( ), -2, &ok );
    TESTCHECK( incr.Minutes( ), 48, &ok );
    TESTCHECKF( incr.Seconds( ), -30.1, &ok );
    TESTCHECKF( incr.Days( ), -0.0503484, &ok );

    cout << "Normalize(): " << endl;
    incr.Normalize( );
    TESTCHECK( incr.Hours( ), -1, &ok );
    TESTCHECK( incr.Minutes( ), -12, &ok );
    TESTCHECKF( incr.Seconds( ), -30.1, &ok );
    TESTCHECKF( incr.Days( ), -0.0503484, &ok );

    h = -1; m = 133; s = -29.9;
    cout << "Set(" << h << ", " << m << ", " << s << ") :" << endl;
    incr.Set( h, m, s );
    TESTCHECK( incr.Hours( ), -1, &ok );
    TESTCHECK( incr.Minutes( ), 133, &ok );
    TESTCHECKF( incr.Seconds( ), -29.9, &ok );
    TESTCHECKF( incr.Days( ), 0.0503484, &ok );

    cout << "Normalize(): " << endl;
    incr.Normalize( );
    TESTCHECK( incr.Hours( ), 1, &ok );
    TESTCHECK( incr.Minutes( ), 12, &ok );
    TESTCHECKF( incr.Seconds( ), 30.1, &ok );
    TESTCHECKF( incr.Days( ), 0.0503484, &ok );

    h = -1; m = 133; s = -29.9;
    cout << "Set(" << h << ", " << m << ", " << s << ") :" << endl;
    incr.Set( h, m, s );
    TESTCHECK( incr.Hours( ), -1, &ok );
    TESTCHECK( incr.Minutes( ), 133, &ok );
    TESTCHECKF( incr.Seconds( ), -29.9, &ok );
    TESTCHECKF( incr.Days( ), 0.0503484, &ok );

    cout << "incr = - incr :" << endl;
    incr = - incr;
    TESTCHECK( incr.Hours( ), 1, &ok );
    TESTCHECK( incr.Minutes( ), -133, &ok );
    TESTCHECKF( incr.Seconds( ), 29.9, &ok );
    TESTCHECKF( incr.Days( ), -0.0503484, &ok );

    cout << "Normalize(): " << endl;
    incr.Normalize( );
    TESTCHECK( incr.Hours( ), -1, &ok );
    TESTCHECK( incr.Minutes( ), -12, &ok );
    TESTCHECKF( incr.Seconds( ), -30.1, &ok );
    TESTCHECKF( incr.Days( ), -0.0503484, &ok );

    if ( ok )
        cout << "TimeIncrement PASSED." << endl << endl;
    else
        cout << "TimeIncrement FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG

//*****************************************************************************

}                                                      //namespace EpsilonDelta
