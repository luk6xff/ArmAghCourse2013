//STRINGI

///typy uzywane w kodzie
typedef enum {DIFFERENT,EQUAL} CompResult;
typedef enum {OK,ERROR} Result;

void CopyString(char pcSource[], char pcDestination[]);
CompResult eCompareString(char pcStr1[], char pcStr2[]);
void AppendString (char pcSourceStr[],char pcDestinationStr[]);
void AppendString (char pcSourceStr[],char pcDestinationStr[]);
void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar);
void UIntToHexStr (unsigned int uiValue, char pcStr[]);
Result eHexStringToUInt(char pcStr[],unsigned int *puiValue);
void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]);
