#include<LPC21xx.h>
#include "uart.h"
#include "string.h"

/************ UART ************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

////////transmitting
/***************** private variables ********************/


struct TransmiterBuffer{
	char cData[TRANSMITER_SIZE]; 
	enum eTransmiterStatus eStatus;
	unsigned char fLastCharacter;
	unsigned char ucCharCtr;
}sTransmiterBuffer;

 

/***************** functions ********************/
char Transmiter_GetCharacterFromBuffer(void){
	char cRetValue;
	
if(sTransmiterBuffer.cData[sTransmiterBuffer.ucCharCtr]=='\0'){
	if(sTransmiterBuffer.fLastCharacter!='\r') {
			sTransmiterBuffer.fLastCharacter= '\r';	
		  sTransmiterBuffer.ucCharCtr++;
	}
	else{
			sTransmiterBuffer.fLastCharacter= '\0';	
			sTransmiterBuffer.eStatus=FREE;
		  sTransmiterBuffer.ucCharCtr=0;
   }
		cRetValue=sTransmiterBuffer.fLastCharacter;
   	 
	}
	else{
		cRetValue= sTransmiterBuffer.cData[sTransmiterBuffer.ucCharCtr];
		sTransmiterBuffer.ucCharCtr++;
		if(sTransmiterBuffer.ucCharCtr>TRANSMITER_SIZE){
			sTransmiterBuffer.ucCharCtr=0;
		  sTransmiterBuffer.eStatus=FREE;
		}
	}
  return cRetValue;
}
//////////////////////////////////////////////////////////////
void Transmiter_SendString(unsigned char cString[]) {

    CopyString((char*)cString,sTransmiterBuffer.cData );
    U0THR= sTransmiterBuffer.cData[0];
	  sTransmiterBuffer.ucCharCtr=1;
	  sTransmiterBuffer.eStatus=BUSY;
}
//////////////////////////////////////////////////////////////
enum eTransmiterStatus Transmiter_GetStatus(void) {
	return sTransmiterBuffer.eStatus;
}

////////////// Zmienne globalne //////////////////////////////

struct RecieverBuffer
{
    char cData[RECEIVER_SIZE];
    unsigned char ucCharCtr;
    enum eRecieverStatus eStatus;
} sRxBuffer;


//////////////////////////////////////////////////////////////

static void Reciever_PutCharacterToBuffer(char cCharacter)
{
    if(sRxBuffer.ucCharCtr==RECEIVER_SIZE)
    {
        sRxBuffer.eStatus=OVERFLOW;
    }
    else if(cCharacter==13)  //enter CHANGE TO '\r'
    {
        sRxBuffer.cData[sRxBuffer.ucCharCtr]='\0';
        sRxBuffer.eStatus=READY;
        sRxBuffer.ucCharCtr=0;
    }
    else
    {
        sRxBuffer.cData[sRxBuffer.ucCharCtr]=cCharacter;
        sRxBuffer.ucCharCtr++;
        sRxBuffer.eStatus=EMPTY;
    }
}

//////////////////////////////////////////////////////////////
__irq void UART0_Interrupt (void) {
   // jesli przerwanie z odbiornika (Rx)
   
   unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

   if      ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
      Reciever_PutCharacterToBuffer(U0RBR);
   } 
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {  
		 if(Transmiter_GetStatus()==BUSY) 
      U0THR=Transmiter_GetCharacterFromBuffer();
   }

   VICVectAddr = 0; // Acknowledge Interrupt
}

//////////////////////////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate){

   // UART0
   PINSEL0 = PINSEL0 | (1<<2|1<<0);                                     // ustawic pina na odbiornik uart0
   U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // dlugosc slowa, DLAB = 1
   U0DLL   = ((15000000)/16)/uiBaudRate;                      // predkosc transmisji
   U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
   U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE|mTHRE_INTERRUPT_ENABLE ;               // wlaczam przerwanie Receive/ Transmitting Data dla UART0. 

   // INT
   VICVectAddr2  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address 
   VICVectCntl2 = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt  //enable and set 2nd slot
   VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
	 sRxBuffer.ucCharCtr=0;
	 sTransmiterBuffer.ucCharCtr=0;
}

//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
enum eRecieverStatus eReciever_GetStatus(void)
{
   return sRxBuffer.eStatus;
}

//////////////////////////////////////////////////////////////
void Reciever_GetStringCopy(char * ucDestination)
{
    unsigned char ucCharCounter;
    for(ucCharCounter=0; sRxBuffer.cData[ucCharCounter]!='\0'; ucCharCounter++)
    {
				ucDestination[ucCharCounter]=sRxBuffer.cData[ucCharCounter];
		}
		ucDestination[ucCharCounter++]='\0';
    sRxBuffer.eStatus=EMPTY;
}

//////////////////////////////////////////////////////////////

