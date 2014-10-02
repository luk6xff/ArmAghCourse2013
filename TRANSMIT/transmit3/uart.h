#define RECEIVER_SIZE 100
#define TRANSMITER_SIZE 100

enum eRecieverStatus { EMPTY, READY, OVERFLOW};

void UART_InitWithInt(unsigned int uiBaudRate);

void Reciever_GetStringCopy(char * ucDestination);

enum eRecieverStatus eReciever_GetStatus(void);


//////////////////////////////////////////
enum eTransmiterStatus {FREE, BUSY};

void Transmiter_SendString(unsigned char cString[]);

enum eTransmiterStatus Transmiter_GetStatus(void);
