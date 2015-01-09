/**************************************************************************/
/*! 
    \file     monitor.c
    \author   Original Code: J.C. Wren
              Modified By: K. Townsend (microBuilder.eu)
    \date     23 November 2009
    \version  1.00
*/
/**************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "FreeRTOS.h"
#include "task.h"

#include "args.h"
#include "monitor.h"
#include "argsdispatch.h"

#if defined CFG_MONITOR_ALLOWRESET && CFG_MONITOR_ALLOWRESET == 1
#include "../drivers/reset.h"
#endif

#if defined CFG_MONITOR_ALLOWI2C && CFG_MONITOR_ALLOWI2C == 1
#include "../core/i2c/i2c.h"
#endif

#if defined CFG_MONITOR_ALLOWSPI && CFG_MONITOR_ALLOWSPI == 1
#include "../core/spi/spi.h"
#endif

#if defined CFG_RTC
#include "../core/rtc/rtc.h"
#endif

#ifdef CFG_FATFS
#include "../fatfs/disk.h"
#include "../fatfs/ff.h"
#endif

#ifdef CFG_LED
#include "../drivers/leds/leds.h"
#endif

#ifdef CFG_UIP
#include "../uip/uiptask.h"
#include "../uip/uip/uip.h"
#include "../uip/uip/uip_arp.h"
#include "../uip/apps/telnetd/telnetd.h"
#include "../uip/apps/sntp/sntp.h"
#endif

// 
// Method Prototypes
// 

static int monitorHelp (int argc, portCHAR **argv);
static int monitorReset (int argc, portCHAR **argv);
#if defined CFG_MONITOR_ALLOWI2C && CFG_MONITOR_ALLOWI2C == 1
static int monitorI2CInit (int argc, portCHAR **argv);
static int monitorI2CRead (int argc, portCHAR **argv);
static int monitorI2CWrite (int argc, portCHAR **argv);
static int monitorI2CWriteRead (int argc, portCHAR **argv);
static int monitorI2CDump (int argc, portCHAR **argv);
static int monitorI2CErrno (int argc, portCHAR **argv);
#endif
#if defined CFG_MONITOR_ALLOWSPI && CFG_MONITOR_ALLOWSPI == 1
static int monitorSPIInit (int argc, portCHAR **argv);
static int monitorSPIWrite (int argc, portCHAR **argv);
#endif
#ifdef CFG_LED
static int monitorLEDStart (int argc, portCHAR **argv);
static int monitorLEDStop  (int argc, portCHAR **argv);
static int monitorLEDDelay (int argc, portCHAR **argv);
#endif
#ifdef CFG_RTC
static int monitorRTCGet (int argc, portCHAR **argv);
static int monitorRTCSet (int argc, portCHAR **argv);
static int monitorRTCAlarm (int argc, portCHAR **argv);
static int monitorRTCPeriodic (int argc, portCHAR **argv);
#endif
#ifdef CFG_FATFS
static int monitorFileCStatus (int argc, portCHAR **argv);
static int monitorFileDf (int argc, portCHAR **argv);
static int monitorFileInit (int argc, portCHAR **argv);
static int monitorFileLs (int argc, portCHAR **argv);
static int monitorFileMkfs (int argc, portCHAR **argv);
static int monitorFileMount (int argc, portCHAR **argv);
static int monitorFileMkdir (int argc, portCHAR **argv);
#endif
#ifdef CFG_UIP
static int monitorUIPDHCP (int argc, portCHAR **argv);
#ifdef CFG_TELNETD
  static int monitorUIPDrop (int argc, portCHAR **argv);
#endif
  static int monitorUIPGW (int argc, portCHAR **argv);
  static int monitorUIPIP (int argc, portCHAR **argv);
  static int monitorUIPMAC (int argc, portCHAR **argv);
  static int monitorUIPNM (int argc, portCHAR **argv);
#ifdef CFG_SNTP
  static int monitorUIPSNTP (int argc, portCHAR **argv);
#endif
#ifdef CFG_SNTP
  static int monitorUIPTO (int argc, portCHAR **argv);
#endif
  static int monitorUIPStart (int argc, portCHAR **argv);
  static int monitorUIPStop (int argc, portCHAR **argv);
  static int monitorUIPStats (int argc, portCHAR **argv);
#endif
static int monitorVersion (int argc, portCHAR **argv);

// 
// Command Lists
// 

#if defined CFG_MONITOR_ALLOWI2C && CFG_MONITOR_ALLOWI2C == 1
/**************************************************************************/
/*! 
    I2C CommandList
*/
/**************************************************************************/
static const commandList_t commandListI2C [] =
{
  { "help",     0,  0, CMDTYPE_FUNCTION,  { monitorHelp        }, "This help list",                     "'help' has no parameters" },
  { "init",     1,  1, CMDTYPE_FUNCTION,  { monitorI2CInit     }, "Initialise I2C bus",                 "'init <25|50|100|200|400>'" },
  { "r",        2,  2, CMDTYPE_FUNCTION,  { monitorI2CRead     }, "Read from I2C device",               "'r <address> <# bytes>'" },
  { "w",        2, 17, CMDTYPE_FUNCTION,  { monitorI2CWrite    }, "Write to I2C device",                "'w <address> <byte> [<byte> [...<byte>]]'" },
  { "wr",       2, 18, CMDTYPE_FUNCTION,  { monitorI2CWriteRead}, "Write to then read from I2C device", "'wr <address> <byte> [<byte> [...<byte>]] <# bytes to read>'" },
  { "dump",     0,  0, CMDTYPE_FUNCTION,  { monitorI2CDump     }, "Dump I2C Debug Buffer",              "'dump' has no parameters" },
  { "errno",    0,  0, CMDTYPE_FUNCTION,  { monitorI2CErrno    }, "Display i2cErrno value",             "'errno' has no parameters" },
  { NULL,       0,  0, CMDTYPE_FUNCTION,  { NULL               }, NULL,                                 NULL },
};
#endif

