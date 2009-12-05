#ifndef CODEPOINTDATA_HPP
#define CODEPOINTDATA_HPP
/*
  CodePointData.hpp
  Copyright © 2009 David M. Anderson

  Unicode code point data. Generated by my UnicodeDB program.
*/


#include "StdInt.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


enum ECharTypeBit
{
    CTB_Alpha       = (1 << 0),
    CTB_Upper       = (1 << 1),
    CTB_Lower       = (1 << 2),
    CTB_DecDigit    = (1 << 3),
    CTB_HexDigit    = (1 << 4),
    CTB_Punctuation = (1 << 5),
    CTB_Graph       = (1 << 6),
    CTB_Blank       = (1 << 7),
    CTB_Space       = (1 << 8),
    CTB_Ctrl        = (1 << 9)
};

enum LineBreakClass
{
    LBC_MandatoryBreak,                 //BK
    LBC_CarriageReturn,                 //CR
    LBC_LineFeed,                       //LF
    LBC_CombiningMark,                  //CM
    LBC_WordJoiner,                     //WJ
    LBC_ZeroWidthSpace,                 //ZW
    LBC_Glue,                           //GL
    LBC_Space,                          //SP
    LBC_BreakOpportunityBeforeAndAfter, //B2
    LBC_BreakAfter,                     //BA
    LBC_BreakBefore,                    //BB
    LBC_Hyphen,                         //HY
    LBC_ContingentBreakOpportunity,     //CB
    LBC_ClosePunctuation,               //CL
    LBC_CloseParenthesis,               //CP
    LBC_ExclamationInterrogation,       //EX
    LBC_Inseparable,                    //IN
    LBC_Nonstarter,                     //NS
    LBC_OpenPunctuation,                //OP
    LBC_Quotation,                      //QU
    LBC_InfixNumericSeperator,          //IS
    LBC_Numeric,                        //NU
    LBC_PosfixNumeric,                  //PO
    LBC_PrefixNumeric,                  //PR
    LBC_SymbolsAllowingBreakAfter,      //SY
    LBC_Alphabetic,                     //AL
    LBC_HangulLVSyllable,               //H2
    LBC_HangulLVTSyllable,              //H3
    LBC_Ideographic,                    //ID
    LBC_HangulLJamo,                    //JL
    LBC_HangulVJamo,                    //JV
    LBC_HangulTJamo,                    //JT
    LBC_NextLine,                       //NL
    LBC_Surrogate,                      //SG
    LBC_Ambiguous,                      //AI
    LBC_ComplexContextDependent,        //SA
    LBC_UnknownLineBreak,               //XX
    NumLineBreakClasses
};


extern int MaximumCodePoint;

extern uint16_t codePointCharTypes[ ];
extern wchar_t codePointToUpper[ ];
extern wchar_t codePointToLower[ ];
extern char codePointToASCII[ ];
extern uint8_t codePointLineBreakClasses[ ];


//*****************************************************************************

}                                                      //namespace EpsilonDelta
#endif //CODEPOINTDATA_HPP