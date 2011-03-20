#ifndef CONSTELLATIONS_HPP
#define CONSTELLATIONS_HPP
/*
  Constellations.hpp
  Copyright (C) 2007 David M. Anderson

  The constellations in the sky.
  NOTES:
  
*/


#include "Equatorial.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


enum Constellation
{
    Andromeda,          //mythical princess
    Antlia,             //air pump
    Apus,               //bird of paradise
    Aquarius,           //water bearer
    Aquila,             //eagle
    Ara,                //altar
    Aries,              //ram
    Auriga,             //charioteer
    Bootes,             //herdsman
    Caelum,             //sculptor's chisel
    Camelopardalis,     //giraffe
    Cancer,             //crab
    CanesVenatici,      //hunting dogs
    CanisMajor,         //greater dog
    CanisMinor,         //lesser dog
    Capricornus,        //sea goat
    Carina,             //keel
    Cassiopeia,         //mythical queen
    Centaurus,          //centaur
    Cepheus,            //mythical king
    Cetus,              //sea monster or whale
    Chamaeleon,         //chameleon
    Circinus,           //pair of drawing compasses
    Columba,            //Noah's dove
    ComaBerenices,      //Berenice's hair
    CoronaAustralis,    //southern crown
    CoronaBorealis,     //northern crown
    Corvus,             //crow
    Crater,             //cup
    Crux,               //(southern) cross
    Cygnus,             //swan
    Delphinus,          //dolphin
    Dorado,             //gold fish
    Draco,              //dragon
    Equuleus,           //little horse (foal)
    Eridanus,           //mythical river
    Fornax,             //chemical furnace
    Gemini,             //twins
    Grus,               //crane (bird)
    Hercules,           //mythical hero
    Horologium,         //pendulum clock
    Hydra,              //mythical beast
    Hydrus,             //lesser water snake
    Indus,              //American Indian
    Lacerta,            //lizard
    Leo,                //lion
    LeoMinor,           //lesser lion
    Lepus,              //hare
    Libra,              //balance
    Lupus,              //wolf
    Lynx,               //lynx
    Lyra,               //lyre
    Mensa,              //Table Mountain (South Africa)
    Microscopium,       //microscope
    Monoceros,          //unicorn
    Musca,              //fly
    Norma,              //carpenter's level
    Octans,             //mariner's octant
    Ophiuchus,          //serpent bearer
    Orion,              //mythical hunter
    Pavo,               //peacock
    Pegasus,            //mythical winged horse
    Perseus,            //mythical hero
    Phoenix,            //phoenix
    Pictor,             //easel
    Pisces,             //two fish
    PiscisAustrinus,    //southern fish
    Puppis,             //stern
    Pyxis,              //mariner's compass
    Reticulum,          //eyepiece graticule
    Sagitta,            //arrow
    Sagittarius,        //archer
    Scorpius,           //scorpion
    Sculptor,           //sculptor's workshop
    Scutum,             //shield
    Serpens,            //serpent
    Sextans,            //astronomical sextant
    Taurus,             //bull
    Telescopium,        //telescope
    Triangulum,         //triangle
    TriangulumAustrale, //southern triangle
    Tucana,             //toucan
    UrsaMajor,          //great bear
    UrsaMinor,          //lesser bear
    Vela,               //sails
    Virgo,              //maiden
    Volans,             //flying fish
    Vulpecula,          //fox
    NumConstellations
};


//*****************************************************************************


class ConstellationInfo
{
public:
    ConstellationInfo( std::string name, std::string abbrev,
                       std::string genitive );
    const std::string & Name( ) const;
    const std::string & Abbrev( ) const;
    const std::string & Genitive( ) const;

private:
    std::string m_name;
    std::string m_abbrev;
    std::string m_genitive;
};


//-----------------------------------------------------------------------------


const ConstellationInfo & GetConstellationInfo( Constellation constellation );


//=============================================================================


Constellation GetConstellation( Equatorial equatorialPos, double epoch );

#ifdef DEBUG
bool TestConstellations( );
#endif


//*****************************************************************************
//*****************************************************************************


inline
ConstellationInfo::ConstellationInfo( std::string name, std::string abbrev,
                                      std::string genitive )
    :   m_name( name ),
        m_abbrev( abbrev ),
        m_genitive( genitive )
{
}

//=============================================================================

inline
const std::string &
ConstellationInfo::Name( ) const
{
    return m_name;
}

//-----------------------------------------------------------------------------

inline
const std::string &
ConstellationInfo::Abbrev( ) const
{
    return m_abbrev;
}

//-----------------------------------------------------------------------------

inline
const std::string &
ConstellationInfo::Genitive( ) const
{
    return m_genitive;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CONSTELLATIONS_HPP
