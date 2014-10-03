#define MAX_TOKEN_NR 2 ///maksymalna dopuszczalna ilosc tokenow
#define MAX_KEYWORD_STRING_LTH 10 /// mksymalna dlugosc komendy



void DecodeMsg(char *pcString);

////////////////////////////////////////////////////
typedef enum {CALLIB, GOTO} KeywordCode;

struct Keyword
{
    KeywordCode eCode;
    char cString[MAX_KEYWORD_STRING_LTH + 1];
};
////////////////////////////////////////////////////
 union TokenValue
{
    KeywordCode eKeyword; /// jezeli KEYWORD
    unsigned int uiNumber; /// jezeli NUMBER
    char * pcString; /// jezeli STRING
};

typedef enum { KEYWORD, NUMBER, STRING} TokenType;

struct Token
{
    TokenType eType; /// KEYWORD, NUMBER, STRING
    union TokenValue uValue; /// enum, unsigned int, char*
};

