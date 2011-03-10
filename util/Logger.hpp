#ifndef LOGGER_HPP
#define LOGGER_HPP
/*
  Logger.hpp
  Copyright (C) 2009 David M. Anderson

  Logger class: Selectively outputs error, warning, and other messages.
  NOTES:
  1. Log() and operator() are equivalent. Each has a version that takes
     a std::string, which may be assembled using IntToString(), other
     ToString() functions, boost::format, etc.
     Each also has a version that works like printf.
     Note that it takes a C string for the format.
  2. All messages with levels greater than the verbosity level will be ignored.
  3. The levels enumerated here are based on Syslog, with additional,
     more verbose debug levels. This class does not enforce the use of these
     levels; any ints may be used.
  4. The default verbosity level is Warning.
  5. The final disposition of the (formatted) message is determined by the
     Output function, which can be set by the user.
  6. By default, the Output function sends a string with the domain, the
     level as a string, and the message to a std::ostream.
  7. By default, the destination of messages is std::cerr for levels up through
     Warning, and std::cout for higher levels.
  8. SetDestination() sets the destination for all levels if level < 0.
  9. On Android, the default is to pass through to the native log facility.
*/


#include <string>
#include <iostream>
#include <cstdarg>
#include <tr1/memory>

namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Logger
{
public:
    class OutputFunc
    {
    public:
        virtual void operator()( const std::string & domain, int level,
                                 const std::string & message ) = 0;
    };

    enum Level { Emergency = 0, Alert = 1, Critical = 2,
                 Error = 3, Warning = 4, Notice = 5, Info = 6,
                 Debug = 7, Debug1 = 8, Debug2 = 9,
                 NumLevels };
    

    Logger( const std::string & domain,
            std::tr1::shared_ptr< OutputFunc > func
            = std::tr1::shared_ptr< OutputFunc >() );

    void operator()( int level, const std::string & message );
    void operator()( int level, const char * format, ... );
    void Log( int level, const std::string & message );
    void Log( int level, const char * format, ... );

    void SetVerbosity( int maxLevel );
    int GetVerbosity( ) const;

    void SetOutputFunc( std::tr1::shared_ptr< OutputFunc > func
                        = std::tr1::shared_ptr< OutputFunc >() );
    void SetOutputStream( std::ostream & dest, int level = -1 );

#ifdef DEBUG
    static bool Test( );
#endif

private:
    class OutputToStream
        :   public OutputFunc
    {
    public:
        OutputToStream( );
        virtual void operator()( const std::string & domain, int level,
                                 const std::string & message );
        void SetOutputStream( std::ostream & dest, int level );

    private:
        std::ostream *  m_destinations[ NumLevels ];
    };
        
    void LogVA( int level, const char * format, std::va_list args );

    std::string                         m_domain;
    int                                 m_verbosity;
    std::tr1::shared_ptr< OutputFunc >  m_pOutputFunc;
};


//=============================================================================


extern Logger g_generalLogger;


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //LOGGER_HPP