#ifdef CFG_LED
/**************************************************************************/
/*! 
    LED CommandList
*/
/**************************************************************************/
static const commandList_t commandListLED [] =
{
  { "help",     0,  0, CMDTYPE_FUNCTION,  { monitorHelp        }, "This help list",           "'help' has no parameters" },
  { "start",    0,  0, CMDTYPE_FUNCTION,  { monitorLEDStart    }, "Start LED task",           "'start' has no parameters" },
  { "stop",     0,  0, CMDTYPE_FUNCTION,  { monitorLEDStop     }, "End LED task",             "'stop' has no parameters" },
  { "delay",    0,  1, CMDTYPE_FUNCTION,  { monitorLEDDelay    }, "Set LED delay (in ticks)", "delay <5..500>" },
  { NULL,       0,  0, CMDTYPE_FUNCTION,  { NULL               }, NULL,                       NULL },
};
#endif

#ifdef CFG_RTC
/**************************************************************************/
/*! 
    RTC CommandList
*/
/**************************************************************************/
static const commandList_t commandListRTC [] =
{
  { "help",     0,  0, CMDTYPE_FUNCTION,  { monitorHelp        }, "This help list",               "'help' has no parameters" },
  { "get",      0,  0, CMDTYPE_FUNCTION,  { monitorRTCGet      }, "Display system date/time",     "'get' has no parameters" },
  { "set",      2,  2, CMDTYPE_FUNCTION,  { monitorRTCSet      }, "Set system date/time",         "set <YYYY/MM/DD HH:MM:SS>" },
  { NULL,       0,  0, CMDTYPE_FUNCTION,  { NULL               }, NULL,                           NULL },
};
#endif

#ifdef CFG_FATFS
/**************************************************************************/
/*! 
    SD Card/FAT CommandList
*/
/**************************************************************************/
static const commandList_t commandListFile [] =
{
  { "help",     0,  0, CMDTYPE_FUNCTION,  { monitorHelp        }, "This help list",               "'help' has no parameters" },
  { "cstatus",  0,  0, CMDTYPE_FUNCTION,  { monitorFileCStatus }, "SD card status",               "'cstatus' has no parameters" },
  { "df",       0,  0, CMDTYPE_FUNCTION,  { monitorFileDf      }, "File system status & info",    "'df' has no parameters" },
  { "init",     0,  0, CMDTYPE_FUNCTION,  { monitorFileInit    }, "Initialize disk subsystem",    "'init' has no parameters" },
  { "ls",       0,  1, CMDTYPE_FUNCTION,  { monitorFileLs      }, "Display directory",            "'ls [optional path]'" },
  { "mkdir",    1,  1, CMDTYPE_FUNCTION,  { monitorFileMkdir   }, "Create directory",             "'mkdir <[path]directory'" },
  { "mkfs",     0,  0, CMDTYPE_FUNCTION,  { monitorFileMkfs    }, "Create a FAT filesystem",      "'mkfs' has no parameters" },
  { "mount",    0,  0, CMDTYPE_FUNCTION,  { monitorFileMount   }, "Mount disk",                   "'mount' has no parameters" },
  { NULL,       0,  0, CMDTYPE_FUNCTION,  { NULL               }, NULL,                           NULL },
};
#endif

#if defined CFG_MONITOR_ALLOWSPI && CFG_MONITOR_ALLOWSPI == 1
/**************************************************************************/
/*! 
    SPI CommandList
*/
/**************************************************************************/
static const commandList_t commandListSPI [] =
{
  { "help",     0,  0, CMDTYPE_FUNCTION,  { monitorHelp        }, "This help list",                     "'help' has no parameters" },
  { "init",     0,  0, CMDTYPE_FUNCTION,  { monitorSPIInit     }, "Initialise SPI bus",                 "'init' has no paraleters" },
  { "w",        1,  1, CMDTYPE_FUNCTION,  { monitorSPIWrite    }, "Write to SPI device",                "'w <byte>'" },
  { NULL,       0,  0, CMDTYPE_FUNCTION,  { NULL               }, NULL,                                 NULL },
};
#endif

#ifdef CFG_UIP
/**************************************************************************/
/*! 
    uIP CommandList
*/
/**************************************************************************/
static const commandList_t commandListUIP [] =
{
  { "help",     0,  0, CMDTYPE_FUNCTION,  { monitorHelp        }, "This help list",               "'help' has no parameters" },
  { "dhcp",     0,  1, CMDTYPE_FUNCTION,  { monitorUIPDHCP     }, "Display/set DHCP state",       "'dhcp [<on|off>]'" },
#ifdef CFG_TELNETD
  { "drop",     0,  1, CMDTYPE_FUNCTION,  { monitorUIPDrop     }, "Drop telnet client connection","'drop' has no parameters" },
#endif
  { "gw",       0,  1, CMDTYPE_FUNCTION,  { monitorUIPGW       }, "Display/set gateway address",  "'gw [<a.b.c.d>]'" },
  { "ip",       0,  1, CMDTYPE_FUNCTION,  { monitorUIPIP       }, "Display/set IP address",       "'ip [<a.b.c.d>]'" },
  { "mac",      0,  1, CMDTYPE_FUNCTION,  { monitorUIPMAC      }, "Display/set MAC address",      "'mac [<xx:yy:zz:aa:bb:cc>]'" },
  { "nm",       0,  1, CMDTYPE_FUNCTION,  { monitorUIPNM       }, "Display/set netmask address",  "'nm [<a.b.c.d>]'" },
#ifdef CFG_SNTP
  { "sntp",     0,  1, CMDTYPE_FUNCTION,  { monitorUIPSNTP     }, "Request SNTP time",            "'sntp [<set>|<a.b.c.d>]', ex.: 'sntp 131.234.137.24'" },
#endif
  { "start",    0,  0, CMDTYPE_FUNCTION,  { monitorUIPStart    }, "Start UIP stack",              "'start' has no parameters" },
  { "stop",     0,  0, CMDTYPE_FUNCTION,  { monitorUIPStop     }, "Stop UIP stack",               "'stop' has no parameters" },
  { "stats",    0,  0, CMDTYPE_FUNCTION,  { monitorUIPStats    }, "Display uIP status",           "'stats' has no parameters" },
#ifdef CFG_SNTP
  { "to",       0,  1, CMDTYPE_FUNCTION,  { monitorUIPTO       }, "Display/set time offset",      "'to [offset_in_seconds]'" },
#endif
  { NULL,       0,  0, CMDTYPE_FUNCTION,  { NULL               }, NULL,                           NULL },
};
#endif

