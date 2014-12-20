#include<LPC21xx.h>
#include "i2c.h"
#include "led.h" //DEBUG

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_I2C_CHANNEL_NR 8
// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE 0x00000020

unsigned char ucI2cAddr;
unsigned char ucI2cData;
unsigned char ucPCF8574_Input;
static unsigned char waitForEndTransmission;

__irq void I2C_Interrupt(void){

	unsigned int temp=0;
	temp=I2STAT;
	switch(temp)
	{
	
		case (0x08):     			//Start Bit
			I2CONCLR =0x20; 		//Clear start bit
		  I2DAT = ucI2cAddr; //Send address and write Bit
			Led_StepRight();
		
		break;
		
		case (0x18):   			//Slave address+W, ACK
			I2DAT = ucI2cData;  //Write data to tx register
		break;
		
		case (0x20):          //Slave address +W,Not ACK
			I2DAT = ucI2cAddr; // Resend address and write bit 
		break;
		
		case (0x28):     //data sent, ACK
		  I2CONSET = 0x10; //Stop Condition
			Led_StepLeft();
			Led_StepLeft();
		break;
		
		case (0x40):      // Slave address +R, ACK
			I2CONSET =0x04; // Enable ACK for data byte
		break;
		
		case (0x48):      // Slave address +R, NOT ACK
			I2CONSET =0x20; // Resend Start Condition
		break;
		
		case (0x50):      // Data received, ACK
			ucPCF8574_Input= I2DAT;
			I2CONSET =0x10; // Stop Condition
			waitForEndTransmission =0;        //Signal end of I2C Activity
		  Led_StepLeft();
			Led_StepLeft();
			Led_StepLeft();
		break;
		
		case (0x58):      // Data received, NOT ACK
			I2CONSET =0x20; // Resend Start Condition
		break;
		
		default: 
			break;
	}
	I2CONCLR = 0x08; // Clear I2C interrupt flag
	VICVectAddr = 0; // Acknowledge Interrupt
}



void I2C_Init(void){

	/* Remap interrupt vectors to SRAM */
	//MEMMAP=0x2;
  Led_Init();
//PINSEL0 GPIO-0.2 SCL  , GPIO-0.3 SDA
	PINSEL0=0x50;
	/* Initialize I2C */
	I2CONCLR=0x6C; /* clearing all flags */
	I2CONSET=0x40; /* enabling I2C */
	I2SCLH=0x80; /* 100 KHz */
	I2SCLL=0x80;
	/* Initialize VIC for I2C use */

	/* ISR address written to the respective address register*/
	//VICVectCntl3  = mIRQ_SLOT_ENABLE | VIC_I2C_CHANNEL_NR ;  		// Enable Slot 0 and assign it to I2C interrupt channel
	//VICVectAddr3  =(unsigned long)I2C_Interrupt; 	   							// Set to Slot 0 Address of Interrupt Service Routine 
	//VICIntEnable |= (1 << VIC_I2C_CHANNEL_NR);            					// Enable I2C interrupt channel 
	VICVectCntl1  = 0x29 ;  		// Enable Slot 0 and assign it to I2C interrupt channel
	VICVectAddr1 =(unsigned long)I2C_Interrupt; 	   							// Set to Slot 0 Address of Interrupt Service Routine 
	VICIntEnable |= 0x200;            					// Enable I2C interrupt channel 
}


void PCF8574_Write(unsigned char ucData){
	
	ucI2cAddr =0x40;
	int i=0;
	//for(i =0; i<0xFF;i++){
		waitForEndTransmission=1;
	//	ucI2cAddr =i;
		ucI2cData = ucData;
		I2CONCLR = 0xFF; //Clear all I2C settings
		I2CONSET = 0x40; //Enable the I2C interface
		I2CONSET = 0x20; //Start Condition
	//	while(waitForEndTransmission);
	//}

	



}