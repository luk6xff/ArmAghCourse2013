#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "string.h"
#include "command_decoder.h"


#define MAX_BUF_SIZE 254
#define MAX_KEYWORD_NR 2  /// maksymalna ilosc slow kluczowych
//////////////////////////////////////////////////////////////////////////////////////
/// ZMIENNE GLOBALNE

extern struct Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;

struct Keyword asKeywordList[MAX_KEYWORD_NR];
struct Keyword asKeywordList[MAX_KEYWORD_NR]=
{
    {CALLIB,"callib"},
    {GOTO, "goto" }
    
};
//////////////////////////////////////////////////////////////////////////////////////

///deklaracje funkcji

//TOKENY
unsigned char ucFindTokensInString (char *pcString)
{
    typedef enum {TOKEN, DELIMETER} State;
    State eState= TOKEN;
    unsigned char ucCharCounter;
    unsigned char ucIndexAsTokenTable=0;
    for(ucCharCounter=0;; ucCharCounter++)
    {
       switch(eState)
       {
        case TOKEN:
        {
            if( ucIndexAsTokenTable==MAX_TOKEN_NR)
            {
                return ucIndexAsTokenTable;
            }
            else if(*pcString==' ')
            {
                eState=DELIMETER;
            }
            else if (*pcString=='\0')
            {
                return  ucIndexAsTokenTable;
            }
            else if(ucCharCounter==0)    // jesli nie ma spacji ani NULLA na pierwszym miejscu
            {
                asToken[ucIndexAsTokenTable].uValue.pcString=pcString;
                ucIndexAsTokenTable++;
                eState=TOKEN;
            }
            else
            {
                eState=TOKEN;
            }
            break;
        }
        case DELIMETER:
        {
            if(*pcString==' ')
            {
                eState=DELIMETER;
            }
            else if(*pcString=='\0')
            {
                return ucIndexAsTokenTable;
            }
            else
            {
                asToken[ucIndexAsTokenTable].uValue.pcString=pcString;
                ucIndexAsTokenTable++;
                eState=TOKEN;
            }
            break;
        }
       }
      pcString++;
    }
}

///////////////////////////////////////////////////////////////////////////////

Result eStringToKeyword (char pcStr[],KeywordCode *peKeywordCode)
{
    unsigned char ucKeywordCounter;

    for(ucKeywordCounter=0; ucKeywordCounter<MAX_KEYWORD_NR; ucKeywordCounter++)
    {
        if(eCompareString(asKeywordList[ucKeywordCounter].cString,pcStr )==EQUAL)
        {
            *peKeywordCode=asKeywordList[ucKeywordCounter].eCode;
            return OK;
        }
		}
    return ERROR;
}

///////////////////////////////////////////////////////////////////////////////

void DecodeTokens(void)
{
    unsigned char ucTokenNumberCounter;
    char *cCurrentToken;
    KeywordCode peKeywordCode;
    unsigned int uiValue;
    TokenType eType;
    union TokenValue uValue;

    for(ucTokenNumberCounter=0; ucTokenNumberCounter<ucTokenNr; ucTokenNumberCounter++)
    {
        cCurrentToken=asToken[ucTokenNumberCounter].uValue.pcString;

        if(eStringToKeyword(cCurrentToken,&peKeywordCode)==OK)
        {
            eType=KEYWORD;
            uValue.eKeyword=peKeywordCode;
        }
        else if(eHexStringToUInt(cCurrentToken,&uiValue)==OK)
        {
            eType=NUMBER;
            uValue.uiNumber=uiValue;
        }
        else
        {
            eType=STRING;
            uValue.pcString=cCurrentToken;
        }
        asToken[ucTokenNumberCounter].eType=eType;
        asToken[ucTokenNumberCounter].uValue=uValue;
    }
}
///////////////////////////////////////////////////////////////////////////////

void DecodeMsg(char *pcString)
{
    ucTokenNr=ucFindTokensInString (pcString);
    ReplaceCharactersInString(pcString,' ','\0');
    DecodeTokens();
}

///////////////////////////////////////////////////////////////////////////////

