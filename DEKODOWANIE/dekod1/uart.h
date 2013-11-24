

void UART_InitWithInt(unsigned int uiBaudRate);

enum eRecieverStatus { EMPTY, READY, OVERFLOW};

void Reciever_PutCharacterToBuffer(char cCharacter);

void Reciever_GetStringCopy(char * ucDestination);

enum eRecieverStatus eReciever_GetStatus(void);
