/**************************************************************************/
/*! 
    \file     i2cInt.c
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 September 2009
    \version  1.00
*/
/**************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#include <string.h>

#include "i2c.h"

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
typedef enum
{
  ACKPOLL_NO = 0,
  ACKPOLL_YES
}
ackPoll_e;

static volatile int i2cBusInUse;
static U8 i2cAddress;
static U8 *i2cDataBuffer;
static int i2cDataLenWrite;
static int i2cDataLenRead;
static unsigned int i2cTimeoutInTicks = CFG_I2C_DEFAULT_TIMEOUT; // / portTICK_RATE_MS;
static int i2cDataCnt;
static ackPoll_e i2cAckPoll = ACKPOLL_NO;
typedef enum
{
  I2CERR_NONE = 0,
  I2CERR_BUSY,
  I2CERR_EMPTY,
  I2CERR_TIMEOUT,
  I2CERR_TIMEOUTWC,
  I2CERR_TIMEOUTACKPOLL,
  I2CERR_NOTIMPLEMENTED,
  I2CERR_OTHER,
  I2CERR_BUSERROR,
  I2CERR_BUSERRORx       = 0x00,
  I2CERR_STARTTX         = 0x08,
  I2CERR_REPEATEDSTARTTX = 0x10,
  I2CERR_SLAWTX_ACKRX    = 0x18,
  I2CERR_SLAWTX_NACKRX   = 0x20,
  I2CERR_DATTX_ACKRX     = 0x28,
  I2CERR_DATTX_NACKRX    = 0x30,
  I2CERR_ARBLOST         = 0x38,
  I2CERR_SLARTX_ACKRX    = 0x40,
  I2CERR_SLARTX_NACKRX   = 0x48,
  I2CERR_DATRX_ACKTX     = 0x50,
  I2CERR_DATRX_NACKTX    = 0x58,
  I2CERR_NOINFO          = 0xf8
}
i2cErr_e;



#if defined CFG_I2C_DEBUG && CFG_I2C_DEBUG == 1
static U8 i2cTransactions [64];
static int i2cTransactionsIndex;
#define i2cDebug(x) do { if (i2cAckPoll && (i2cTransactionsIndex < (int) sizeof (i2cTransactions))) i2cTransactions [i2cTransactionsIndex++] = x; } while (0)
#define i2cDebugReset() do { i2cTransactionsIndex = 0; } while (0)
#else
#define i2cDebug(x) do { } while (0)
#define i2cDebugReset() do { } while (0)
#endif

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
static void i2cISR_Handler (void)
{
  i2cErrno = (I2C0_STAT & I2C_STAT_STATMASK);
  i2cDebug (i2cErrno);

  switch (i2cErrno)
  {
    //
    //  Transmit conditions
    //
    case I2CERR_BUSERRORx : // 0x00
      {
        I2C0_CONSET = I2C_CONSET_STO | I2C_CONSET_AA;
        i2cAckPoll = ACKPOLL_NO;
        i2cBusInUse = FALSE;
      }
      break;

    case I2CERR_STARTTX : // 0x08
    case I2CERR_REPEATEDSTARTTX : // 0x10
      {
        i2cDebug (i2cAddress);
        I2C0_DAT = i2cAddress;
      }
      break;

    case I2CERR_SLAWTX_ACKRX : // 0x18
    case I2CERR_DATTX_ACKRX : // 0x28
      {
        i2cAckPoll = ACKPOLL_NO;

        if (i2cDataLenWrite && (i2cDataCnt < i2cDataLenWrite))
        {
          i2cDebug (i2cDataBuffer [i2cDataCnt]);
          I2C0_DAT = i2cDataBuffer [i2cDataCnt++];
          I2C0_CONCLR = I2C_CONCLR_STAC;
        }
        else
        {
          if (!i2cDataLenRead)
          {
            i2cDebug (0xff);
            I2C0_CONCLR = I2C_CONCLR_STAC;
            I2C0_CONSET = I2C_CONSET_STO;
            i2cErrno = I2CERR_NONE;
            i2cBusInUse = FALSE;
          }
          else
          {
            i2cDebug (0xfe);
            i2cAddress |= 0x01;
            i2cDataCnt = 0;
            I2C0_CONSET = I2C_CONSET_STA;
          }
        }
      }
      break;

    case I2CERR_DATRX_NACKTX : // 0x58
      {
        i2cDataBuffer [i2cDataCnt++] = I2C0_DAT;
        i2cDebug (i2cDataBuffer [i2cDataCnt - 1]);
        i2cErrno = I2CERR_NONE;
      }

    case I2CERR_SLAWTX_NACKRX : // 0x20
    case I2CERR_SLARTX_NACKRX : // 0x48
      {
        if (i2cAckPoll)
          break;
      }

    case I2CERR_DATTX_NACKRX : // 0x30
      {
        I2C0_CONCLR = I2C_CONCLR_STAC;
        I2C0_CONSET = I2C_CONSET_STO;
        i2cBusInUse = FALSE;
      }
      break;

    case I2CERR_ARBLOST : // 0x38
      {
        I2C0_CONSET = I2C_CONSET_STA;
      }
      break;

    //
    //  Receive byte conditions (fall through is intentional)
    //
    case I2CERR_DATRX_ACKTX : // 0x50
      {
        i2cDataBuffer [i2cDataCnt++] = I2C0_DAT;
        i2cDebug (i2cDataBuffer [i2cDataCnt - 1]);
      }

    case I2CERR_SLARTX_ACKRX : // 0x40
      {
        if (i2cDataCnt < i2cDataLenRead - 1)
        {
          I2C0_CONCLR = I2C_CONCLR_STAC;
          I2C0_CONSET = I2C_CONSET_AA;
        }
        else
          I2C0_CONCLR = I2C_CONCLR_STAC | I2C_CONCLR_AAC;
      }
      break;

    case I2CERR_NOINFO :
      break;

    default:
      {
        I2C0_CONCLR = I2C_CONCLR_I2ENC;
        i2cAckPoll = ACKPOLL_NO;
        i2cBusInUse = FALSE;
      }
      break;
  }

  I2C0_CONCLR = I2C_CONSET_SI;
  VIC_VectAddr = 0;
}

/**************************************************************************/
/*! 
    I2C Interrupt Service Routine    
*/
/**************************************************************************/
static void i2cISR (void) __attribute__ ((naked));
static void i2cISR (void)
{
  portSAVE_CONTEXT ();
  i2cISR_Handler ();
  portRESTORE_CONTEXT ();
}

