
struct SPI_FrameParams{
	unsigned char ucCpha:1;
	unsigned char ucCpol:1;
	unsigned char ucClsbf:1;
	unsigned char ClkDivider;
};

struct SPI_TransactionParams{
	unsigned char *pucBytesForTx; // wskaznik na tablice z bajtami do wyslania
	unsigned char ucNrOfBytesForTx; // ilosc bajtów do wyslania
	unsigned char ucTxBytesOffset; // offset bajtów do wyslania
	unsigned char *pucBytesForRx; // wskaznik na tablice na odebrane bajty
	unsigned char ucNrOfBytesForRx; // ilosc bajtów do odebrania
	unsigned char ucRxBytesOffset; // offset bajtów do odebrania
};




void SPI_ConfigMaster(struct SPI_FrameParams);
	
	
void SPI_ExecuteTransaction(struct SPI_TransactionParams);


