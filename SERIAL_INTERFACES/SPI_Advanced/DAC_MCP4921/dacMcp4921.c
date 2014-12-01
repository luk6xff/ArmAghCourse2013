#include<LPC21xx.h>
#include "spi.h"
#include "dacMcp4921.h"


#define CS_PinDirMask IO0DIR
#define CS_PinMask 1<<10
#define CS_PIN_SET   (IO0SET = CS_PinMask)
#define CS_PIN_RESET (IO0CLR = CS_PinMask)

//private functions and memebers
static struct SPI_FrameParams spiFrameParams;
static struct SPI_TransactionParams spiTransactionParams;
unsigned char txBuf[10];
unsigned char rxBuf[10];

static void DAC_MCP4921_InitCSPin(void){
    CS_PinDirMask|=CS_PinMask;
    CS_PIN_SET;
}


//public functions
void DAC_MCP4921_Init(void){

	spiFrameParams.ucCpha=0;
	spiFrameParams.ucCpol=0;
	spiFrameParams.ucClsbf=0;
	spiFrameParams.ClkDivider=8;
	SPI_ConfigMaster(spiFrameParams);
	
	spiTransactionParams.ucNrOfBytesForRx=0;
	spiTransactionParams.ucNrOfBytesForTx=2;
	spiTransactionParams.ucRxBytesOffset=0;
	spiTransactionParams.ucTxBytesOffset=0;
	spiTransactionParams.pucBytesForTx= txBuf;
	spiTransactionParams.pucBytesForRx= rxBuf;
  DAC_MCP4921_InitCSPin();

}

void DAC_MCP4921_Set_Adv(unsigned int uiData){
    CS_PIN_RESET;
    txBuf[0]= ((uiData>>8)&0xFF)|0x30;
	  txBuf[1]=  (uiData)&0xFF;
    SPI_ExecuteTransaction(spiTransactionParams );
	
	  DAC_MCP4921_InitCSPin();
}