/**************************************************************************/
/*! 
    I2C Init
*/
/**************************************************************************/
void i2cInit (i2cSpeed_e speed)
{
  i2cBusInUse = FALSE;
  i2cAckPoll = ACKPOLL_NO;

  PCB_PINSEL0 = (PCB_PINSEL0 & ~(PCB_PINSEL0_P02_MASK | PCB_PINSEL0_P03_MASK)) | (PCB_PINSEL0_P02_SCL0 | PCB_PINSEL0_P03_SDA0);

  // Add power to I2C0
  SCB_PCONP |= SCB_PCONP_PCI2C0;

  // Enable I2C0
  I2C0_CONCLR = I2C_CONCLR_MASK;
  I2C0_CONSET = I2C_CONSET_I2EN;

  // Set serial clock generator (assumes PCLK of 12MHz ... multiply for faster PCLK speeds)
  switch (speed)
  {
    case I2CSPEED_25KHZ:
      I2C0_SCLL   = 1920;  // 12MHz = 480
      I2C0_SCLH   = 1920;
      break;
    case I2CSPEED_50KHZ:
      I2C0_SCLL   = 960;  // 12MHz = 240
      I2C0_SCLH   = 960;
      break;
    case I2CSPEED_100KHZ:
      I2C0_SCLL   = 480;  // 12MHz = 120
      I2C0_SCLH   = 480;
      break;
    case I2CSPEED_200KHZ:
      I2C0_SCLL   = 240;   // 12MHz = 60
      I2C0_SCLH   = 240;
      break;
    case I2CSPEED_400KHZ:
      I2C0_SCLL   = 120;   // 12MHz = 30
      I2C0_SCLH   = 120;
      break;
    default:
      // Use 100kHz as default rate
      I2C0_SCLL   = 480;  // 12MHz = 120
      I2C0_SCLH   = 480;
      break;
  }

  //
  //  Initialize the interrupt vector
  //
  VIC_IntSelect &= ~VIC_IntSelect_I2C0;
  VIC_VectCntl7 = VIC_VectCntl_ENABLE | VIC_Channel_I2C0;
  VIC_VectAddr7 = (int) i2cISR;
  VIC_IntEnable = VIC_IntEnable_I2C0;
}

