#ifndef ISLAMICCALENDAR_HPP
#define ISLAMICCALENDAR_HPP
/*
  IslamicCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class IslamicCalendar, which defines the Islamic calendar.

  This calendar, used by the majority of the Moslem world, is usually
  attributed to Caliph Umar I, in A.H. 17 (A.D. 639). It is a strictly lunar
  calendar. Its epoch (A.H., Anno Hegirae) is JD 1,948,440,
  the year of the Hijra, Mohammed's emigration to Medina.
  Days begin at sunset.
  NOTES:
  1. A wide variety of systems have been suggested or adopted for 
     the Islamic calendar; it is in fact misleading to refer to it as a
     single calendar. Some of these are implemented here:
     i) Several purely arithmetic systems exist. The ones given here are the
        most common.
     ii) The Umm al-Qura calendar is used in Saudia Arabia and neighboring
        states on the Arabian Peninsula. It has changed at least twice, and
        simply compares the New Moon or moonset times to sunset.
     iii) The Islamic Society of North America adopted a simple rule
        recommended by the Fiqh Council of North American, 10 June 2006.
*/


#include "GeodeticLocation.hpp"
#include <tr1/memory>
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


extern GeodeticLocation Mecca;


//*****************************************************************************


class IslamicCalendar
{                                                             //IslamicCalendar
public:
    static void JulianDayToDMY( long julianDay,
                                int * pDay, int * pMonth, long * pYear );
    static long DMYToJulianDay( int day, int month, long year );
    static int MonthsInYear( long year );
    static int DaysInMonth( int month, long year );
    static const std::string & MonthName( int month, long year );
    static const std::string & MonthName( int month );
    static bool IsLeapYear( long year );

    enum EMonth
    { Muharram = 1, Safar, RabiI, RabiII, JumadaI, JumadaII,
      Rajab, Shaban, Ramadan, Shawwal, DhuAlQada, DhuAlHijja };

//-----------------------------------------------------------------------------

    class MonthFunc;

//-----------------------------------------------------------------------------

    class System
    {
    public:
        virtual ~System( ) { }

    protected:
        virtual void JulianDayToDMY( long julianDay,
                                     int * pDay, int * pMonth, long * pYear ) = 0;
        virtual long DMYToJulianDay( int day, int month, long year ) = 0;
        virtual int DaysInMonth( int month, long year ) = 0;
        virtual bool IsLeapYear( long year ) = 0;

        friend class IslamicCalendar;
    };

//.............................................................................

    class ArithmeticSystem
        :   public System
    {
    public:
        enum ELeapSequence
        {
            Nizari,     // 2, 5, 7, 10, 13, 16, 18, 21, 24, 26, 29
            Labban     // 2, 5, 7, 10, 13, 15, 18, 21, 24, 26, 29
//            Bohra,      // 2, 5, 8, 10, 13, 16, 19, 21, 24, 27, 29
//            AlHasib,    // 2, 5, 8, 11, 13, 16, 19, 21, 24, 27, 30
        };

        ArithmeticSystem( ELeapSequence leapSequence );
        void SetLeapSequence( ELeapSequence leapSequence );
        ELeapSequence LeapSequence( ) const;

    protected:
        virtual void JulianDayToDMY( long julianDay,
                                     int * pDay, int * pMonth, long * pYear );
        virtual long DMYToJulianDay( int day, int month, long year );
        virtual int DaysInMonth( int month, long year );
        virtual bool IsLeapYear( long year );

    private:
        ELeapSequence   m_leapSequence;
    };

//.............................................................................

    class AstronomicalSystem
        :   public System
    {
    public:
        AstronomicalSystem( std::tr1::shared_ptr< MonthFunc > pMonthFunc );
        void SetMonthFunc( std::tr1::shared_ptr< MonthFunc > pMonthFunc );
        std::tr1::shared_ptr< MonthFunc > GetMonthFunc( ) const;

    protected:
        virtual void JulianDayToDMY( long julianDay,
                                     int * pDay, int * pMonth, long * pYear );
        virtual long DMYToJulianDay( int day, int month, long year );
        virtual int DaysInMonth( int month, long year );
        virtual bool IsLeapYear( long year );

    private:
        long MonthStart( long julianDay );

        std::tr1::shared_ptr< MonthFunc >   m_pMonthFunc;
    };

//-----------------------------------------------------------------------------