/**************************************************************************/
/*! 
    Top-Level CommandList
*/
/**************************************************************************/
static const commandList_t commandList [] =
{
  { "help",     0,  0, CMDTYPE_FUNCTION,  { monitorHelp        }, "This help list",                 "'help' has no parameters" },
#if defined CFG_FATFS
  { "file",     1,  0, CMDTYPE_CMDLIST,   { commandListFile    }, "Various file related commands",  "'file help' for help list" },
#endif
#if defined CFG_MONITOR_ALLOWI2C && CFG_MONITOR_ALLOWI2C == 1
  { "i2c",      1,  0, CMDTYPE_CMDLIST,   { commandListI2C     }, "Generic I2C commands",           "'i2c help' for help list" },
#endif
#ifdef CFG_LED
  { "led",      1,  0, CMDTYPE_CMDLIST,   { commandListLED     }, "Various LED commands",           "'led help' for help list" },
#endif
#if defined CFG_MONITOR_ALLOWRESET && CFG_MONITOR_ALLOWRESET == 1
  { "reset",    0,  0, CMDTYPE_FUNCTION,  { monitorReset       }, "Resets the MCU",                 "'reset' has no parameters" },
#endif
#ifdef CFG_RTC
  { "rtc",      1,  0, CMDTYPE_CMDLIST,   { commandListRTC     }, "RTC (real-time clock) commands", "'rtc help' for help list" },
#endif
#if defined CFG_MONITOR_ALLOWSPI && CFG_MONITOR_ALLOWSPI == 1
  { "spi",      1,  0, CMDTYPE_CMDLIST,   { commandListSPI     }, "Generic SPI commands",           "'spi help' for help list" },
#endif
#ifdef CFG_UIP
  { "uip",      1,  0, CMDTYPE_CMDLIST,   { commandListUIP     }, "Various uIP commands",           "'uip help' for help list" },
#endif
  { "version",  0,  0, CMDTYPE_FUNCTION,  { monitorVersion     }, "Display version information",    "'version' has no parameters" },
  { NULL,       0,  0, CMDTYPE_FUNCTION,  { NULL               }, NULL,                             NULL },
};

#ifdef CFG_UIP
#ifdef CFG_DHCP
static int uipDHCPC = TRUE;
#else
static int uipDHCPC = FALSE;
#endif
#endif

#ifdef CFG_FATFS
static FATFS fatfs;
static FILINFO fileInfo;
#endif

commandList_t *activeCommandList = NULL;

// 
// External variables
// 

extern xTaskHandle taskHandles [TASKHANDLE_LAST];

#ifdef CFG_FATFS
//
//  Recursion is our friend...  fileInfo is global to avoid eating stack space
//
static FRESULT scan_files_ex (char *path, int *total_size, int *total_files, int *total_dirs)
{
  DIR dirs;
  FRESULT res;

  if ((res = f_opendir (&dirs, path)) == FR_OK) 
  {
    int i = strlen (path);

    while (((res = f_readdir (&dirs, &fileInfo)) == FR_OK) && fileInfo.fname [0]) 
    {
      if (fileInfo.fattrib & AM_DIR) 
      {
        *total_dirs += 1;
        *(path + i) = '/'; 
        strcpy (path + i + 1, &fileInfo.fname [0]);
        res = scan_files_ex (path, total_size, total_files, total_dirs);
        *(path + i) = '\0';

        if (res != FR_OK) 
          return res;
      } 
      else 
      {
        *total_files += 1;
        *total_size += fileInfo.fsize;
      }
    }
  }

  return res;
}

static FRESULT scan_files (char *path, int *total_size, int *total_files, int *total_dirs)
{
  *total_size = 0;
  *total_files = 0;
  *total_dirs = 0;

  return scan_files_ex (path, total_size, total_files, total_dirs);
}

//
//  These two really ought to be in the FatFS code
//
U32 get_fattime ()
{
  U32 tmr;
  time_t now;
  struct tm tm;

  now = time (NULL);
  localtime_r (&now, &tm);

  tmr = 0
    | ((tm.tm_year - 80) << 25)
    | ((tm.tm_mon + 1)   << 21)
    | (tm.tm_mday        << 16)
    | (tm.tm_hour        << 11)
    | (tm.tm_min         << 5)
    | (tm.tm_sec         >> 1);

  return tmr;
}

#endif

// 
// Private Methods
// 

#if defined CFG_UIP
/**************************************************************************/
/*! 
    Determines whether the supplied string is a decimal value
*/
/**************************************************************************/
static int isDecimalString (char *s)
{
  while (*s)
    if (!isdigit (*s++))
      return 0;

  return 1;
}
#endif

/**************************************************************************/
/*! 
    Converts a string representation of a number to an integer
*/
/**************************************************************************/
static int getNumber (char *s, unsigned int *result)
{
  unsigned int value;
  unsigned int mustBeHex = FALSE;
  int sgn = 1;
  const unsigned char hexToDec [] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 255, 255, 255, 255, 255, 255, 255, 10, 11, 12, 13, 14, 15};

  if (!s)
    return 0;

  if ((strlen (s) > 2) && (!strncmp (s, "0x", 2) || !strncmp (s, "0X", 2)))
  {
    mustBeHex = TRUE;
    s += 2;
  }

  if (!mustBeHex && *s && (*s == '-'))
  {
    sgn = -1;
    s++;
  }

  for (value = 0; *s; s++)
  {
    if (mustBeHex && isxdigit (*s))
      value = (value << 4) | hexToDec [toupper (*s) - '0'];
    else if (isdigit (*s))
      value = (value * 10) + (*s - '0');
    else
    {
      printf ("Malformed number.  Must be decimal number, or hex value preceeded by '0x'\r\n");
      return 0;
    }
  }

  if (!mustBeHex)
    value *= sgn;

  *result = value;

  return 1;
}

