/*
  CalendarLibText.cpp
  Copyright (C) 2007 David M. Anderson

  Strings used in CalendarLib.
  In principle most of these should be included in the relevent classes,
  but keeping them all in one file makes localization and other maintainance
  easier.
  NOTES:
  1. These are generally the forms of the names given in Wikipedia. I have also
     used Dershowitz & Reingold, "Calendrical Calculations" (various editions)
     and other sources as noted.
*/


#include "CalendarLibText.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const string g_egyptianMonthNames[ 13 ]
    = {
    //These are the Hellenized forms.
        "Thoth",
        "Phaophi",
        "Athyr",
        "Choiak",
        "Tybi",
        "Mechir",
        "Phamenoth",
        "Pharmouthi",
        "Pachon",
        "Payni",
        "Epiphi",
        "Mesore",
        "Epagomenae"
    };
const std::string g_mayanHaabMonthNames[ 19 ]
    = {
    //Orthography of the Guatemalan Academia de Lenguas Mayas
        "Pop",
        "Wo'",
        "Sip",
        "Sotz'",
        "Sek",
        "Xul",
        "Yaxk'in'",
        "Mol",
        "Ch'en",
        "Yax",
        "Sak'",
        "Keh",
        "Mak",
        "K'ank'in",
        "Muwan'",
        "Pax",
        "K'ayab",
        "Kumk'u",
        "Wayeb'"
    };
const std::string g_mayanTzolkinVeintenaNames[ 20 ]
    = {
    //Orthography of the Guatemalan Academia de Lenguas Mayas
        "Imix'",
        "Ik'",
        "Ak'b'al",
        "K'an",
        "Chikchan",
        "Kimi",
        "Manik'",
        "Lamat",
        "Muluk",
        "Ok",
        "Chuwen",
        "Eb'",
        "B'en",
        "Ix",
        "Men",
        "K'ib'",
        "Kab'an",
        "Etz'nab'",
        "Kawak",
        "Ajaw"
    };
const std::string g_chineseCelestialStemNames[ 10 ]
    = {
        "Jiǎ",
        "Yǐ",
        "Bǐng",
        "Dīng",
        "Wù",
        "Jǐ",
        "Gēng",
        "Xīn",
        "Rén",
        "Guǐ"
    };
const std::string g_chineseTerrestrialBranchNames[ 12 ]
    = {
        "Zǐ",
        "Chǒu",
        "Yín",
        "Mǎo",
        "Chén",
        "Sì",
        "Wǔ",
        "Wèi",
        "Shēn",
        "Yǒu",
        "Xū",
        "Hài"
    };
