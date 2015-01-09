/**************************************************************************/
/*! 
    \file     tcs3414.c
    \author   K. Townsend (microBuilder.eu)
    \date     25 September 2009
    \version  1.00

    Driver for Taos TCS3414 Digital RGB Color/Light Sensor.
*/
/**************************************************************************/

#include "tcs3414.h"

//  If CFG_TCS3414 isn't defined don't allocate a task handle slot for it
#ifndef CFG_TCS3414
#define TASKHANDLE_TCS3414 0
#endif

static U8 tcs3414Address = TCS3414_ADDRESS;
static U8 tcs3414LastRegister = TCS3414_REGISTER_CONTROL;

// ======================================
// Private Methods
// ======================================

/**************************************************************************/
/*! 
    \brief Sends an 8 bit value to the I2C device.

    This method will send a single byte to the I2C device, as follows:
    [address] [reg].

    @param[in]  reg   The 8 bit value to send to the I2C device.

    \return     Returns I2CERR_NONE ('0') if no error occurred, or -1
                if an error occurred during the I2C request.
*/
/**************************************************************************/
static int tcs3414Write8 (U8 reg)
{
  U8 buffer [1];

  buffer [0] = tcs3414LastRegister = reg;

  return i2cWriteBuffer (tcs3414Address, buffer, sizeof (buffer));
}

/**************************************************************************/
/*! 
    \brief Sends an 8 bit 'registry' to the I2C device, followed
    by a secondary 8 bit value.

    This method will send a sequence of bytes to the I2C device in the
    following order: [address] [reg] [value].  This method should be used
    if the registry being addressed ('reg') is expecting another value
    immediately afterwards.  If you simply wish to write to an 8 bit
    registry, use tcs3414Write8(U8 reg) instead.

    @param[in]  reg   The 8 bit value to send to the I2C device.
    @param[in]  value The optional 8-bit value that will be sent after
                      the device address and 'reg'.

    \return     Returns I2CERR_NONE ('0') if no error occurred, or -1
                if an error occurred during the I2C request.
*/
/**************************************************************************/
static int tcs3414Write16 (U8 reg, int value)
{
  U8 buffer [2];

  buffer [0] = tcs3414LastRegister = reg;
  buffer [1] = value;

  return i2cWriteBuffer (tcs3414Address, buffer, sizeof (buffer));
}

/**************************************************************************/
/*! 
    \brief Writes the supplied registry value to the I2C devices and
    waits for an 8 bit response.

    @param[in]  reg   The registry value to send to the I2C device
    @param[out] value The int that will be used to hold the response

    \return     Returns I2CERR_NONE ('0') if no error occurred, or -1
                if an error occurred during the I2C request.
*/
/**************************************************************************/
static int tcs3414Read8(U8 reg, int *value)
{
  U8 buffer [1];
  buffer [0] = tcs3414LastRegister = reg;

  if (i2cWriteReadBuffer (tcs3414Address, buffer, sizeof (U8), sizeof (buffer)))
  {
    return -1;
  }

  *value = buffer[0];

  return I2CERR_NONE;
}

// ======================================
// Public Methods
// ======================================

/**************************************************************************/
/*! 
    \brief Initialises the TCS3414, turning on the IC as well and powering
    up the ADC converter.

    \return     Returns I2CERR_NONE ('0') if the intialisation was
                successful, or -1 if a problem was encountered.
    \note       There is a 12ms delay between the time that the ADC
                converter is turned on and the first results will be
                available.
*/
/**************************************************************************/
int tcs3414Init(void)
{
  // Power up the TCS3414 (0X01) and turn on the ADC (0X02)
  if (tcs3414Write16(TCS3414_REGISTER_CONTROL, 0x03))
  {
    return -1;
  }

  return I2CERR_NONE;
}

/**************************************************************************/
/*! 
    \brief Gets the latest conversion results for a specific colour.

    @param[in]  colour  The colour that should be 'read'.
    @param[out] value   The int to copy the conversion results to.

    \return     Returns I2CERR_NONE ('0') if no error occurred during the
                I2C conversion/request or -1 is an error occurred.
*/
/**************************************************************************/
int tcs3414GetColour(tcs3414Colours_t colour, int *value)
{
  int low = 0;
  int high = 0;

  switch (colour)
  {
    case tcs3414Colours_Red:
      if (tcs3414Read8(TCS3414_REGISTER_REDLOW, &low))
        return -1;
      if (tcs3414Read8(TCS3414_REGISTER_REDHIGH, &high))
        return -1;
      break;
    case tcs3414Colours_Green:
      if (tcs3414Read8(TCS3414_REGISTER_GREENLOW, &low))
        return -1;
      if (tcs3414Read8(TCS3414_REGISTER_GREENHIGH, &high))
        return -1;
      break;
    case tcs3414Colours_Blue:
      if (tcs3414Read8(TCS3414_REGISTER_BLUELOW, &low))
        return -1;
      if (tcs3414Read8(TCS3414_REGISTER_BLUEHIGH, &high))
        return -1;
      break;
    case tcs3414Colours_Clear:
      if (tcs3414Read8(TCS3414_REGISTER_CLEARLOW, &low))
        return -1;
      if (tcs3414Read8(TCS3414_REGISTER_CLEARHIGH, &high))
        return -1;
      break;
    default:
      low = 0;
      high = 0;
      break;
  }

  // Shift high bits and add the two bytes together
  *value = (high << 7) + low;

  return I2CERR_NONE;
}

/**************************************************************************/
/*! 
    \brief Returns a number identifying the IC type (3404 or 3414), as
    well as the revision number.

    @param[out] value  The int to copy the returned value into.

    \return     Returns I2CERR_NONE ('0') if no error occurred, or -1
                if an error occurred during the I2C request.
*/
/**************************************************************************/
int tcs3414GetICRevision(int *value)
{
  if (tcs3414Read8(TCS3414_REGISTER_PARTNO_REVID, value))
  {
    return -1;
  }

  return I2CERR_NONE;
}

// ======================================
// FreeRTOS Task(s)
// ======================================

/**************************************************************************/
/*! 
    ToDo: Initialise hardware and execute ??? activity
*/
/**************************************************************************/
static portTASK_FUNCTION(vTCS3414Task, pvParameters __attribute__((unused)))
{
  // ToDo: Initialise hardware

  for (;;)
  {
    // ToDo
  }
}

/**************************************************************************/
/*! 
    \brief Starts th TCS3414 Monitoring Task.
*/
/**************************************************************************/
signed portBASE_TYPE tcs3414TaskStart (void)
{
  return xTaskCreate (vTCS3414Task,  (const signed portCHAR * const) "TCS3414", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1), &taskHandles [TASKHANDLE_TCS3414]);
}

