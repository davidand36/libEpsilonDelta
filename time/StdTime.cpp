/*
  StdTime.cpp
  Copyright (C) 2007 David M. Anderson

  StdTime class: A time source giving the real time elapsed, based on the
  std::time() function.
*/


#include "StdTime.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


StdTime::StdTime( )
{
}

//-----------------------------------------------------------------------------

StdTime::~StdTime( )
{
}

//=============================================================================

double 
StdTime::Seconds( ) const
{
    return static_cast< double >( std::time( 0 ) );
}

//=============================================================================

#ifdef DEBUG

bool 
StdTime::Test( )
{
    bool ok = true;
    cout << "Testing StdTime" << endl;

    cout << "MaxSeconds(): " << StdTime::Instance().MaxSeconds() << endl;

    double now = StdTime::Instance().Seconds( );
    std::streamsize oldPrecision = std::cout.precision( 12 );
    cout << "now = StdTime::Instance().Seconds( ): " << now << endl;
    std::cout.precision( oldPrecision );
    TESTCHECK( (now >= 0.), true, &ok );
    cout << "Waiting for 15 seconds..." << endl;
    while ( StdTime::Instance().SecondsSince( now ) < 15. )
        ;
    TESTCHECKF( StdTime::Instance().Seconds( ), (now + 15.), &ok );

    double start = StdTime::Instance().Seconds( );
    int numTicks = 0;
    double old = start;
    do
    {
        now = StdTime::Instance().Seconds( );
        if ( old != now )
        {
            ++numTicks;
            old = now;
        }
    } while ( now < start + 10. );
    cout << "Ticks per second (est.): " << (numTicks / 10.) << endl;

    if ( ok )
        cout << "StdTime PASSED." << endl << endl;
    else
        cout << "StdTime FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