/**************************************************************************/
/*! 
    Dumps the memory contents of the supplied address
*/
/**************************************************************************/
static int monitorDumpMemory (unsigned int displayAddress, unsigned int mask, unsigned int address, int length)
{
  unsigned char *buffer;
  int i;

  if (!length)
  {
    printf ("Error: monitorDumpMemory() passed 0 for length\r\n");
    return address;
  }

  for (buffer = (unsigned char *) address, i = 0; i < length; i += 16)
  {
    unsigned int l;
    unsigned int j;

    if (i)
      printf ("\r\n");

    printf ("%08x: ", (displayAddress + i) & mask);

    if ((length - i) < 16)
      l = length & 15;
    else
      l = 16;

    for (j = 0; j < 16; j++)
    {
      if (j < l)
        printf ("%02x ", buffer [i+j]);
      else
        printf ("   ");
    }

    printf ("  ");

    for (j = 0; j < l; j++)
    {
      unsigned char c = buffer [i+j];

      if (c < 32 || c > 127)
        c = '.';

      printf ("%c", c);
    }
  }

  printf ("\r\n");

  address += length;

  return address;
}

/**************************************************************************/
/*! 
    Monitor help menu
*/
/**************************************************************************/
static int monitorHelp (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  unsigned int i;
  int t;
  int longestCmd;
  portCHAR spaces [32];

  memset (spaces, ' ', sizeof (spaces));

  for (longestCmd = 0, i = 0; activeCommandList [i].command; i++)
    if ((t = strlen (activeCommandList [i].command)) > longestCmd)
      longestCmd = t;

  spaces [longestCmd] = '\0';

  for (i = 0; activeCommandList [i].command; i++)
  {
    const commandList_t *cl = &activeCommandList [i];

    printf ("%s%s -- %s\r\n", cl->command, &spaces [strlen (cl->command)], cl->description);
  }

  printf ("\r\nUse '<command> ?' for details on parameters to command\r\n");

  return 0;
}

#if defined CFG_MONITOR_ALLOWRESET && CFG_MONITOR_ALLOWRESET == 1
/**************************************************************************/
/*! 
    Resets the MCU
*/
/**************************************************************************/
int monitorReset (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  printf ("Resetting the MCU ...\r\n");
  reset();

  return 0;
}
#endif

#if defined CFG_MONITOR_ALLOWI2C && CFG_MONITOR_ALLOWI2C == 1

/**************************************************************************/
/*! 
    Determines whether I2C has been initialised yet.
*/
/**************************************************************************/
static int isI2CInitialised(void)
{
  // Check I2C0 flag in PCONP register
  if (SCB_PCONP & (1 << 7)) // PCONP Bit 7 = SCB_PCONP_PCI2C0
  {
    // ToDo: This is a very weak test ... check the enable flags, etc.!
    return 1;
  }
  else
  {
    return 0;
  }
}

/**************************************************************************/
/*! 
    Initialise I2C bus at a specific speed (25, 50, 100, 200 or 400 Hz)
*/
/**************************************************************************/
int monitorI2CInit (int argc __attribute__ ((unused)), portCHAR **argv)
{
  unsigned int speed;

  if (!getNumber (argv [0], &speed))
  {
    return 0;
  }

  if ((speed == 25) || (speed == 50) || (speed == 100) || (speed == 200) || (speed == 400))
  {
    switch (speed)
    {
      case 25:
        i2cInit(I2CSPEED_25KHZ);
        break;
      case 50:
        i2cInit(I2CSPEED_50KHZ);
        break;
      case 100:
        i2cInit(I2CSPEED_100KHZ);
        break;
      case 200:
        i2cInit(I2CSPEED_200KHZ);
        break;
      case 400:
        i2cInit(I2CSPEED_400KHZ);
        break;
    }
    printf("I2C bus initialised at %d Hz\r\n", speed);
  }
  else
  {
    printf("Error: I2C speed must be 25, 50, 100, 200 or 400\r\n");
    return 0;
  }
}

/**************************************************************************/
/*! 
    Read a value from the I2C bus
*/
/**************************************************************************/
int monitorI2CRead (int argc __attribute__ ((unused)), portCHAR **argv)
{
  unsigned int address;
  unsigned int numBytes;
  unsigned char buffer [16];
  int r;

  if (!getNumber (argv [0], &address))
  {
    return 0;
  }
  if (!getNumber (argv [1], &numBytes))
  {
    return 0;
  }

  if (address > 255)
  {
    printf ("Error: address must be 0x00..0xff (0..255)\r\n");
    return 0;
  }

  if ((numBytes < 1) || (numBytes > sizeof (buffer)))
  {
    printf ("Error: number of bytes must be 1..%ld\r\n", sizeof (buffer));
    return 0;
  }

  if (!isI2CInitialised())
  {
    printf("Error: I2C has not been initialised.  Use 'i2c init <speed>'\r\n");
    return 0;
  }

  r = i2cReadBuffer (address, buffer, numBytes);

  printf ("i2cReadBuffer() returned %d/%s (%s)\r\n\r\n", i2cGetErrno (), i2cStrerror (i2cGetErrno ()), r ? "error" : "no error");

  // The first line with buffer will dump the entire buffer
  // monitorDumpMemory (0, 0xffffffff, (unsigned int) buffer, (int) sizeof (buffer));
  // ... the second line only dumps numBytes
  monitorDumpMemory (0, 0xffffffff, (unsigned int) buffer, numBytes);

  return 0;
}

/**************************************************************************/
/*! 
    Write a value to the I2C bus
*/
/**************************************************************************/
int monitorI2CWrite (int argc, portCHAR **argv)
{
  unsigned int address;
  unsigned char buffer [16];
  int i;

  if (!getNumber (argv [0], &address))
  {
    return 0;
  }

  if (address > 255)
  {
    printf ("Error: address must be 0x00..0xff (0..255)\r\n");
    return 0;
  }

  if (!isI2CInitialised())
  {
    printf("Error: I2C has not been initialised.  Use 'i2c init <speed>'\r\n");
    return 0;
  }

  for (i = 0; i < argc - 1; i++)
  {
    unsigned int n;

    if (!getNumber (argv [i + 1], &n))
      return 0;

    if (n > 255)
    {
      printf ("Error: data must be 0x00..0xff (0..255)\r\n");
      return 0;
    }

    buffer [i] = n;
  }

  i = i2cWriteBuffer (address, buffer, argc - 1);

  printf ("i2cWriteBuffer() returned %d/%s (%s)\r\n", i2cGetErrno (), i2cStrerror (i2cGetErrno ()), i ? "error" : "no error");

  return 0;
}