    class MonthFunc
    {
    public:
        virtual ~MonthFunc( ) { }
        virtual bool operator()( long julianDay ) = 0;
    };

//.............................................................................

    class ISNA_Hijri
        :   public MonthFunc
    {
    public:
        virtual bool operator()( long julianDay );
    };

//.............................................................................

    typedef bool (*VisibilityFunc)( double julianDay,
                                    const GeodeticLocation & location );

    class LocalMonthFunc
        :   public MonthFunc
    {
    public:
        LocalMonthFunc( VisibilityFunc visibilityFunc,
                        const GeodeticLocation & location = Mecca );
        void SetVisibilityFunc( VisibilityFunc visibilityFunc );
        VisibilityFunc GetVisibilityFunc( ) const;
        void SetLocation( const GeodeticLocation & location );
        const GeodeticLocation & Location( ) const;
        virtual bool operator()( long julianDay );

    private:
        VisibilityFunc  m_visibilityFunc;
        GeodeticLocation    m_location;
    };

//-----------------------------------------------------------------------------

    static bool UmmAlQuraVisibility( double julianDay,
                                     const GeodeticLocation & location );

//-----------------------------------------------------------------------------

    static void SetSystem( std::tr1::shared_ptr< System > pSystem );
    std::tr1::shared_ptr< System > GetSystem( );

//=============================================================================

private:
    static std::tr1::shared_ptr< System >    ms_pSystem;
};                                                            //IslamicCalendar


//#############################################################################


inline
int
IslamicCalendar::MonthsInYear( long /*year*/ )
{
    return 12;
}

//=============================================================================

inline 
void 
IslamicCalendar::SetSystem( std::tr1::shared_ptr< System > pSystem )
{
    ms_pSystem = pSystem;
}

//-----------------------------------------------------------------------------

inline 
std::tr1::shared_ptr< IslamicCalendar::System > 
IslamicCalendar::GetSystem( )
{
    return ms_pSystem;
}


//*****************************************************************************


inline 
IslamicCalendar::ArithmeticSystem::ArithmeticSystem(
    ELeapSequence leapSequence )
    :   m_leapSequence( leapSequence )
{
}

//-----------------------------------------------------------------------------

inline
void 
IslamicCalendar::ArithmeticSystem::SetLeapSequence(
    ELeapSequence leapSequence )
{
    m_leapSequence = leapSequence;
}

//-----------------------------------------------------------------------------

inline
IslamicCalendar::ArithmeticSystem::ELeapSequence 
IslamicCalendar::ArithmeticSystem::LeapSequence( ) const
{
    return m_leapSequence;
}


//*****************************************************************************


inline
IslamicCalendar::AstronomicalSystem::AstronomicalSystem(
    std::tr1::shared_ptr< MonthFunc > pMonthFunc )
    :   m_pMonthFunc( pMonthFunc )
{
}

//-----------------------------------------------------------------------------

inline
void
IslamicCalendar::AstronomicalSystem::SetMonthFunc(
    std::tr1::shared_ptr< MonthFunc > pMonthFunc )
{
    m_pMonthFunc = pMonthFunc;
}

//-----------------------------------------------------------------------------

inline
std::tr1::shared_ptr< IslamicCalendar::MonthFunc > 
IslamicCalendar::AstronomicalSystem::GetMonthFunc( ) const
{
    return m_pMonthFunc;
}


//*****************************************************************************


inline
IslamicCalendar::LocalMonthFunc::LocalMonthFunc(
    VisibilityFunc visibilityFunc,
    const GeodeticLocation & location )
    :   m_visibilityFunc( visibilityFunc ),
        m_location( location )
{
}

//-----------------------------------------------------------------------------

inline
void 
IslamicCalendar::LocalMonthFunc::SetVisibilityFunc(
    VisibilityFunc visibilityFunc )
{
    m_visibilityFunc = visibilityFunc;
}

//-----------------------------------------------------------------------------

inline
IslamicCalendar::VisibilityFunc 
IslamicCalendar::LocalMonthFunc::GetVisibilityFunc( ) const
{
    return m_visibilityFunc;
}

//=============================================================================

inline
void 
IslamicCalendar::LocalMonthFunc::SetLocation(
    const GeodeticLocation & location )
{
    m_location = location;
}

//-----------------------------------------------------------------------------

inline
const GeodeticLocation & 
IslamicCalendar::LocalMonthFunc::Location( ) const
{
    return m_location;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ISLAMICCALENDAR_HPP
