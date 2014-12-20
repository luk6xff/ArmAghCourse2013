#include<LPC21xx.h>
#include "spi.h"
#include "mcp23S09.h"
#include "led.h"  //DEBUG

#define CS_PinDirMask IO0DIR
#define CS_PinMask 1<<10
#define CS_PIN_SET   (IO0SET = CS_PinMask)
#define CS_PIN_RESET (IO0CLR = CS_PinMask)

//mcp23s09 registers mask
#define DEVICE_OPCODE_WRITE 0x40
#define DEVICE_OPCODE_READ 0x41

#define IODIR_REG_ADDR 0x00
#define GPIO_REG_ADDR 0x09


//private functions and memebers
static struct SPI_FrameParams spiFrameParams;
static struct SPI_TransactionParams spiTransactionParams;
static unsigned char txBuf[10];
static unsigned char rxBuf[10];

static void Port_MCP23S09_InitCSPin(void){
    CS_PinDirMask|=CS_PinMask;
    CS_PIN_SET;
}


//public functions
//public functions
void MCP23S09_Init(void){

	spiFrameParams.ucCpha=0;
	spiFrameParams.ucCpol=0;
	spiFrameParams.ucClsbf=0;
	spiFrameParams.ClkDivider=8;
	SPI_ConfigMaster(spiFrameParams);
  Port_MCP23S09_InitCSPin();

}



void Port_MCP23S09_Set(unsigned char ucData){
	
	txBuf[0]= DEVICE_OPCODE_WRITE;
	txBuf[1]= IODIR_REG_ADDR;
	txBuf[2]= 0; // set all pins as outputs
	spiTransactionParams.ucNrOfBytesForRx=0;
	spiTransactionParams.ucNrOfBytesForTx=3;
	spiTransactionParams.ucRxBytesOffset=0;
	spiTransactionParams.ucTxBytesOffset=0;
	spiTransactionParams.pucBytesForTx= txBuf;
	spiTransactionParams.pucBytesForRx= rxBuf;
	
	CS_PIN_RESET;
  SPI_ExecuteTransaction(spiTransactionParams );
	Led_StepRight();//DEBUG
	Port_MCP23S09_InitCSPin();
	
	
	txBuf[1]= GPIO_REG_ADDR;
	txBuf[2]= ucData; // set given pins to high state
	spiTransactionParams.pucBytesForTx= txBuf;
	spiTransactionParams.pucBytesForRx= rxBuf;
	
	CS_PIN_RESET;
  SPI_ExecuteTransaction(spiTransactionParams );
	Port_MCP23S09_InitCSPin();
	
}
	
	
unsigned char Port_MCP23S09_Get(void){
	
	txBuf[0]= DEVICE_OPCODE_WRITE;
	txBuf[1]= IODIR_REG_ADDR;
	txBuf[2]= 0xFF; // set all pins as inputs
	spiTransactionParams.ucNrOfBytesForRx=0;
	spiTransactionParams.ucNrOfBytesForTx=3;
	spiTransactionParams.ucRxBytesOffset=0;
	spiTransactionParams.ucTxBytesOffset=0;
	spiTransactionParams.pucBytesForTx= txBuf;
	spiTransactionParams.pucBytesForRx= rxBuf;
	
	CS_PIN_RESET;
  SPI_ExecuteTransaction(spiTransactionParams );
	Port_MCP23S09_InitCSPin();
	
	txBuf[0]= DEVICE_OPCODE_READ;
	txBuf[1]= GPIO_REG_ADDR;
	spiTransactionParams.ucNrOfBytesForRx=1;
	spiTransactionParams.ucNrOfBytesForTx=2;
	spiTransactionParams.ucRxBytesOffset=2;
	spiTransactionParams.ucTxBytesOffset=0;
	spiTransactionParams.pucBytesForTx= txBuf;
	spiTransactionParams.pucBytesForRx= rxBuf;
	
	CS_PIN_RESET;
  SPI_ExecuteTransaction(spiTransactionParams );
	Led_StepLeft();//DEBUG
	Port_MCP23S09_InitCSPin();
	
 return rxBuf[0];

}