/**************************************************************************/
/*! 
    Write a value to the I2C bus and then wait for a reply
*/
/**************************************************************************/
int monitorI2CWriteRead (int argc, portCHAR **argv)
{
  unsigned int address;
  unsigned int bytesToWrite;
  unsigned int bytesToRead;
  unsigned char buffer [16];
  unsigned int i;

  if (!getNumber (argv [0], &address))
  {
    return 0;
  }

  if (address > 255)
  {
    printf ("Error: address must be 0x00..0xff (0..255)\r\n");
    return 0;
  }

  if (!isI2CInitialised())
  {
    printf("Error: I2C has not been initialised.  Use 'i2c init <speed>'\r\n");
    return 0;
  }

  for (bytesToWrite = argc - 2, i = 0; i < bytesToWrite; i++)
  {
    unsigned int n;

    if (!getNumber (argv [i + 1], &n))
      return 0;

    if (n > 255)
    {
      printf ("Error: data must be 0x00..0xff (0..255)\r\n");
      return 0;
    }

    buffer [i] = n;
  }

  if (!getNumber (argv [argc - 1], &bytesToRead))
    return 0;

  if ((bytesToRead < 1) || (bytesToRead > sizeof (buffer)))
  {
    printf ("Error: number of bytes must be 1..%ld\r\n", sizeof (buffer));
    return 0;
  }

  i2cWriteReadBuffer (address, buffer, bytesToWrite, bytesToRead);

  printf ("i2cWriteReadBuffer() returned %d/%s\r\n\r\n", i2cGetErrno (), i2cStrerror (i2cGetErrno ()));

  // The first line with buffer will dump the entire buffer
  // monitorDumpMemory (0, 0xffffffff, (unsigned int) buffer, (int) sizeof (buffer));
  // ... the second line only dumps bytesToRead # of bytes
  monitorDumpMemory (0, 0xffffffff, (unsigned int) buffer, bytesToRead);

  return 0;
}

/**************************************************************************/
/*! 
    Dumps the I2C debug buffer
*/
/**************************************************************************/
int monitorI2CDump (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  i2cDump ();

  return 0;
}

/**************************************************************************/
/*! 
    Displays the I2C error number value
*/
/**************************************************************************/
int monitorI2CErrno (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  printf ("i2cErrno=%d/%s\r\n", i2cGetErrno (), i2cStrerror (i2cGetErrno ()));

  return 0;
}
#endif

#ifdef CFG_LED
/**************************************************************************/
/*! 
    Start LED Task
*/
/**************************************************************************/
int monitorLEDStart (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  if (taskHandles [TASKHANDLE_LED])
  {
    printf ("LED task is already running.  Use 'led stop' to stop it\r\n");
  }
  else
  {
    ledsTaskStart ();
  }

  return 0;
}

/**************************************************************************/
/*! 
    Stop LED Task
*/
/**************************************************************************/
int monitorLEDStop (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  if (!ledsTaskStop ())
  {
    printf ("LED task is not yet running.  Use 'led start' to start it\r\n");
  }

  return 0;
}

/**************************************************************************/
/*! 
    Sets the delay for the flashing LED.  Values can be between 5 and
    500 ticks, where each 'tick' represents ~10mS.
*/
/**************************************************************************/
int monitorLEDDelay (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  U32 speed;

  // If no args were passed, simple display the current delay
  if (argc == 0)
  {
    printf("Current delay is %d ticks\r\n", ledsGetDelay());
    return 0;
  }

  // Make sure that numeric data was entered
  if (!getNumber (argv [0], &speed))
  {
    return 0;
  }
  // Check if rate is between 1 and 100Hz
  else if ((speed > 500) || (speed < 5))
  {
    printf("Delay value must be between 5 and 500\r\n");
    return 0;
  }
  else
  {
    // Change blink rate
    ledsSetDelay(speed);
    printf("Delay has been set to: %d\r\n", speed);
    return 0;
  }

  return 0;
}

#endif

#ifdef CFG_RTC
/**************************************************************************/
/*! 
    Gets the current time from the RTC (Real-Time Clock)
*/
/**************************************************************************/
static int monitorRTCGet (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  time_t now;
  char buffer [32];

  now = time (NULL);
  ctime_r (&now, buffer);
  printf ("%s", buffer);

  return 0;
}

//
//  This works because atoi() stops on a non-digit.  We already know the 
//  string values are digits, because of isGoodString().
//
static int tmSetElement (int *element, int minValue, int maxValue, int adjustValue, char *s)
{
  int v;
  
  if (((v = atoi (s)) < minValue) || (v > maxValue))
  {
    printf ("Illegal value encountered in parameter\r\n");
    return FALSE;
  }

  *element = (v + adjustValue);

  return TRUE;
}

/**************************************************************************/
/*! 
    Checks if a string matches the supplied format
*/
/**************************************************************************/
static int isGoodString (char *s, char *fmt)
{
  unsigned int i;
  unsigned int l;

  if ((l = strlen (s)) != strlen (fmt))
    return FALSE;

  for (i = 0; i < l; i++, s++, fmt++)
  {
    if (*fmt == 'N')
    {
      if (!isdigit (*s))
        return FALSE;
    }
    else if (*fmt != *s)
      return FALSE;
  }

  return TRUE;
}

