#include<LPC21xx.h>

#include "spi.h"

//Bit_Masks of content of registers
//SPI Control Register (SPCR - S0SPCR,S1SPCR)
#define SPCR_CPHA (1<<3)
#define SPCR_CPOL (1<<4) 
#define SPCR_MSTR	 (1<<5)
#define SPCR_LSBF  (1<<6)
#define SPCR_SPIE  (1<<7)

//SPI Status Register (SPSR- S0SPSR,S1SPSR)
#define SPSR_ABRT  (1<<3)
#define SPSR_MODF  (1<<4) 
#define SPSR_ROVR	 (1<<5)
#define SPSR_WCOL  (1<<6)
#define SPSR_SPIF  (1<<7)

//SPI Interrupt Register (SPINT - S0SPINT,S1SPINT)
#define SPINT_SPI_INTERRUPT  (1<<0)

//SCK frequency divider
#define SCK_FREQ_DIVIDER 254  /*frequqncy of SCK = pclk/8  -> in our case: 12MHz/8 = 1.5 MHz*/


// Pinout Bit Masks
#define CS_PinDirMask IO0DIR
#define CS_PinMask 1<<10

#define CS_PIN_SET   (IO0SET = CS_PinMask)
#define CS_PIN_RESET (IO0CLR = CS_PinMask)



void spiInit(void)
{  
//SPI0 (SCK-P0.4; MISO-P0.5; MOSI-P0.6) 	
	  PINSEL0 = 0x5500; //0x5500 with SSEL on pin P0.7
    CS_PinDirMask|=CS_PinMask;
		S0SPCCR= SCK_FREQ_DIVIDER; /* frequency of SCK = pclk/8 */	
	  S0SPCR = (SPCR_MSTR);/*|(SPCR_CPOL)|(SPCR_CPHA)*/;
	  CS_PIN_SET;
}


void DAC_MCP4921_Set(unsigned int uiVoltage){
	//spiInit();
  CS_PIN_RESET; 
	S0SPDR=((uiVoltage>>8)&0xFF)|0x30;
	/* Wait for transfer to be completed */
	while(!(S0SPSR & SPSR_SPIF)){};
	S0SPDR=(uiVoltage)&0xFF;
	/* Wait for transfer to be completed */
	while(!(S0SPSR & SPSR_SPIF)){};
	//S0SPDR=S0SPDR; //read dummy data to clear flags
	CS_PIN_SET;

}

 