const std::string g_chineseCelestialStemEnglishNames[ 10 ]
    = { //From Lance Latham, "Standard C Date/Time Library", 
        // in the file strnameen.c on the accompanying CD-ROM.
        // The stem names are actually not translatable.
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
const std::string g_chineseMajorSolarTermNames[ 12 ]
    = {
        "Yǔshuǐ",
        "Chūnfēn",
        "Gǔyǔ",
        "Xiǎomǎn",
        "Xiàzhì",
        "Dàshǎ",
        "Chǔshǔ",
        "Qiūfēn",
        "Shuāngjiàng",
        "Xiǎoxuě",
        "Dōngzhì",
        "Dàhán"
    };
const std::string g_chineseMinorSolarTermNames[ 12 ]
    = {
        "Lìchūn",
        "Jīngzhé",
        "Qīngmíng",
        "Lìxià",
        "Mángzhòng",
        "Xiǎoshǔ",
        "Lìqiū",
        "Báilù",
        "Hánlù",
        "Lìdōng",
        "Dàxuě",
        "Xiǎohán"
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
const string g_armenianMonthNames[ 13 ]
    = {
        "Nawasardi",
        "Hoi",
        "Sahmi",
        "Trē",
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
const string g_copticMonthNames[ 13 ]
    = {
        "Thoout",
        "Paope",
        "Athôr",
        "Koiak",
        "Tôbe",
        "Meshir",
        "Paremotep",
        "Parmoute",
        "Pashons",
        "Paône",
        "Epêp",
        "Mesorê",
        "Epagomenê"
    };
const std::string g_copticWeekDayNames[ 7 ]
    = {
        "Tkyriakê",
        "Pesnau",
        "Pshoment",
        "Peftoou",
        "Ptiou",
        "Psoou",
        "Psabbaton"
    };
const string g_ethiopianMonthNames[ 13 ]
    = {
        "Maskaram",
        "Teqemt",
        "Ḫedār",
        "Tākhśāś",
        "Ṫer",
        "Yakātit",
        "Magābit",
        "Miyāzyā",
        "Genbot",
        "Sanē",
        "Ḥamlē",
        "Naḥasē",
        "Pāguemēn"
    };
const std::string g_ethiopianWeekDayNames[ 7 ]
    = {
        "Iḥud",
        "Sanyo",
        "Maksanyo",
        "Rob",      //or Rabu`e
        "Ḫamus",
        "Arb",
        "Kidāmmē"
    };
const string g_hebrewMonthNamesCY[ 12 ] //common year
    = {
        "Nisan",
        "Iyyar",
        "Sivan",
        "Tammuz",
        "Av",
        "Elul",
        "Tishri",
        "Marḥeshvan",
        "Kislev",
        "Tevet",
        "Shevat",
        "Adar"
    };
const string g_hebrewMonthNamesLY[ 13 ] //leap year
    = {
        "Nisan",
        "Iyyar",
        "Sivan",
        "Tammuz",
        "Av",
        "Elul",
        "Tishri",
        "Marḥeshvan",
        "Kislev",
        "Tevet",
        "Shevat",
        "Adar I",
        "Adar II"
    };
const string g_hebrewWeekDayNames[ 7 ]
    = {
        "yom rishon",
        "yom sheni",
        "yom shelishi",
        "yom revi`i",
        "yom ḥamishi",
        "yom shishi",
        "yom shabbat"
    };
const std::string g_hinduSolarMonthNames[ 12 ]
    = {
        "Mesh",
        "Vrishabh",
        "Mithun",
        "Karkat",
        "Simha",
        "Kanya",
        "Tula",
        "Vrishchik",
        "Dhanu",
        "Makar",
        "Kumbha",
        "Meen"
    };
const std::string g_hinduLunarMonthNames[ 12 ]
    = {
        "Chaitra",
        "Vaishākh",
        "Jyaishtha",
        "Āshādha",
        "Shrāvana",
        "Bhādrapad",        //or Bhaadra
        "Āshwin",
        "Kārtik",
        "Mārgashīrsha",     //or Agrahayana
        "Paush",
        "Māgh",
        "Phālgun"
    };
const std::string g_hinduWeekDayNames[ 7 ]
    = {
        "Ravi vāsara",      //or Adityavara
        "Soma vāsara",      //or Chandravara
        "Mangala vāsara",   //or Bhaumavara
        "Budha vāsara",     //or Saumyavara
        "Guru vāsara",      //or Brihaspatvara
        "Shukra vāsara",
        "Shani vāsara"
    };
const string g_islamicMonthNames[ 12 ]
    = {
        "Muḥarram",
        "Ṣafar",
        "Rabī` I",
        "Rabī` II",
        "Jumādā I",
        "Jumādā II",
        "Rajab",
        "Sha`bān",
        "Ramaḍān",
        "Shawwāl",
        "Dhū al-Qa`da",
        "Dhū al-Ḥijja"
    };
const string g_islamicWeekDayNames[ 7 ]
    = {
        "yawm al-aḥad",
        "yawm al-ithnayn",
        "yawm ath-thulaathaa'",
        "yawm al-arba`aa'",
        "yawm al-khamīs",
        "yawm al-jumu`ah",
        "yawm as-sabt"
    };
const string g_persianMonthNames[ 12 ]
    = {
        "Farvardīn",
        "Ordībehesht",
        "Xordād",
        "Tīr",
        "Mordād",
        "Shahrīvar",
        "Mehr",
        "Ābān",
        "Āzar",
        "Dey",
        "Bahman",
        "Esfand"
    };
const string g_persianWeekDayNames[ 7 ]
    = {
        "Shanbēh",
        "Yek-shanbēh",
        "Do-shanbēh",
        "Se-shanbēh",
        "Chār-shanbēh",
        "Panj-shanbēh",
        "Jom'ēh"
    };
const std::string g_bahaiDayNames[ 19 ]
    = {
        "Bahā`",
        "Jalāl",
        "Jamāl",
        "`Aẓamat",
        "Nūr",
        "Raḥmat",
        "Kalimāt",
        "Kamāl",
        "Asmā'",
        "`Izzat",  
        "Mashīyyat",
        "`Ilm",
        "Qudrat",
        "Qawl",
        "Masā'il",
        "Sharaf",
        "Sulṭān",
        "Mulk",
        "`Alā'"
    };
const std::string g_bahaiMonthNames[ 20 ]
    = {
        "Bahā`",
        "Jalāl",
        "Jamāl",
        "`Aẓamat",
        "Nūr",
        "Raḥmat",
        "Kalimāt",
        "Kamāl",
        "Asmā'",
        "`Izzat",  
        "Mashīyyat",
        "`Ilm",
        "Qudrat",
        "Qawl",
        "Masā'il",
        "Sharaf",
        "Sulṭān",
        "Mulk",
        "Ayyām-i-Hā",
        "`Alā'"
    };
const std::string g_bahaiYearNames[ 19 ]
    = {
        "Alif",
        "Bā'",
        "Āb",
        "Dāl",
        "Bāb",
        "Vāv",
        "Abad",
        "Jād",
        "Bahā'",
        "Ḥubb",
        "Bahhāj",
        "Javāb",
        "Aḥad",
        "Vahhāb",
        "Vidād",
        "Badī'",
        "Bahī",
        "Abhā",
        "Vāḥid"
    };
const std::string g_bahaiWeekDayNames[ 7 ]
    = {
        "Jalāl",
        "Jamāl",
        "Kamāl",
        "Fidāl",
        "`Idāl",
        "Istijlāl",
        "Istiqlāl"
    };
const std::string g_frenchRevolutionaryMonthNames[ 13 ]
    = {
        "Vendémiaire",
        "Brumaire",
        "Frimaire",
        "Nivôse",
        "Pluviôse",
        "Ventôse",
        "Germinal",
        "Floréal",
        "Prairial",
        "Messidor",
        "Thermidor",
        "Fructidor",
        "Sansculottides"
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
        "Décadi"
    };
const std::string g_frenchRevolutionarySansculottidesNames[ 6 ]
    = {
        "Fête de la Vertu",
        "Fête du Génie",
        "Fête du Travail",
        "Fête de l'Opinion",
        "Fête de la Récompense",
        "Jour de la Révolution"
    };


//*****************************************************************************

}                                                      //namespace EpsilonDelta
