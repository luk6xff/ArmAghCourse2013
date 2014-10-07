#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



#define NULL '\0'
#define MAX_BUF_SIZE 254
#define MAX_TOKEN_NR 3 ///maksymalna dopuszczalna ilosc tokenow
#define MAX_KEYWORD_NR 3  /// maksymalna ilosc slow kluczowych
#define MAX_KEYWORD_STRING_LTH 10 /// mksymalna dlugosc komendy

///typy uzywane w kodzie
typedef enum {DIFFERENT,EQUAL} CompResult;
typedef enum {OK,ERROR} Result;
////////////////////////////////////////////////////
typedef enum {LD, ST, RST} KeywordCode;
typedef struct Keyword
{
    KeywordCode eCode;
    char cString[MAX_KEYWORD_STRING_LTH + 1];
};
////////////////////////////////////////////////////
typedef union TokenValue
{
    KeywordCode eKeyword; /// jezeli KEYWORD
    unsigned int uiNumber; /// jezeli NUMBER
    char * pcString; /// jezeli STRING
};

typedef enum { KEYWORD, NUMBER, STRING} TokenType;

typedef struct Token
{
    TokenType eType; /// KEYWORD, NUMBER, STRING
    union TokenValue uValue; /// enum, unsigned int, char*
};

//////////////////////////////////////////////////////////////////////////////////////
/// ZMIENNE GLOBALNE

struct Token asToken[MAX_TOKEN_NR];
unsigned char ucTokenNr;
struct Keyword asKeywordList[MAX_KEYWORD_NR];
struct Keyword asKeywordList[MAX_KEYWORD_NR]=
{
    {RST,"reset"},
    {LD, "load" },
    {ST, "store"}
};
//////////////////////////////////////////////////////////////////////////////////////

///deklaracje funkcji
//STRINGI
void CopyString(char pcSource[], char pcDestination[]);
CompResult eCompareString(char pcStr1[], char pcStr2[]);
void AppendString (char pcSourceStr[],char pcDestinationStr[]);
void AppendString (char pcSourceStr[],char pcDestinationStr[]);
void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar);
void UIntToHexStr (unsigned int uiValue, char pcStr[]);
Result eHexStringToUInt(char pcStr[],unsigned int *puiValue);
void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]);
//TOKENY
unsigned char ucFindTokensInString (char *pcString);
Result eSringToKeyword (char pcStr[],KeywordCode *peKeywordCode);
void DecodeTokens(void);
void DecodeMsg(char *pcString);


///deklaracje funkcji testowych
//STRINGI
void TestOf_CopyString(void);
void TestOf_eCompareString(void);
void TestOf_AppendString(void);
void TestOf_ReplaceCharactersInString(void);
void TestOf_UIntToHexStr(void);
void TestOf_eHexStringToUInt(void);
void TestOf_AppendUIntToString(void);
//TOKENY
void TestOf_ucFindTokensInString (void);
void TestOf_eSringToKeyword(void);
void TestOf_DecodeTokens(void);
void TestOf_DecodeMsg(void);

///Definicje funkcji
//STRINGI
//////////////////////////////////////////////////////////////////////////////////////
void CopyString(char pcSource[], char pcDestination[])
{
    unsigned char ucCharCounter;
    for(ucCharCounter=0; pcSource[ucCharCounter]!=NULL; ucCharCounter++)
    {
        pcDestination[ucCharCounter]=pcSource[ucCharCounter];
    }
    pcDestination[ucCharCounter+3]='P';
}
//////////////////////////////////////////////////////////////////////////////////////


CompResult eCompareString(char pcStr1[], char pcStr2[])
{
    unsigned char ucCharCounter;
    for(ucCharCounter=0; pcStr1[ucCharCounter]!=NULL; ucCharCounter++)
    {
        if(pcStr1[ucCharCounter]!=pcStr2[ucCharCounter])
        {
            return DIFFERENT;
        }
    }
    return EQUAL;
}
//////////////////////////////////////////////////////////////////////////////////////
void AppendString (char pcSourceStr[],char pcDestinationStr[])
{
    unsigned char ucCharCounter;
    for(ucCharCounter=0; pcDestinationStr[ucCharCounter]!=NULL; ucCharCounter++)
    {
        ;
    }
    CopyString(pcSourceStr,&pcDestinationStr[ucCharCounter]);

}
//////////////////////////////////////////////////////////////////////////////////////