/**************************************************************************/
/*! 
    Waits for the interrupt handler to mark the I2C bus as no longer in use, then
    return 0 on success or -1 if any error occurred.
*/
/**************************************************************************/
static int i2cWaitComplete (int ticks)
{
  if (i2cBusInUse)
  {
    unsigned long theFuture;

    for (theFuture = xTaskGetTickCount() + ticks; i2cBusInUse; )
    {
      if (xTaskGetTickCount() > theFuture)
      {
        I2C0_CONCLR = I2C_CONCLR_I2ENC;
        i2cErrno = I2CERR_TIMEOUTWC;
        return -1;
      }
    }
  }

  return (i2cErrno == I2CERR_NONE) ? 0 : -1;
}

/**************************************************************************/
/*! 
    Disables interrupts then starts sending repeated STARTs until the slave device
    returns ACK.  This indicates the slave has completed any operation in progress.
*/
/**************************************************************************/
static int i2cPoll (U8 address)
{
  unsigned long timeoutTick = xTaskGetTickCount() + i2cTimeoutInTicks;

  VIC_IntEnClr = VIC_IntEnClr_I2C0;

  while (xTaskGetTickCount() < timeoutTick) 
  {
    I2C0_CONCLR = I2C_CONSET_SI;
    I2C0_CONSET = I2C_CONSET_STA;

    while (!(I2C0_CONSET & I2C_CONSET_SI))
      ;

    I2C0_CONCLR = I2C_CONCLR_STAC | I2C_CONCLR_SIC;
    I2C0_DAT = address & ~0x01;

    while (!(I2C0_CONSET & I2C_CONSET_SI))
      ;

    if ((i2cErrno = I2C0_STAT) == I2CERR_SLAWTX_ACKRX)
      break;
  }

  if (i2cErrno != I2CERR_SLAWTX_ACKRX)
    i2cErrno = I2CERR_TIMEOUTACKPOLL;

  I2C0_CONCLR = I2C_CONCLR_SIC;
  I2C0_CONSET = I2C_CONSET_STO | I2C_CONSET_AA;

  while (I2C0_CONSET & I2C_CONSET_STO)
    ;

  VIC_IntEnable = VIC_IntEnable_I2C0;

  return (i2cErrno == I2CERR_SLAWTX_ACKRX) ? 0 : -1;
}

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
static int i2cKickOff (U8 address, U8 *buffer, int bufferLenWrite, int bufferLenRead, ackPoll_e ackPoll)
{
  //
  //  Determine if our first operation will be a write or read.  If both, the
  //  write always occurs first.
  //
  if (bufferLenWrite)
    address &= ~0x01;
  else if (bufferLenRead)
    address |= 0x01;
  else
    address &= ~0x01;

  //
  //  Wait until last I2C operation has finished.  
  //
  if (i2cBusInUse && i2cWaitComplete (i2cTimeoutInTicks))
  {
    i2cErrno = I2CERR_TIMEOUT;
    return -1;
  }

  //
  //  Mark bus as in use, save the address, buffer and length
  //
  i2cBusInUse = TRUE;
  i2cAddress = address;
  i2cDataBuffer = buffer;
  i2cDataLenWrite = bufferLenWrite;
  i2cDataLenRead = bufferLenRead;
  i2cDataCnt = 0;
  i2cAckPoll = ackPoll;
  i2cDebugReset ();

  //
  //  Clear any outstanding bits, enable the interface, and send a start bit.
  //  Once the start bit goes out, the interrupt drive state machine begins.
  //
  I2C0_CONCLR = I2C_CONCLR_MASK;
  I2C0_CONSET = I2C_CONSET_I2EN;
  I2C0_CONSET = I2C_CONSET_STA;

  return 0;
}

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
static int i2cWriteBufferEx (U8 address, U8 *buffer, U32 bufferLength, int milliseconds)
{
  int r;
  if (!(r = i2cKickOff (address, buffer, bufferLength, 0, ACKPOLL_NO)))
    r = i2cWaitComplete (milliseconds / portTICK_RATE_MS);

  return r;
}

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
static int i2cReadBufferEx (U8 address, U8 *buffer, U32 bufferLength, int milliseconds)
{
  int r;

  if (!(r = i2cKickOff (address, buffer, 0, bufferLength, ACKPOLL_NO)))
    r = i2cWaitComplete (milliseconds / portTICK_RATE_MS);

  return r;
}

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
static int i2cWriteReadBufferEx (U8 address, U8 *buffer, U32 putLength, U32 getLength, int milliseconds)
{
  int r;

  if (!(r = i2cKickOff (address, buffer, putLength, getLength, ACKPOLL_NO)))
    r = i2cWaitComplete (milliseconds / portTICK_RATE_MS);

  return r;
}

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
void i2cSetTimeout (unsigned int timeoutInMilliseconds)
{
  if (timeoutInMilliseconds < portTICK_RATE_MS)
    timeoutInMilliseconds = portTICK_RATE_MS;

  i2cTimeoutInTicks = timeoutInMilliseconds / portTICK_RATE_MS;
}

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
void i2cDump (void)
{
#if defined CFG_I2C_DEBUG && CFG_I2C_DEBUG == 1
  int i;

  for (i = 0; i < i2cTransactionsIndex; i++)
    printf ("0x%02x ", i2cTransactions [i]);

  printf ("\r\n");
#else
  printf ("Not compiled for i2c debugging\r\n");
#endif
}

