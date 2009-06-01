#ifndef MYSQLLIBRARY_HPP
#define MYSQLLIBRARY_HPP
/*
  MySQLLibrary.hpp
  Copyright (C) 2007 David M. Anderson

  MySQLLibrary class: Handles initialization and shutdown of an
  embedded MySQL server library.
  NOTES:
  1. This class doesn't need to be used by the application. It is used by the
     class MySQLDatabase.
  2. MySQLLibrary is a singleton with the accessor
     MySQLLibrary & MySQLLibrary::Instance().
*/


#ifdef USE_MYSQL


#include "Singleton.hpp"
#include <vector>
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MySQLLibrary
    :   public Singleton< MySQLLibrary >
{
public:
    void Init( );
    void Init( const std::vector< std::string > & options );
    void Shutdown( );

private:
    MySQLLibrary( );
    ~MySQLLibrary( );

    bool m_initialized;

    friend class Singleton< MySQLLibrary >;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_MYSQL
#endif //MYSQLLIBRARY_HPP