/**************************************************************************/
/*! 
    Sets the date/time for the RTC (Real-Time Clock)
*/
/**************************************************************************/
static int monitorRTCSet (int argc, portCHAR **argv)
{
  if (isGoodString (argv [0], "NNNN/NN/NN") && isGoodString (argv [1], "NN:NN:NN"))
  {
    struct tm tm;

    if (!tmSetElement (&tm.tm_year, 1900, 2038, -1900, &argv [0][0]))
      return 0;
    if (!tmSetElement (&tm.tm_mon, 1, 12, -1, &argv [0][5]))
      return 0;
    if (!tmSetElement (&tm.tm_mday, 1, 31, 0, &argv [0][8]))
      return 0;
    if (!tmSetElement (&tm.tm_hour, 0, 23, 0, &argv [1][0]))
      return 0;
    if (!tmSetElement (&tm.tm_min, 0, 59, 0, &argv [1][3]))
      return 0;
    if (!tmSetElement (&tm.tm_sec, 0, 59, 0, &argv [1][6]))
      return 0;

    tm.tm_wday  = 0;
    tm.tm_yday  = 0;
    tm.tm_isdst = 0;

    rtcSetEpochSeconds (mktime (&tm));
  }
  else
  {
    printf ("Parameter(s) not 'YYYYY/MM/DD HH:MM:SS'\r\n");
  }

  return 0;
}

#endif

#if defined CFG_FATFS

/**************************************************************************/
/*! 
    Checks whether a card is insert and if it is read-only
*/
/**************************************************************************/
static int monitorFileCStatus (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  if (1 == sspHardwareCardPresent())
  {
    // Card is present ... check if it is write-protected
    if (1 == sspHardwareWriteProtected())
    {
      printf("SD card present : yes\r\n");
      printf("Write-protected : yes\r\n");
      return 0;
    }
    else 
    {
      printf("SD card present : yes\r\n");
      printf("Write-protected : no\r\n");
      return 0;
    }
  }
  else
  {
      printf("SD card present : no\r\n");
      printf("Write-protected : --\r\n");
    return 0;
  }

  return 0;
}

/**************************************************************************/
/*! 
    Display file system status and info
*/
/**************************************************************************/
static int monitorFileDf (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  U32 p2;
  FATFS *fs;
  char buffer [100];
  FRESULT res;
  int acc_size;
  int acc_files;
  int acc_dirs;

  if ((res = f_getfree ("", (U32 *) &p2, &fs)))
  { 
    f_printerror (res); 
    return 0;
  }

  printf ("FAT type = %u\nBytes/Cluster = %u\nNumber of FATs = %u\n"
      "Root DIR entries = %u\nSectors/FAT = %u\nNumber of clusters = %u\n"
      "FAT start (lba) = %u\nDIR start (lba,clustor) = %u\nData start (lba) = %u\n",
      fs->fs_type, fs->sects_clust * 512, fs->n_fats,
      fs->n_rootdir, fs->sects_fat, fs->max_clust - 2,
      fs->fatbase, fs->dirbase, fs->database
      );

  acc_size = acc_files = acc_dirs = 0;

  buffer [0] = '\0';

  if ((res = scan_files (buffer, &acc_size, &acc_files, &acc_dirs)))
  { 
    f_printerror (res); 
    return 0;
  }

  printf ("\n%u files, %u bytes.\n%u folders.\n"
      "%u bytes total disk space.\n%u bytes available\n",
      acc_files, acc_size, acc_dirs,
      (fs->max_clust - 2) * fs->sects_clust * 512, p2 * fs->sects_clust * 512
      );

  return 0;
}

/**************************************************************************/
/*! 
    Initialises the disk subsystem
*/
/**************************************************************************/
static int monitorFileInit (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  printf ("RRC=%d\n", diskInitialize (0));

  return 0;
}

/**************************************************************************/
/*! 
    Displays directory contents
*/
/**************************************************************************/
static int monitorFileLs (int argc, portCHAR **argv)
{
  DIR dir;
  FRESULT res;
  U32 size;
  U16 files;
  U16 dirs;
  FATFS *fs;
  char *path;

  path = argc ? argv [0] : "";

  if ((res = f_opendir (&dir, path)))
  { 
    f_printerror (res); 
    return 0;
  }

  for (size = files = dirs = 0;;)
  {
    if (((res = f_readdir (&dir, &fileInfo)) != FR_OK) || !fileInfo.fname [0]) 
      break;

    if (fileInfo.fattrib & AM_DIR) 
      dirs++;
    else 
    {
      files++; 
      size += fileInfo.fsize;
    }

    printf ("\n%c%c%c%c%c %u/%02u/%02u %02u:%02u %9u  %s",
        (fileInfo.fattrib & AM_DIR) ? 'D' : '-',
        (fileInfo.fattrib & AM_RDO) ? 'R' : '-',
        (fileInfo.fattrib & AM_HID) ? 'H' : '-',
        (fileInfo.fattrib & AM_SYS) ? 'S' : '-',
        (fileInfo.fattrib & AM_ARC) ? 'A' : '-',
        (fileInfo.fdate >> 9) + 1980, (fileInfo.fdate >> 5) & 15, fileInfo.fdate & 31,
        (fileInfo.ftime >> 11), (fileInfo.ftime >> 5) & 63,
        fileInfo.fsize, &(fileInfo.fname [0]));
  }

  printf ("\n%4u File(s),%10u bytes\n%4u Dir(s)", files, size, dirs);

  if (f_getfree (path, (U32 *) &size, &fs) == FR_OK)
    printf (", %10uK bytes free", size * fs->sects_clust / 2);

  printf ("\n");

  return 0;
}

/**************************************************************************/
/*! 
    Creates a directory
*/
/**************************************************************************/
static int monitorFileMkdir (int argc __attribute__ ((unused)), portCHAR **argv)
{
  f_printerror (f_mkdir (argv [0]));

  return 0;
}

/**************************************************************************/
/*! 
    Creates a FAT filesystem
*/
/**************************************************************************/
static int monitorFileMkfs (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  f_printerror (f_mkfs (0, 0, 64));

  return 0;
}

/**************************************************************************/
/*! 
    Mounts a logical drive
*/
/**************************************************************************/
static int monitorFileMount (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  f_printerror (f_mount (0, &fatfs));

  return 0;
}

#endif

#if defined CFG_MONITOR_ALLOWSPI && CFG_MONITOR_ALLOWSPI == 1
/**************************************************************************/
/*! 
    Determines whether SPI has been initialised yet.
*/
/**************************************************************************/
static int isSPIInitialised(void)
{
  // Check SPI  flag in PCONP register
  if (SCB_PCONP & (1 << 8)) // PCONP Bit 8 = SCB_PCONP_PCSPI0
  {
    // ToDo: This is a very weak test ... check the enable flags, etc.!
    return 1;
  }
  else
  {
    return 0;
  }
}

