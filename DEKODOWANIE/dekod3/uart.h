#define RECEIVER_SIZE 100

enum eRecieverStatus { EMPTY, READY, OVERFLOW};

void UART_InitWithInt(unsigned int uiBaudRate);

void Reciever_GetStringCopy(char * ucDestination);

enum eRecieverStatus eReciever_GetStatus(void);


//////////////////////////////////////////
