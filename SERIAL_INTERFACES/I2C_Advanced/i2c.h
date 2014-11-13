typedef enum {TX,RX,RX_AFTER_TX,TX_AFTER_RX } eI2CTransMode;

struct I2C_Params{
	eI2CTransMode eI2CTransmisionMode;
	unsigned char ucSlaveAddress;
	unsigned char *pucBytesForTx;
	unsigned char ucNrOfBytesForTx;
	unsigned char *pucBytesForRx;
	unsigned char ucNrOfBytesForRx;
	unsigned char ucDone;
};

void I2C_Init(void);


void I2C_ExecuteTransaction(struct I2C_Params i2cParams);
