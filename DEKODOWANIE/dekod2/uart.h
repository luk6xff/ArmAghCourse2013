#define RECEIVER_SIZE 10

enum eRecieverStatus { EMPTY, READY, OVERFLOW};

void UART_InitWithInt(unsigned int uiBaudRate);

void Reciever_GetStringCopy(char * ucDestination);

enum eRecieverStatus eReciever_GetStatus(void);


//////////////////////////////////////////
typedef enum {DIFFERENT,EQUAL} CompResult;

CompResult eCompareString(char pcStr1[], char pcStr2[]);