void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar)
{

    unsigned char ucCharCounter;
    for(ucCharCounter=0; pcString[ucCharCounter]!=NULL; ucCharCounter++)
    {
        if(pcString[ucCharCounter]==cOldChar)
        {
            pcString[ucCharCounter]=cNewChar;
        }
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////KONWERSJE//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UIntToHexStr (unsigned int uiValue, char pcStr[])
{
    pcStr[0]='0';
    pcStr[1]='x';
    pcStr[6]=NULL;

    unsigned char ucTetradCounter;
    unsigned int uiTetradValue = uiValue;

    for(ucTetradCounter=0; ucTetradCounter<4; ucTetradCounter++)
    {
        uiTetradValue= uiValue ;
        uiTetradValue=((uiTetradValue>>(ucTetradCounter*4))&0xF);
        if (uiTetradValue<=9)
        {
            pcStr[5-ucTetradCounter]=(char)(uiTetradValue+'0');
        }
        else
        {
            pcStr[5-ucTetradCounter]=(char)('A'+uiTetradValue-10);
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////


Result eHexStringToUInt(char pcStr[],unsigned int *puiValue)
{
    char cCharToAddToResult;
    unsigned char ucCharCounter;
    char cSingleCharOfString;
    *puiValue=0;
    if((pcStr[0]!='0')||(pcStr[1]!='x')||(pcStr[2]==NULL) )
    {
        return ERROR;
    }
    for (ucCharCounter=2; pcStr[ucCharCounter]!=NULL; ucCharCounter++)
    {
        cSingleCharOfString=pcStr[ucCharCounter];
        if ((ucCharCounter>6)/*&&(cSingleCharOfString!=NULL)*/)
        {
            return ERROR;
        }
        else if((cSingleCharOfString>='0')&&(cSingleCharOfString<='9'))
        {
            cCharToAddToResult= -'0';
        }
        else if((cSingleCharOfString>='A')&&(cSingleCharOfString<='F'))
        {
            cCharToAddToResult= -'A'+10;
        }
        else return ERROR;
        *puiValue=(unsigned int)((*puiValue<<4)|(cSingleCharOfString+ cCharToAddToResult));
    }
    return OK;
}
///////////////////////////////////////////////////////////////////////////////
void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[])
{
    unsigned char ucCharCounter;
    for(ucCharCounter=0; pcDestinationStr[ucCharCounter]!=NULL; ucCharCounter++)
    {
        ;
    }
    UIntToHexStr (uiValue, &pcDestinationStr[ucCharCounter]);
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////TOKENY///////////////////////////////////////////

unsigned char ucFindTokensInString (char *pcString)
{
    typedef enum {TOKEN, DELIMETER} State;
    State eState= TOKEN;
    unsigned char ucCharCounter;
    bool bDoubleDelimeter=false;
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
            else if (*pcString==NULL)
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
            else if(*pcString==NULL)
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
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
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
    unsigned char ucTokenNumberCounter;
    ucTokenNr=ucFindTokensInString (pcString);

    for(ucTokenNumberCounter=0; ucTokenNumberCounter<ucTokenNr; ucTokenNumberCounter++)
    {
        ReplaceCharactersInString(asToken[ucTokenNumberCounter].uValue.pcString,' ',NULL);
    }
    DecodeTokens();
}

///////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////TESTY_STRINGI/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////TESTY_STRINGI/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////TESTY_STRINGI/////////////////////////////////////////////////////////////////////////

void TestOf_CopyString(void)
{
    printf("\n**************************************************");
    unsigned char ucCounterLoop;
    printf("\nCopyString\n\n");

    char pcSource[]= {"ABCDEFGHIJKLMNOPQRSTUVXYZ"}  ;
    char pcDestination[MAX_BUF_SIZE]= {0};
    CopyString( pcSource, pcDestination);

    printf("pcSource:  ");
    for(ucCounterLoop=0; ucCounterLoop<strlen(pcSource); ucCounterLoop++)
    {
        printf("%c",pcSource[ucCounterLoop]);
    }

    printf("\npcDestination:  ");
    for(ucCounterLoop=0; ucCounterLoop<strlen(pcDestination); ucCounterLoop++)
    {
        printf("%c",pcDestination[ucCounterLoop]);
    }

    printf("\n**************************************************\n\n");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestOf_eCompareString(void)
{
    printf("\n**************************************************");
    printf("\neCompareString\n ");


    unsigned char ucCounter=2;
    unsigned char ucCounterLoop;
    char pcSource[]= {"ABCDEFGHIJKLMNOPQRSTUVXYZ"}  ;
    char* pcDestination;

    CompResult result;
    while(ucCounter--)
    {
        if(ucCounter==1)
        {
            printf("\nTest 1 - Stringi rowne: \n");
            //both strings are equal
            pcDestination= "ABCDEFGHIJKLMNOPQRSTUVXYZ";
        }
        else
        {
            pcDestination= "1234567890";
            printf ("\n\nTest 2 - Stringi rozne: \n");
        }
        switch(eCompareString(pcSource, pcDestination))
        {
        case EQUAL:
        {
            result=EQUAL;
            break;
        }
        case DIFFERENT:
        {
            result=DIFFERENT;
            break;
        }
        }
        printf("ODP:  %s",result?"EQUAL\n":"DIFFERENT\n");
        printf("pcSource:  ");
        for(ucCounterLoop=0; ucCounterLoop<strlen(pcSource); ucCounterLoop++)
        {
            printf("%c",pcSource[ucCounterLoop]);
        }
        printf("\npcDestination:  ");
        for(ucCounterLoop=0; ucCounterLoop<strlen(pcDestination); ucCounterLoop++)
        {
            printf("%c",pcDestination[ucCounterLoop]);
        }
    }
    printf("\n**************************************************\n\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestOf_AppendString(void)
{
    printf("\n**************************************************");
    printf("\nAppendString\n ");

    unsigned char ucCounterLoop;
    char pcSource[]= {"ABCDEFGHIJKLMNOPQRSTUVXYZ"}  ;
    char pcDestination[MAX_BUF_SIZE]= {"01232456789"};

    printf("\n---PRZED WYWOLANIEM FUNKCJI:---\n  ");
    printf("pcSource:  ");
    for(ucCounterLoop=0; ucCounterLoop<strlen(pcSource); ucCounterLoop++)
    {
        printf("%c",pcSource[ucCounterLoop]);
    }
    printf("\npcDestination:  ");
    for(ucCounterLoop=0; ucCounterLoop<strlen(pcDestination); ucCounterLoop++)
    {
        printf("%c",pcDestination[ucCounterLoop]);
    }

    AppendString( pcSource, pcDestination);
    printf("\n---PO WYWOLANIU FUNKCJI:---\n  ");
    printf("pcSource:  ");
    for(ucCounterLoop=0; ucCounterLoop<strlen(pcSource); ucCounterLoop++)
    {
        printf("%c",pcSource[ucCounterLoop]);
    }
    printf("\npcDestination:  ");
    for(ucCounterLoop=0; ucCounterLoop<strlen(pcDestination); ucCounterLoop++)
    {
        printf("%c",pcDestination[ucCounterLoop]);
    }





    printf("\n**************************************************\n\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestOf_ReplaceCharactersInString(void)
{
    printf("\n**************************************************");
    printf("\nReplaceCharactersInString\n ");
    unsigned char ucCounterLoop;
    char pcSource[]= {"AAAAAABBBBBBCCCCAAADDDDD"}  ;
    char cOldChar='A';
    char cNewChar='O';
    printf("\n---PRZED WYWOLANIEM FUNKCJI:---\n  ");
    printf("pcSource:  ");
    for(ucCounterLoop=0; ucCounterLoop<strlen(pcSource); ucCounterLoop++)
    {
        printf("%c",pcSource[ucCounterLoop]);
    }
    ReplaceCharactersInString(pcSource, cOldChar,cNewChar);
    printf("\n---PO WYWOLANIU FUNKCJI:---\n  ");
    printf("pcSource:  ");
    for(ucCounterLoop=0; ucCounterLoop<strlen(pcSource); ucCounterLoop++)
    {
        printf("%c",pcSource[ucCounterLoop]);
    }
    printf("\n**************************************************\n\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestOf_UIntToHexStr(void)
{
    printf("\n**************************************************");
    printf("\nUIntToHexStr\n ");
    unsigned char ucCounterLoop;
    unsigned char ucCtrlLoop;
    unsigned int uiTabTestValues[]= {65000,100,4,1000};
    unsigned char ucNumberOfTests=sizeof(uiTabTestValues)/sizeof(uiTabTestValues[0]);
    unsigned int uiValue;
    char pcSource[100]= {0} ;
    for(ucCtrlLoop=0; ucCtrlLoop<ucNumberOfTests; ucCtrlLoop++)
    {
        uiValue = uiTabTestValues[ucCtrlLoop] ;
        UIntToHexStr (uiValue,  pcSource);
        printf("\nTest %d: \n",ucCtrlLoop+1);
        printf("uiValue: %d  \n",uiValue);
        printf("HexValue:  ");
        for(ucCounterLoop=0; ucCounterLoop<strlen(pcSource); ucCounterLoop++)
        {
            printf("%c",pcSource[ucCounterLoop]);
        }
        printf("\n");
    }
    printf("\n**************************************************\n\n");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestOf_eHexStringToUInt(void)
{
    printf("\n**************************************************");
    printf("\neHexStringToUInt\n ");
    unsigned char ucCounterLoop;
    unsigned char ucCtrlLoop;
    char *pcHex;
    unsigned int uiValue;
    for(ucCtrlLoop=0; ucCtrlLoop<4; ucCtrlLoop++)
    {
        if(ucCtrlLoop==0)pcHex= "0x7";
        else if(ucCtrlLoop==1)pcHex= "0xFFFFFF";
        else if(ucCtrlLoop==2)pcHex= "0x100";
        else pcHex= "0xA";
        eHexStringToUInt(pcHex,&uiValue);
        printf("\nTest %d: \n",ucCtrlLoop+1);
        printf("HexValue: ");
        for(ucCounterLoop=0; ucCounterLoop<strlen(pcHex); ucCounterLoop++)
        {
            printf("%c",pcHex[ucCounterLoop]);
        }
        printf("\n");
        printf("uiValue: %d  \n ",uiValue);
    }
    printf("\n**************************************************\n\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestOf_AppendUIntToString(void)
{
    printf("\n**************************************************");
    printf("\nAppendUIntToString\n ");
    unsigned char ucCounterLoop;
    unsigned char ucCtrlLoop;
    unsigned int uiTabTestValues[]= {65000,100,4,1000};
    unsigned char ucNumberOfTests=sizeof(uiTabTestValues)/sizeof(uiTabTestValues[0]);
    unsigned int uiValue;
    char pcDestination[MAX_BUF_SIZE]= {"ABCD"};

    for(ucCtrlLoop=0; ucCtrlLoop<ucNumberOfTests; ucCtrlLoop++)
    {
        uiValue = uiTabTestValues[ucCtrlLoop];
        AppendUIntToString (uiValue, pcDestination);
        printf("\nTest %d: \n",ucCtrlLoop+1);
        printf("uiValue: %d  \n ",uiValue);
        printf("pcDestination:  ");
        for(ucCounterLoop=0; ucCounterLoop<strlen(pcDestination); ucCounterLoop++)
        {
            printf("%c",pcDestination[ucCounterLoop]);
        }
        printf("\n");
    }
    printf("\n**************************************************\n\n");
}


////////////////////////////////////////////////////////////////////////////////////TESTY_TOKENY/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////TESTY_TOKENY/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////TESTY_TOKENY/////////////////////////////////////////////////////////////////////////

void TestOf_ucFindTokensInString (void)
{
    printf("\n**************************************************");
    printf("\nucFindTokensInString\n ");

    unsigned char ucNumberOfTests=4;
    char *pcSource;
    unsigned char ucCounter;
    unsigned char ucCounterTokenNr;
    unsigned char ucCounterTokenChar;
    for(ucCounter=0; ucCounter<ucNumberOfTests; ucCounter++)
    {
        switch(ucCounter)
        {
        case 0:
            pcSource="0x3343 ABDCDEFGH reset";
            break;
        case 1:
            pcSource="  i ";
            break;
        case 2:
            pcSource="   0x3343 ABDCDEFGH ";
            break;
        case 3:
            pcSource="0x3343   ABDCDEFGH      reset";
            break;
        }
        printf("\nTest %d: \n",ucCounter+1);
        ucTokenNr=ucFindTokensInString(pcSource);
        printf("LICZBA TOKENOW: %d\n",ucTokenNr);
        for(ucCounterTokenNr=0; ucCounterTokenNr<ucTokenNr; ucCounterTokenNr++)
        {

            for(ucCounterTokenChar=0; *(asToken[ucCounterTokenNr].uValue.pcString)!=(NULL); ucCounterTokenChar++)
            {
                printf("%c",*(asToken[ucCounterTokenNr].uValue.pcString++));
            }
            printf("\n");
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestOf_eStringToKeyword(void)
{
    /*
       printf("\n**************************************************");
       printf("\neSringToKeyword\n\n");

       unsigned char ucCounter;
       char* cKeywordCode;
       KeywordCode *keyword;
       char pcSource[]= {"  0x3343   ABDCDEFGH  reset "}  ;
       ucTokenNr=ucFindTokensInString(pcSource);

       for(ucCounter=0; ucCounter<MAX_KEYWORD_NR; ucCounter++)
       {
           printf("\nTest %d: \n",ucCounter+1);
           cKeywordCode=asKeywordList[ucCounter].cString;


           //Result eResult =eStringToKeyword(pcSource,keyword);

           switch( eResult )
           {

           case OK:
           {
               printf("OK");
               break;
           }
           case ERROR:
           {
               printf("ERROR",*keyword);
               break;
           }

           }
       }
       */
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/
void TestOf_DecodeTokens(void)
{
    printf("\n**************************************************");
    printf("\neDecodeTokens\n\n");
    unsigned char ucCounter;
    unsigned char ucCounterToken;
    TokenType eType;
    char pcSource[]= {"  0x3343   ABDCDEFGH  reset "}  ;
    ucTokenNr=ucFindTokensInString(pcSource);

    printf("LICZBA TOKENOW: %d\n",ucTokenNr);
    DecodeTokens();

    for(ucCounter=0; ucCounter<ucTokenNr; ucCounter++)
    {
        eType=asToken[ucCounter].eType;
        printf("TOKEN %d:    ",ucCounter+1);
        for(ucCounterToken=0; *(asToken[ucCounter].uValue.pcString)!=NULL; ucCounterToken++)
        {

            printf("%c",*(asToken[ucCounter].uValue.pcString++));

        }
        printf("\n");
        switch(eType)
        {
        case KEYWORD:
        {
            printf("TYP TOKENA: KEYWORD\n");
            break;
        }
        case NUMBER:
        {
            printf("TYP TOKENA: NUMBER\n");
            break;
        }
        case STRING:
        {
            printf("TYP TOKENA: STRING\n");
            break;
        }
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestOf_DecodeMsg(void)
{
    printf("\n**************************************************");
    printf("\neDecodeMsg\n\n");

    char pcSource[]= {"  0x3343   ABDCDEFGH  reset        "}  ;
    unsigned char ucCounter;
    unsigned char ucCounterToken;
    DecodeMsg(pcSource);
    TokenType eType;
    printf("LICZBA TOKENOW: %d\n\n",ucTokenNr);
    for(ucCounter=0; ucCounter<ucTokenNr; ucCounter++)
    {
        eType=asToken[ucCounter].eType;
        printf("TOKEN %d:    ",ucCounter+1);
        for(ucCounterToken=0; *(asToken[ucCounter].uValue.pcString)!=NULL; ucCounterToken++)
        {
            printf("%c",*(asToken[ucCounter].uValue.pcString++));
        }
        printf("\n");
        switch(eType)
        {
        case KEYWORD:
        {
            printf("TYP TOKENA: KEYWORD\n\n");
            break;
        }
        case NUMBER:
        {
            printf("TYP TOKENA: NUMBER\n\n");
            break;
        }
        case STRING:
        {
            printf("TYP TOKENA: STRING\n\n");
            break;
        }
        }
    }
    printf("**************************************************");
}
/******************************************************************************************************************************************
  MAIN     MAIN      MAIN     MAIN      MAIN     MAIN           MAIN     MAIN         MAIN     MAIN            MAIN     MAIN      MAIN
*******************************************************************************************************************************************/

int main()
{

    ///TESTS
    printf("TESTY FUNKCJI DO OPERACJI NA STRINGACH \n\n\n ");
    // TestOf_CopyString();
    // TestOf_eCompareString();
    // TestOf_AppendString();
    // TestOf_ReplaceCharactersInString();
    // TestOf_UIntToHexStr();
    //TestOf_eHexStringToUInt();
    //TestOf_AppendUIntToString();


    printf("TESTY FUNKCJI DO DEKODOWANIA KOMUNIKATOW\n\n\n ");

    //TestOf_ucFindTokensInString();
    // TestOf_eSringToKeyword();
    // TestOf_DecodeTokens();
     TestOf_DecodeMsg();
    return 0;
}