/**************************************************************************/
/*! 
    DANGER, WILL ROBINSON!  The callers buffer must persist until we're done
*/
/**************************************************************************/
int i2cWriteBuffer (U8 address, U8 *buffer, U32 bufferLength)
{
  return i2cWriteBufferEx (address, buffer, bufferLength, i2cTimeoutInTicks);
}

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
int i2cReadBuffer (U8 address, U8 *buffer, U32 bufferLength)
{
  return i2cReadBufferEx (address, buffer, bufferLength, i2cTimeoutInTicks);
}

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
int i2cWriteReadBuffer (U8 address, U8 *buffer, U32 putLength, U32 getLength)
{
  return i2cWriteReadBufferEx (address, buffer, putLength, getLength, i2cTimeoutInTicks);
}

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
int i2cWriteBufferPoll (U8 address, U8 *buffer, U32 bufferLength)
{
  int r;

  if (!(r = i2cKickOff (address, buffer, bufferLength, 0, ACKPOLL_NO)))
    if (!(r = i2cWaitComplete (i2cTimeoutInTicks)))
      r = i2cPoll (address);

  return r;
}

/**************************************************************************/
/*! 
    
*/
/**************************************************************************/
int i2cWriteReadBufferPoll (U8 address, U8 *buffer, U32 putLength, U32 getLength)
{
  int r;

  if (!(r = i2cKickOff (address, buffer, putLength, getLength, ACKPOLL_NO)))
    if (!(r = i2cWaitComplete (i2cTimeoutInTicks)))
      r = i2cPoll (address);

  return r;
}
