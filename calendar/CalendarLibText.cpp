/*
  CalendarLibText.cpp
  Copyright (C) 2007 David M. Anderson

  Strings used in CalendarLib.
  In principle most of these should be included in the relevent classes,
  but keeping them all in one file makes localization and other maintainance
  easier.
  NOTES:
  1. Unless otherwise noted, these are the names listed in Dershowitz &
     Reingold, "Calendrical Calculations".
*/


#include "CalendarLibText.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const string g_islamicMonthNames[ 12 ]
    = { //diacritics lost
        "Muharram",
        "Safar",
        "Rabi` I",
        "Rabi` II",
        "Jumada I",
        "Jumada II",
        "Rajab",
        "Sha`ban",
        "Ramadan",
        "Shawwal",
        "Dhu al-Qa`da",
        "Dhu al-Hijja"
    };
const string g_islamicWeekDayNames[ 7 ]
    = { //diacritics lost
        "yawm al-'ahad",
        "yawm al-'ithnayn",
        "yawm ath-thalatha'",
        "yawm al-'arba`a'",
        "yawm al-khamis",
        "yawm al-jum`a",
        "yawm as-sabt"
    };
const string g_persianMonthNames[ 12 ]
    = { //diacritics lost
        "Farvardin",
        "Ordibehesht",
        "Khordad",
        "Tir",
        "Mordad",
        "Shahrivar",
        "Mehr",
        "Aban",
        "Azar",
        "Dey",
        "Bahman",
        "Esfand"
    };
const string g_persianWeekDayNames[ 7 ]
    = {
        "Shanbeh",
        "Yek-shanbeh",
        "Do-shanbeh",
        "Se-shanbeh",
        "Chahar-shanbeh",
        "Panj-shanbeh",
        "Jom'eh"
    };
const string g_hebrewMonthNamesCY[ 12 ]
    = { //diacritics lost
        "Nisan",
        "Iyyar",
        "Sivan",
        "Tammuz",
        "Av",
        "Elul",
        "Tishri",
        "Heshvan",
        "Kislev",
        "Teveth",
        "Shevat",
        "Adar"
    };
const string g_hebrewMonthNamesLY[ 13 ]
    = { //diacritics lost
        "Nisan",
        "Iyyar",
        "Sivan",
        "Tammuz",
        "Av",
        "Elul",
        "Tishri",
        "Heshvan",
        "Kislev",
        "Teveth",
        "Shevat",
        "Adar I",
        "Adar II"
    };
const string g_hebrewWeekDayNames[ 7 ]
    = {
        "Yom Reeshone",
        "Yom Shaynee",
        "Yom Shlee'shee",
        "Yom Revee'ee",
        "Yom Khah'mee'shee",
        "Yom Ha'shee'shee",
        "Shabbat"
    };
const string g_copticMonthNames[ 13 ]
    = {  //diacritics lost
        "Thoout",
        "Paope",
        "Athor",
        "Koiak",
        "Tobe",
        "Meshir",
        "Paremotep",
        "Parmoute",
        "Pashons",
        "Paone",
        "Epap",
        "Mesore",
        "Epagomene"
    };
const std::string g_copticWeekDayNames[ 7 ]
    = { //diacritics lost
        "Tkyriake",
        "Pesnau",
        "Pshoment",
        "Peftoou",
        "Ptiou",
        "Psoou",
        "Psabbaton"
    };
const string g_ethiopianMonthNames[ 13 ]
    = {  //diacritics lost
        "Maskaram",
        "Teqemt",
        "Hedar",
        "Takhsas",
        "Ter",
        "Yakatit",
        "Magabit",
        "Miyazya",
        "Genbot",
        "Sane",
        "Hamle",
        "Nahase",
        "Paguemen"
    };
const std::string g_ethiopianWeekDayNames[ 7 ]
    = { //diacritics lost
        "Ihud",
        "Sanyo",
        "Maksanyo",
        "Rob",      //or Rabu`e
        "Hamus",
        "Arb",
        "Kidamme"
    };
const std::string g_bahaiDayNames[ 19 ]
    = {  //diacritics lost
        "Baha",
        "Jalal",
        "Jamal",
        "`Azamat",
        "Nur",
        "Rahmat",
        "Kalimat",
        "Kamal",
        "Asma'",
        "`Izzat",  
        "Mashiyyat",
        "`Ilm",
        "Qudrat",
        "Qawl",
        "Masa'il",
        "Sharaf",
        "Sultan",
        "Mulk",
        "`Ala'"
    };
