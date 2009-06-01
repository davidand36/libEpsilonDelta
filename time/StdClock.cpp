/*
  StdClock.cpp
  Copyright (C) 2007 David M. Anderson

  StdClock class: A time source giving the real time elapsed, based on the
  std::clock() function.
*/


#include "StdClock.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


StdClock::StdClock( )
{
}

//-----------------------------------------------------------------------------

StdClock::~StdClock( )
{
}

//=============================================================================

double 
StdClock::Seconds( ) const
{
    return  static_cast< double >( std::clock( ) ) / CLOCKS_PER_SEC;
}

//=============================================================================

#ifdef DEBUG

bool 
StdClock::Test( )
{
    bool ok = true;
    cout << "Testing StdClock" << endl;

    cout << "MaxSeconds(): " << StdClock::Instance().MaxSeconds() << endl;

    double now = StdClock::Instance().Seconds( );
    cout << "now = StdClock::Instance().Seconds( ): " << now << endl;
    TESTCHECK( (now >= 0.), true, &ok );
    //This test should be run within an hour of the test program's  beginning.
    TESTCHECK( (now < 3600.), true, &ok );  
    cout << "Waiting for 1.5 seconds..." << endl;
    while ( StdClock::Instance().SecondsSince( now ) < 1.5 )
        ;
    TESTCHECKF( StdClock::Instance().Seconds( ), (now + 1.5), &ok );

    double start = StdClock::Instance().Seconds( );
    int numTicks = 0;
    double old = start;
    do
    {
        now = StdClock::Instance().Seconds( );
        if ( old != now )
        {
            ++numTicks;
            old = now;
        }
    } while ( now < start + 10. );
    cout << "Ticks per second (est.): " << (numTicks / 10.) << endl;

    if ( ok )
        cout << "StdClock PASSED." << endl << endl;
    else
        cout << "StdClock FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

