#include<LPC21xx.h>
#include "i2c.h"
#include "mc24lc64.h"

//private functions and memebers
static struct I2C_Params i2cParams;
static unsigned char txBuf[10];
static unsigned char rxBuf[10];
static unsigned char ucPCF8574_Input;



void MC24LC64_ByteWrite( unsigned int WordAddress, unsigned char Data){
  txBuf[0]=Data;
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

void MC24LC64_RandomRead (unsigned int WordAddress){
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