/**************************************************************************/
/*! 
    Initialises SPI0
*/
/**************************************************************************/
static int monitorSPIInit (int argc, portCHAR **argv __attribute__ ((unused)))
{
  spiInit();
  printf("SPI bus initialised\r\n");

  return 0;
}

/**************************************************************************/
/*! 
    Writes an 8-byte value to SPI0 and displays the results
*/
/**************************************************************************/
static int monitorSPIWrite (int argc, portCHAR **argv)
{
  U8 results;

  if (!isSPIInitialised())
  {
    printf("Error: SPI has not been initialised.  Use 'spi init'\r\n");
    return 0;
  }

  unsigned int value;   // value to send to SPI bus

  if (!getNumber (argv[0], &value))
    return 0;

  if (value > 255)
  {
    printf ("Error: data must be 0x00..0xff (0..255)\r\n");
    return 0;
  }

  results = spiPut(value);

  printf ("spiPut() returned 0x%X (%d)\r\n", results, results);

  return 0;
}
#endif

#ifdef CFG_UIP
/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int getIPAddress (char *s, unsigned int *ipAddress)
{
  char *p = s;
  char *t = NULL;
  int i;

  for (*ipAddress = 0, i = 0; i < 4; i++)
  {
    unsigned int ipOctet;

    if (i != 3)
    { 
      if ((t = strchr (p, '.')) && (t != p))
        *t = '\0';
      else
      { 
        printf ("Incorrectly formatted IP address\r\n");
        return 0;
      }
    }

    if (!*p)
    { 
      printf ("Missing octet in IP address\r\n");
      return 0;
    }

    if (!isDecimalString (p))
    {
      printf ("Bad octet value in IP address: \"%s\"\r\n", p);
      return 0;
    }

    getNumber (p, &ipOctet);

    if (ipOctet > 255)
    {
      printf ("Illegal octet value: \"%s\"\r\n", p);
      return 0;
    }

    *ipAddress = (*ipAddress << 8) | ipOctet;
    p = t + 1;
  }

  return 1;
}

/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int atoh (char *s, unsigned int *r)
{
  unsigned int i;
  unsigned int l;
  char buffer [(l = strlen (s)) + 3];

  for (i = 0; i < l; i++)
    if (!isxdigit (s [i]))
      return 0;

  strcpy (buffer, "0x");
  strcat (buffer, s);

  return getNumber (buffer, r);
}

/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int monitorUIPDHCP (int argc, portCHAR **argv __attribute__ ((unused)))
{
#ifdef CFG_DHCP
  if (!argc)
    printf ("DHCP is %s\r\n", uipDHCPC  ? "on" : "off");
  else if (!strcmp (argv [0], "on"))
    uipDHCPC = TRUE;
  else if (!strcmp (argv [0], "off"))
    uipDHCPC = FALSE;
  else 
    printf ("State must be 'on' or 'off'\r\n");
#else
  printf ("DHCP support not enabled\r\n");
  argc = argc;
#endif

  return 0;
}

#ifdef CFG_TELNETD
/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int monitorUIPDrop (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  telnetdDisconnect ();

  return 0;
}
#endif

/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int monitorUIPGW (int argc, portCHAR **argv __attribute__ ((unused)))
{
  uip_ipaddr_t addr;

  if (!argc)
  {
    uip_getdraddr (&addr);

    printf ("%d.%d.%d.%\r\n", uip_ipaddr1 (&addr), uip_ipaddr2 (&addr), uip_ipaddr3 (&addr), uip_ipaddr4 (&addr));
  }
  else
  {
    unsigned int ipaddr;

    if (getIPAddress (argv [0], &ipaddr))
    {
      uip_ipaddr (&addr, (ipaddr >> 24) & 0xff, (ipaddr >> 16) & 0xff, (ipaddr >> 8) & 0xff, ipaddr & 0xff);
      uip_setdraddr (&addr);
    }
  }

  return 0;
}

/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int monitorUIPIP (int argc, portCHAR **argv __attribute__ ((unused)))
{
  uip_ipaddr_t addr;

  if (!argc)
  {
    uip_gethostaddr (&addr);

    printf ("%d.%d.%d.%d\r\n", uip_ipaddr1 (&addr), uip_ipaddr2 (&addr), uip_ipaddr3 (&addr), uip_ipaddr4 (&addr));
  }
  else
  {
    unsigned int ipaddr;

    if (getIPAddress (argv [0], &ipaddr))
    {
      uip_ipaddr (&addr, (ipaddr >> 24) & 0xff, (ipaddr >> 16) & 0xff, (ipaddr >> 8) & 0xff, ipaddr & 0xff);
      uip_sethostaddr (&addr);
    }
  }

  return 0;
}

/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int monitorUIPMAC (int argc, portCHAR **argv __attribute__ ((unused)))
{
  if (!argc)
    printf ("%02x:%02x:%02x:%02x:%02x:%02x\r\n", uip_ethaddr.addr [0], uip_ethaddr.addr [1], uip_ethaddr.addr [2], uip_ethaddr.addr [3], uip_ethaddr.addr [4], uip_ethaddr.addr [5]);
  else if (uipIsRunning ())
    printf ("uIP must be stopped with 'uip stop' before changing MAC address\r\n");
  else
  {
    int l;

    if ((l = strlen (argv [0]) != 17))
      printf ("MAC address must be formatted as xx:yy:zz:aa:bb:cc\r\n");
    else
    {
      int i;
      int o;
      struct uip_eth_addr mac;

      for (o = 0, i = 0; i < 6; i++, o += 3)
      {
        unsigned int v;

        argv [0][o + 2] = '\0';

        if (!atoh (&argv [0][o], &v))
        {
          printf ("Badly formatted MAC address: \"%s\"\r\n", &argv [0][o]);
          return 0;
        }

        mac.addr [i] = v;
      }

      uip_setethaddr (mac);
    }
  }

  return 0;
}