const std::string g_bahaiMonthNames[ 20 ]
    = { //diacritics lost
        "Baha",
        "Jalal",
        "Jamal",
        "`Azamat",
        "Nur",
        "Rahmat",
        "Kalimat",
        "Kamal",
        "Asma'",
        "`Izzat",  
        "Mashiyyat",
        "`Ilm",
        "Qudrat",
        "Qawl",
        "Masa'il",
        "Sharaf",
        "Sultan",
        "Mulk",
        "Ayyam-i-Ha",
        "`Ala'"
    };
const std::string g_bahaiYearNames[ 19 ]
    = { //diacritics lost
        "Alif",
        "Ba'",
        "Ab",
        "Dal",
        "Bab",
        "Vav",
        "Abad",
        "Jad",
        "Baha",
        "Hubb",
        "Bahhaj",
        "Javab",
        "Ahad",
        "Vahhab",
        "Vidad",
        "Badi",
        "Bahi",
        "Abha",
        "Vahid"
    };
const std::string g_bahaiWeekDayNames[ 7 ]
    = { //diacritics lost
        "Jalal",
        "Jamal",
        "Kamal",
        "Fidal",
        "`Idal",
        "Istijlal",
        "Istiqlal"
    };
const std::string g_chineseMajorSolarTermNames[ 12 ]
    = {
        "Yushui",
        "Chunfen",
        "Guyu",
        "Xiaoman",
        "Xiazhi",
        "Dashu",
        "Chushu",
        "Qiufen",
        "Shuangjiang",
        "Xiaoxue",
        "Dongzhi",
        "Dahan"
    };
const std::string g_chineseMinorSolarTermNames[ 12 ]
    = {
        "Lichun",
        "Jingzhe",
        "Qingming",
        "Lixia",
        "Mangzhong",
        "Xiaoshu",
        "Liqiu",
        "Bailu",
        "Hanlu",
        "Lidong",
        "Daxue",
        "Xiaohan"
    };
const std::string g_chineseMajorSolarTermEnglishNames[ 12 ]
    = {
        "Rain Water",
        "Spring Equinox",
        "Grain Rain",
        "Grain Full",
        "Summer Solstice",
        "Great Heat",
        "Limit of Heat",
        "Autumnal Equinox",
        "Descent of Frost",
        "Slight Snow",
        "Winter Solstice",
        "Great Cold"
    };
const std::string g_chineseMinorSolarTermEnglishNames[ 12 ]
    = {
        "Beginning of Spring",
        "Waking of Insects",
        "Pure Brightness",
        "Beginning of Summer",
        "Grain in Ear",
        "Slight Heat",
        "Beginning of Autumn",
        "White Dew",
        "Cold Dew",
        "Beginning of Winter",
        "Great Snow",
        "Slight Cold"
    };
const std::string g_chineseCelestialStemNames[ 10 ]
    = {
        "Jia",
        "Yi",
        "Bing",
        "Ding",
        "Wu",
        "Ji",
        "Geng",
        "Xin",
        "Ren",
        "Gui"
    };
const std::string g_chineseTerrestrialBranchNames[ 12 ]
    = {
        "Zi",
        "Chou",
        "Yin",
        "Mao",
        "Chen",
        "Si",
        "Wu",
        "Wei",
        "Shen",
        "You",
        "Xu",
        "Hai"
    };
const std::string g_chineseCelestialStemEnglishNames[ 10 ]
    = { //From Lance Latham, "Standard C Date/Time Library", 
        // in the file strnameen.c on the accompanying CD-ROM.
        "Helmet",
        "Germination",
        "Fire",
        "Nail",
        "Halberd",
        "Self",
        "Age",
        "Bitterness",
        "Burden",
        "Grass"
    };
const std::string g_chineseTerrestrialBranchEnglishNames[ 12 ]
    = {
        "Rat",
        "Ox",
        "Tiger",
        "Hare",
        "Dragon",
        "Snake",
        "Horse",
        "Sheep",
        "Monkey",
        "Fowl",
        "Dog",
        "Pig"
    };
