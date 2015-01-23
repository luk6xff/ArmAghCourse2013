#include<LPC21xx.h>
#include "i2c.h"
#include "pcf8574.h"

//private functions and memebers
static struct I2C_Params i2cParams;
static unsigned char txBuf[10];
static unsigned char rxBuf[10];
static unsigned char ucPCF8574_Input;


/*
	enum I2CTransmisionMode eI2CTransmisionMode;
	unsigned char ucSlaveAddress;
	unsigned char *pucBytesForTx;
	unsigned char ucNrOfBytesForTx;
	unsigned char *pucBytesForRx;
	unsigned char ucNrOfBytesForRx;
	unsigned char ucDone;
*/


unsigned char getPCF8574_Input(void){
				return ucPCF8574_Input; 
}

void PCF8574_Init(void){

	
}



void PCF8574_Write(unsigned char ucData){
  txBuf[0]=ucData;
	i2cParams.eI2CTransmisionMode=TX;
	i2cParams.ucSlaveAddress=0x40;
	i2cParams.pucBytesForTx=txBuf;
	i2cParams.ucNrOfBytesForTx=1;
	i2cParams.pucBytesForRx=rxBuf;
	i2cParams.ucNrOfBytesForRx=0;
	i2cParams.ucDone =0;
	I2C_ExecuteTransaction(i2cParams);
	//while(i2cParams.ucDone);


	
}


void PCF8574_Read(void){
	i2cParams.eI2CTransmisionMode=TX;
	i2cParams.ucSlaveAddress=0x40;
	i2cParams.pucBytesForTx=txBuf;
	i2cParams.ucNrOfBytesForTx=0;
	i2cParams.pucBytesForRx=rxBuf;
	i2cParams.ucNrOfBytesForRx=1;
	i2cParams.ucDone =0;
	I2C_ExecuteTransaction(i2cParams);
  ucPCF8574_Input= rxBuf[0];

}