/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int monitorUIPNM (int argc, portCHAR **argv __attribute__ ((unused)))
{
  uip_ipaddr_t addr;

  if (!argc)
  {
    uip_getnetmask (&addr);

    printf ("%d.%d.%d.%d\r\n", uip_ipaddr1 (&addr), uip_ipaddr2 (&addr), uip_ipaddr3 (&addr), uip_ipaddr4 (&addr));
  }
  else
  {
    unsigned int ipaddr;

    if (getIPAddress (argv [0], &ipaddr))
    {
      uip_ipaddr (&addr, (ipaddr >> 24) & 0xff, (ipaddr >> 16) & 0xff, (ipaddr >> 8) & 0xff, ipaddr & 0xff);
      uip_setnetmask (&addr);
    }
  }

  return 0;
}

#ifdef CFG_SNTP
/**************************************************************************/
/*! 

*/
/**************************************************************************/
static void monitorUIPSNTPTimeSynced (time_t *epochSeconds)
{
  if (epochSeconds)
  {
    n32_t to;
      
    uip_gettimeoffset (&to);
    *epochSeconds += to;
#ifdef CFG_RTC
    rtcSetEpochSeconds (*epochSeconds);
    printf ("\r\n");
    monitorRTCGet (0, NULL);
#else
    printf ("\r\nRTC support not enabled, time is %ld epoch seconds\r\n", *epochSeconds);
#endif
  }
  else
    printf ("\r\nCouldn't get time from SNTP server\r\n");

  printf (CFG_MONITOR_PROMPT);
  // fflush (stdout);
}

/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int monitorUIPSNTP (int argc, portCHAR **argv)
{
  uip_ipaddr_t addr;

  if (!argc)
  {
    uip_getsntpaddr (&addr);

    printf ("%d.%d.%d.%d\r\n", uip_ipaddr1 (&addr), uip_ipaddr2 (&addr), uip_ipaddr3 (&addr), uip_ipaddr4 (&addr));
  }
  else
  {
    if (!strcmp (argv [0], "set"))
    {
      uip_getsntpaddr (&addr);

      if (uip_iszeroaddr (&addr))
      {
        printf ("NTP server IP address not set\r\n");
        return 0;
      }
    }
    else
    {
      unsigned int ipaddr;

      if (!getIPAddress (argv [0], &ipaddr))
        return 0;

      uip_ipaddr (&addr, (ipaddr >> 24) & 0xff, (ipaddr >> 16) & 0xff, (ipaddr >> 8) & 0xff, ipaddr & 0xff);
      uip_setsntpaddr (&addr);
    }

    if (!sntpSync (&addr, monitorUIPSNTPTimeSynced))
      printf ("sntpSync() returned 0 (no UDP entries available?)\r\n");
  }

  return 0;
}
#endif

/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int monitorUIPStart (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  switch (uipStart (uipDHCPC))
  {
    case UIPSTATE_ALREADYRUNNING :
      printf ("uIP task is already running.  Use 'uip stop' to stop it\r\n");
      break;

    case UIPSTATE_TASKDIDNTSTART :
      printf ("uIP task failed to start after 5 seconds (reason not known)\r\n");
      break;

    case UIPSTATE_ENCJ28C60ERR :
      printf ("ENC28J60 on SPI0 failed to initialize\r\n");
      break;

    default :
      break;
  }

  return 0;
}

/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int monitorUIPStop (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  switch (uipStop ())
  {
    case UIPSTATE_NOTRUNNING :
      printf ("uIP task is not yet running.  Use 'uip start' to start it\r\n");
      break;

    default : 
      break;
  }

  return 0;
}

/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int monitorUIPStats (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  void uip_printstats (void);

  uip_printstats ();

  return 0;
}

#ifdef CFG_SNTP
/**************************************************************************/
/*! 

*/
/**************************************************************************/
static int monitorUIPTO (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  if (!argc)
  {
    n32_t to;

    uip_gettimeoffset (&to);
    printf ("Offset is %d seconds from UTC\r\n", to);
  }
  else
  {
    unsigned int to;

    if (!getNumber (argv [0], &to))
      return 0;

    uip_settimeoffset ((n32_t *) &to);
  }
  return 0;
}
#endif
#endif


/**************************************************************************/
/*! 
    Display the monitor version and copyright information
*/
/**************************************************************************/
static int monitorVersion (int argc __attribute__ ((unused)), portCHAR **argv __attribute__ ((unused)))
{
  printf ("%s Version %d.%d,\r\n", CFG_PROJECT_NAME, CFG_PROJECT_VERSION_MAJOR, CFG_PROJECT_VERSION_MINOR);
  printf ("%s \r\n", CFG_PROJECT_COPYRIGHT);

  return 0;
}

// 
// FreeRTOS Task Code
// 

/**************************************************************************/
/*! 
    Main FreeRTOS task handling
*/
/**************************************************************************/
static portTASK_FUNCTION (vMonitorTask, pvParameters __attribute__ ((unused)))
{
  static xQueueHandle rxQueue = NULL;
  static U8 buffer [256];
  static portCHAR *argv [34];
  int argc;

  // Get a reference to the appropriate RX queue
  #ifdef CFG_MONITOR_UART0
  uart0GetRxQueue (&rxQueue);
  #endif
  #ifdef CFG_MONITOR_USBSER
  usbserGetRxQueue (&rxQueue);
  #endif

  monitorVersion (0, NULL);

#if defined CFG_UIP && defined CFG_AUTO_UIP
  // Automatically start uIP if requested
  uipStart (CFG_AUTO_DHCP_VALUE);
#endif

  for (;;)
  {
    int l;

    if ((l = argsGetLine (rxQueue, buffer, sizeof (buffer), CFG_MONITOR_PROMPT)))
    {
        if (argsParse ((char *) buffer, argv, sizeof (argv), &argc))
        {
          printf ("Too many arguments (max %ld)\r\n", arrsizeof (argv));
        }
        else if (argv [0])
        {
          argsDispatch (commandList, argc, &argv [0]);
        }
    }
  }
}

/**************************************************************************/
/*! 
    Starts the monitor FreeRTOS task
*/
/**************************************************************************/
signed portBASE_TYPE monitorTaskStart (void)
{
  return xTaskCreate (vMonitorTask,  (const signed portCHAR * const) "Monitor", 1024, NULL, (tskIDLE_PRIORITY + 3), &taskHandles [TASKHANDLE_MONITOR]);
}