const std::string g_chineseElementNames[ 5 ]
    = {
        "Mu",
        "Huo",
        "Tu",
        "Jin",
        "Shui"
    };
const std::string g_chineseElementEnglishNames[ 5 ]
    = { //From Helmer Aslaksen, "The Mathematics of the Chinese Calendar".
        "Wood",
        "Fire",
        "Earth",
        "Metal",
        "Water"
    };
const std::string g_hinduSolarMonthNames[ 12 ]
    = { //diacritics lost
        "Mesha",
        "Vrishabha",
        "Mithuna",
        "Karka",
        "Simha",
        "Kanya",
        "Tula",
        "Vrischika",
        "Dhanu",
        "Makara",
        "Kumbha",
        "Mina"
    };
const std::string g_hinduLunarMonthNames[ 12 ]
    = { //diacritics lost
        "Chaitra",
        "Vaisakha",
        "Jyaishtha",
        "Ashadha",
        "Sravana",
        "Bhadrapada",
        "Asvina",
        "Karttika",
        "Margasira",
        "Pausha",
        "Magha",
        "Phalguna"
    };
const std::string g_hinduWeekDayNames[ 7 ]
    = { //diacritics lost
        "Ravivara",         //or Adityavara
        "Chandravara",      //or Somavara
        "Mangalavara",      //or Bhaumavara
        "Buddhavara",       //or Saumyavara
        "Brihaspatvara",    //or Guruvara
        "Sukravara",
        "Sanivara"
    };
const string g_egyptianMonthNames[ 13 ]
    = {
        "Thoth",
        "Phaophi",
        "Athyr",
        "Choiak",
        "Tybi",
        "Mechir",
        "Phamenoth",
        "Pharmuthi",
        "Pachon",
        "Payni",
        "Epiphi",
        "Mesori",
        "Epagomenae"
    };
const string g_armenianMonthNames[ 13 ]
    = {
        "Nawasardi",
        "Hori",
        "Sahmi",
        "Tre",
        "K`aloch",
        "Arach",
        "Mehekani",
        "Areg",
        "Ahekani",
        "Mareri",
        "Margach",
        "Hrotich",
        "Aweleach"
    };
const std::string g_armenianWeekDayNames[ 13 ]
    = {
        "Miashabathi",
        "Erkoushabathi",
        "Erekhshabathi",
        "Chorekhshabathi",
        "Hingshabathi",
        "Urbath",
        "Shabath"
    };
const std::string g_mayanHaabMonthNames[ 19 ]
    = {
        "Pop",
        "Uo",
        "Zip",
        "Zotz",
        "Tzec",
        "Xul",
        "Yaxkin",
        "Mol",
        "Chen",
        "Yax",
        "Zac",
        "Ceh",
        "Mac",
        "Kankin",
        "Muan",
        "Pax",
        "Kayab",
        "Cumku",
        "Uayeb"
    };
const std::string g_mayanTzolkinVeintenaNames[ 20 ]
    = {
        "Imix",
        "Ik",
        "Akbal",
        "Kan",
        "Chicchan",
        "Cimi",
        "Manik",
        "Lamat",
        "Muluc",
        "Oc",
        "Chuen",
        "Eb",
        "Ben",
        "Ix",
        "Men",
        "Cib",
        "Caban",
        "Etznab",
        "Cauac",
        "Ahau"
    };
const std::string g_frenchRevolutionaryMonthNames[ 13 ]
    = {
        "Vendemiaire",
        "Brumaire",
        "Frimaire",
        "Nicose",
        "Pluviose",
        "Ventose",
        "Germinal",
        "Floreal",
        "Prairial",
        "Messidor",
        "Thermidor",
        "Fructidor",
        "Sanscullottides"
    };
const std::string g_frenchRevolutionaryDecadeNames[ 10 ]
    = {
        "Primidi",
        "Duodi",
        "Tridi",
        "Quartidi",
        "Quintidi",
        "Sextidi",
        "Septidi",
        "Octidi",
        "Nonidi",
        "Decadi"
    };
const std::string g_frenchRevolutionarySansculottidesNames[ 6 ]
    = { //diacritics lost
        "Fete de la Vertu",
        "Fete du Genie",
        "Fete du Travail",
        "Fete de l'Opinion",
        "Fete de la Recompense",
        "Fete de la Revolution"
    };


//*****************************************************************************

}                                                      //namespace EpsilonDelta
