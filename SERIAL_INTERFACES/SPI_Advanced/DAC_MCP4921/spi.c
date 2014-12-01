#include<LPC21xx.h>

#include "spi.h"

//Bit_Masks of content of registers
//SPI Control Register (SPCR - S0SPCR,S1SPCR)
#define SPCR_CPHA (1<<3)
#define SPCR_CPOL (1<<4) 
#define SPCR_MSTR	 (1<<5)
#define SPCR_LSBF  (1<<6)
#define SPCR_SPIE  (1<<7)

#define SPCR_CPHA_POS  (3)
#define SPCR_CPOL_POS  (4) 
#define SPCR_MSTR_POS	 (5)
#define SPCR_LSBF_POS  (6)
#define SPCR_SPIE_POS  (7)

//SPI Status Register (SPSR- S0SPSR,S1SPSR)
#define SPSR_ABRT  (1<<3)
#define SPSR_MODF  (1<<4) 
#define SPSR_ROVR	 (1<<5)
#define SPSR_WCOL  (1<<6)
#define SPSR_SPIF  (1<<7)

//SPI Interrupt Register (SPINT - S0SPINT,S1SPINT)
#define SPINT_SPI_INTERRUPT  (1<<0)

//SCK frequency divider
#define SCK_FREQ_DIVIDER 0x8  /*frequqncy of SCK = pclk/8  -> in our case: 12MHz/8 = 1.5 MHz*/


// Pinout Bit Masks

#define SCK_PinP04SelMask (0<<9|1<<8)
#define MISO_PinP05SelMask (0<<11|1<<10)
#define MOSI_PinP06SelMask (0<<13|1<<12)


/*
static void spiInit(void)
{  
//SPI0 (SCK-P0.4; MISO-P0.5; MOSI-P0.6) 	
	  PINSEL0 = SCK_PinP04SelMask|MISO_PinP05SelMask|MOSI_PinP06SelMask;  //0x1500; //0x5500 with SSEL on pin P0.7
    CS_PinDirMask|=CS_PinMask;
		S0SPCCR= SCK_FREQ_DIVIDER; // frequency of SCK = pclk/8 	
	  S0SPCR = (SPCR_MSTR);//~SPCR_CPOL ,~SPCR_CPHA
}


static void DAC_MCP4921_Set(unsigned int uiVoltage){
	spiInit();
  CS_PIN_SET; 
	S0SPDR=(uiVoltage>>8)&0xFF;
	// Wait for transfer to be completed 
	while(!(S0SPSR & SPSR_SPIF)){};
	S0SPDR=(uiVoltage)&0xFF;
	// Wait for transfer to be completed 
	while(!(S0SPSR & SPSR_SPIF)){};
	CS_PIN_RESET;

}
*/
void SPI_ConfigMaster(struct SPI_FrameParams spiFrameParams){
  //hardware dependent code
	//SPI0 (SCK-P0.4; MISO-P0.5; MOSI-P0.6) 	
	  PINSEL0 = 0x5500; //SCK_PinP04SelMask|MISO_PinP05SelMask|MOSI_PinP06SelMask;  //0x1500; //0x5500 with SSEL on pin P0.7
    S0SPCCR = spiFrameParams.ClkDivider;
	  S0SPCR=0;
	  S0SPCR= (spiFrameParams.ucCpha<<SPCR_CPHA_POS)|(spiFrameParams.ucCpol<<SPCR_CPOL_POS)|(spiFrameParams.ucClsbf<<SPCR_LSBF_POS);
	  S0SPCR|= (SPCR_MSTR);
}
	

void SPI_ExecuteTransaction(struct SPI_TransactionParams spiTransactionParams ){
  
	unsigned char ucSumOfNrOfBytesRx= spiTransactionParams.ucNrOfBytesForRx+ spiTransactionParams.ucRxBytesOffset;
	unsigned char ucSumOfNrOfBytesTx= spiTransactionParams.ucNrOfBytesForTx+ spiTransactionParams.ucTxBytesOffset;
	unsigned char nrOfBytes=  (ucSumOfNrOfBytesTx>ucSumOfNrOfBytesRx)?ucSumOfNrOfBytesTx:ucSumOfNrOfBytesRx;
	
	 unsigned char ucBytesCounter;
   for(ucBytesCounter=0;ucBytesCounter<nrOfBytes;ucBytesCounter++){
     if(ucBytesCounter>=spiTransactionParams.ucTxBytesOffset){
			 S0SPDR= *(spiTransactionParams.pucBytesForTx++);
		 }
		 else {
			 S0SPDR= *(spiTransactionParams.pucBytesForTx); //dummy data
     }
	// Wait for transfer to be completed 
	   while(!(S0SPSR & SPSR_SPIF)){};
	   
			 if(ucBytesCounter>=spiTransactionParams.ucRxBytesOffset){
				*(spiTransactionParams.pucBytesForRx++) = S0SPDR; 
		   }
		   else {
				*(spiTransactionParams.pucBytesForRx) = S0SPDR;  // dummy read to clear flags
     }
	}

	
	
}


