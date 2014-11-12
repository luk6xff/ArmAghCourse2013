#include "string.h"
//STRINGI
//////////////////////////////////////////////////////////////////////////////////////
void CopyString(char pcSource[], char pcDestination[])
{
    unsigned char ucCharCounter;
    for(ucCharCounter=0; pcSource[ucCharCounter]!='\0'; ucCharCounter++)
    {
        pcDestination[ucCharCounter]=pcSource[ucCharCounter];
    }
    pcDestination[ucCharCounter++]='\0';
}
//////////////////////////////////////////////////////////////////////////////////////


CompResult eCompareString(char pcStr1[], char pcStr2[])
{
    unsigned char ucCharCounter;
    for(ucCharCounter=0; pcStr1[ucCharCounter]!='\0'; ucCharCounter++)
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
    for(ucCharCounter=0; pcDestinationStr[ucCharCounter]!='\0'; ucCharCounter++)
    {
        ;
    }
    CopyString(pcSourceStr,&pcDestinationStr[ucCharCounter]);

}
//////////////////////////////////////////////////////////////////////////////////////

void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar)
{

    unsigned char ucCharCounter;
    for(ucCharCounter=0; pcString[ucCharCounter]!='\0'; ucCharCounter++)
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
    pcStr[6]='\0';

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
    unsigned char ucCharCounter;
    char cSingleCharOfString;
    *puiValue=0;
    if((pcStr[0]!='0')||(pcStr[1]!='x')||(pcStr[2]=='\0') )
    {
        return ERROR;
    }
    for (ucCharCounter=2; pcStr[ucCharCounter]!='\0'; ucCharCounter++)
    {
        cSingleCharOfString=pcStr[ucCharCounter];
        if ((ucCharCounter>6&&pcStr[ucCharCounter]!='\0'))
        {
            return ERROR;
        }
        else if((cSingleCharOfString>='0')&&(cSingleCharOfString<='9'))
        {
					cSingleCharOfString-='0';
        }
        else if((cSingleCharOfString>='A')&&(cSingleCharOfString<='F'))
        {
					cSingleCharOfString=cSingleCharOfString-'A'+10;
        }
        else return ERROR;
        *puiValue=((*puiValue<<4)|(cSingleCharOfString));
    }
    return OK;
}

///////////////////////////////////////////////////////////////////////////////
void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[])
{
    unsigned char ucCharCounter;
    for(ucCharCounter=0; pcDestinationStr[ucCharCounter]!='\0'; ucCharCounter++)
    {
        ;
    }
    UIntToHexStr (uiValue, &pcDestinationStr[ucCharCounter]);
}
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////TOKENY///////////////////////////////////////////
