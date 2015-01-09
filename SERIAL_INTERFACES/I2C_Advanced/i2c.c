#include<LPC21xx.h>
#include "i2c.h"
#include "led.h" //DEBUG

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_I2C_CHANNEL_NR 8
// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE 0x00000020

static unsigned char ucI2cAddr;
static  unsigned char ucI2cData;
static unsigned char *i2cDataBuffer;
static int i2cDataLenWrite;
static int i2cDataLenRead;
static int i2cDataCnt;
static volatile unsigned char ucEndOfTransmissionFlag;

__irq void I2C_Interrupt(void){

	unsigned int temp=0;
	temp=I2STAT;
	switch(temp)
	{
	  case (0x00):    //BUS ERROR 
			I2CONSET= 0x14;
		  ucEndOfTransmissionFlag= 0;
		 break;
		
		case (0x08):     			//START TX
		case (0x10):          //REPEATED START_TX	
			I2CONCLR =0x20; 		//Clear start bit
		  I2DAT = ucI2cAddr; //Send address and write Bit	
		 break;
		
		case (0x18):   			//SLAWTX, ADDRESS+W, ACK
		case (0x28):        //DATA TXSENT, ACK
		{
        if (i2cDataLenWrite && (i2cDataCnt < i2cDataLenWrite))
        {
          I2DAT = i2cDataBuffer [i2cDataCnt++];
          I2CONCLR = 0x20;
        }
        else
        {
          if (!i2cDataLenRead)
          {
            I2CONCLR = 0x20;
            I2CONSET = 0x10; //stop condition
						temp = 0;
            ucEndOfTransmissionFlag= 0;
          }
          else
          {
            ucI2cAddr |= 0x01;
            i2cDataCnt = 0;
            I2CONSET = 0x20; // Resend Start Condition
          }
        }
     }
		 break;
		 
	  case (0x58):  // Data received, NOT ACK
      {
        i2cDataBuffer [i2cDataCnt++] = I2DAT;
        temp = 0;
      }	
		case (0x20):        // Slave address +W,Not ACK
		case (0x48):        // Slave address +R, NOT ACK
		case (0x30):        // 0x30
      {
        I2CONCLR = 0x20;
        I2CONSET = 0x10;
        ucEndOfTransmissionFlag =0;
      }
		 break;
			
	  case (0x38): // I2CERR_ARBLOST
      {
        I2CONSET = 0x20;
      }
     break;
		
	  //Receive byte conditions 
						
		case (0x50):      // Data received, ACK
				i2cDataBuffer [i2cDataCnt++] = I2DAT;
		case (0x40):      // Slave address +R, ACK
		  {
        if (i2cDataCnt < i2cDataLenRead - 1)
        {
          I2CONCLR = 0x20;
          I2CONSET = 0x04; // Enable ACK for data byte
        }
        else
          I2CONCLR = 0x20 | 0x04;
      }
      break;
		
		case (0xF8):
		 break;
				
		default: 
      {
        I2CONCLR = 0x40;
        ucEndOfTransmissionFlag =0;
      }
      break;
	}
	I2CONCLR = 0x08; // Clear I2C interrupt flag
	VICVectAddr = 0; // Acknowledge Interrupt
}



void I2C_Init(void){

	/* Remap interrupt vectors to SRAM */
	//MEMMAP=0x2;
  Led_Init();  //DEBUG
  //PINSEL0 /************* GPIO-0.2 SCL  , GPIO-0.3 SDA ************/
	PINSEL0=0x50;
	/* Initialize I2C */
	I2CONCLR=0x6C; /* clearing all flags */
	I2CONSET=0x40; /* enabling I2C */
	I2SCLH=0x80; /* 100 KHz */
	I2SCLL=0x80;
	/* Initialize VIC for I2C use */

	/* ISR address written to the respective address register*/
	VICVectCntl1  = 0x29 ;  		// Enable Slot 0 and assign it to I2C interrupt channel
	VICVectAddr1 =(unsigned long)I2C_Interrupt; 	   							// Set to Slot 0 Address of Interrupt Service Routine 
	VICIntEnable |= 0x200;            					// Enable I2C interrupt channel 
}



void I2C_ExecuteTransaction(struct I2C_Params i2cParams){
	
	ucI2cAddr = i2cParams.ucSlaveAddress;
	ucEndOfTransmissionFlag=1;
	
	if(i2cParams.eI2CTransmisionMode==TX){
		ucI2cAddr &= ~0x01;
		i2cDataBuffer =i2cParams.pucBytesForTx;
		i2cDataLenWrite = i2cParams.ucNrOfBytesForTx;
		i2cDataLenRead = i2cParams.ucNrOfBytesForRx;
		i2cDataCnt = 0;
		I2CONCLR = 0xFF; //Clear all I2C settings
		I2CONSET = 0x40; //Enable the I2C interface
		I2CONSET = 0x20; //Start Condition
		while(ucEndOfTransmissionFlag); //for tests
	

	}
	else if(i2cParams.eI2CTransmisionMode==RX){
		ucI2cAddr |= 0x01;
		i2cDataBuffer =i2cParams.pucBytesForRx;
		i2cDataLenWrite = i2cParams.ucNrOfBytesForTx;
		i2cDataLenRead = i2cParams.ucNrOfBytesForRx;
		i2cDataCnt = 0;
		I2CONCLR = 0xFF; //Clear all I2C settings
		I2CONSET = 0x40; //Enable the I2C interface
		I2CONSET = 0x20; //Start Condition
		while(ucEndOfTransmissionFlag); //for tests
	}
	

	
	
}


