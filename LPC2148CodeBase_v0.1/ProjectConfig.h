/**************************************************************************/
/*! 
    \file     ProjectConfig.h
    \author   K. Townsend (microBuilder.eu)
    \date     25 November 2009
    \version  1.00

    ProjectConfig.h contains various definitions and enumerations that
    determine how the project works.  By keeping this config information 
    in one central location it will often be easier to adjust the
    behaviour of your program, rather than having to search through
    dozens or hundreds of files to enable or disable different flags
    or find the name of a task handler, etc.
*/
/**************************************************************************/

#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

// Included Peripherals
// ====================
// In order to make the code more portable, you can disable
// certain peripherals that may not be available on all
// devices (for example, RTC isn't available on certain chips)

#define CFG_RTC

// Included Drivers
// ================
// You can comment out these definitions to remove any unused
// drivers from your compiled code, saving space and reducing
// any risk of errors

#define CFG_LED
#define CFG_TCS3414
#define CFG_TPS851

// General Project Settings
// ========================

/**************************************************************************/
/*! 
    General project settings
*/
/**************************************************************************/
#define CFG_PROJECT_NAME              (portCHAR *) "LPC2148 Reference Board"  // The name of this project if you wish to display it
#define CFG_PROJECT_VERSION_MAJOR     0                                       // The 'major' version of this project
#define CFG_PROJECT_VERSION_MINOR     1                                       // The 'minor' version of this project
#define CFG_PROJECT_COPYRIGHT         (portCHAR *) "(C) microBuilder.eu 2009" // The copyright notice for this project if you wish to display it

/**************************************************************************/
/*! 
    I2C settings
*/
/**************************************************************************/
#define CFG_I2C_DEFAULT_TIMEOUT       100                                     // Default timeout, in milliseconds for generic read/write
#define CFG_I2C_DEBUG                 1                                       // Whether I2C debug information (i2CDump) is available or not

/**************************************************************************/
/*! 
    Whether the LED "blinky" task should be started after reset/boot
*/
/**************************************************************************/
#define CFG_LED_RUNLEDONSTARTUP       1                                       // Run the led task on startup (to confirm board is OK)

/**************************************************************************/
/*! 
    Configuration settings for Chibi 802.15.4 wireless stack
*/
/**************************************************************************/
// #define CFG_CHIBI
// #define CFG_CHB_DEBUG

/**************************************************************************/
/*! 
    Configuration settings for FATFS and SD Cards
*/
/**************************************************************************/
#define CFG_FATFS


/**************************************************************************/
/*! 
    Configuration settings for uIP
*/
/**************************************************************************/
// #define CFG_UIP           // Enable uIP w/ ENC28J60 on SPI0
#define CFG_TELNETD       // Enable telnet server under uIP
#define CFG_HTTPD         // Enable http server under uIP
#define CFG_SNTP	  // Enable sntp client under uIP
#define CFG_DHCP          // Enable DHCP under uIP
#define CFG_AUTO_UIP      // Automatically start uIP at reboot (ignored if not CFG_UIP)
#define CFG_AUTO_DHCP     // Automatically to DHCP for an address at start (ignored if not CFG_UIP, CFG_DHCP and CFG_AUTO_UIP)
#define CFG_AUTO_SNTP     // Automatically to set the time via SNTP (ignored if not CFG_UIP and CFG_AUTO_UIP)

//  No UIP means no servers and no auto stuff
#ifndef CFG_UIP
#undef CFG_TELNETD
#undef CFG_HTTPD
#undef CFG_SNTP
#undef CFG_DHCP
#undef CFG_AUTO_UIP
#undef CFG_AUTO_DHCP
#undef CFG_AUTO_SNTP
#endif

//  Can't auto DHCP unless there's DHCP support, and auto requested
#if !defined CFG_DHCP || !defined CFG_AUTO_UIP
#undef CFG_AUTO_DHCP
#endif

//  We need an lvalue to pass to uipStart() to DHCP or not
#ifdef CFG_AUTO_DHCP
#define CFG_AUTO_DHCP_VALUE 1
#else
#define CFG_AUTO_DHCP_VALUE 0
#endif

/**************************************************************************/
/*! 
    Monitor settings

    CFG_MONITOR_UART0 should be defined if you wish to use UART0 for the 
    monitor, or CFG_MONITOR_USBSER if you wish to use USB Serial (CDC).
    Please note that only one of CFG_MONITOR_UART0 or CFG_MONITOR_USBSER can be
    defined at a time!  By default both operate at 9600 Baud 8N1.
*/
/**************************************************************************/
// #define CFG_MONITOR_UART0                              // Use UART0 for the monitor console
#define CFG_MONITOR_USBSER                                // Use USB Serial for the monitor console
#define CFG_MONITOR_PROMPT            "Command > "        // The 'command prompt' that will be displayed in the monitor
#define CFG_MONITOR_ALLOWRESET        1                   // Whether the user is allowed to issue the reset command
#define CFG_MONITOR_ALLOWI2C          1                   // Whether the user is allowed to send generic I2C commands
#define CFG_MONITOR_ALLOWSPI          1                   // Whether the user is allowed to send generic SPI commands



// Config Error Checking
// =====================

// Make sure that an input/output device is defined for monitor
#if !defined CFG_MONITOR_UART0 && !defined CFG_MONITOR_USBSER
    #error "You must define a monitor output: CFG_MONITOR_UART0 or CFG_MONITOR_USBSER"
#endif

// Make sure that only ONE device is defined for monitor
#if defined CFG_MONITOR_UART0 && defined CFG_MONITOR_USBSER
    #error "You can only define one of: CFG_MONITOR_UART0 or CFG_MONITOR_USBSER"
#endif



/**************************************************************************/
/*! 
    An enumeration of every xTaskHandle used for FreeRTOS 'tasks'.
    
    By defining the task handle here you can easily access any task
    without having to remember or look up the xTaskHandle name in
    dozens of different files.

    When creating a new task, you simply need to add a new task handle to
    this enumeration (anywhere before TASKHANDLE_LAST), and then reference
    the entry when you actually create your task, as in the example below:

    \code
    // Create a 'startup' task (assumes 'TASKHANDLE_STARTUP' is defined)
    xTaskCreate (vStartupTask, 
                 (const signed portCHAR * const)"Startup",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 configMAX_PRIORITIES,
                 &taskHandles[TASKHANDLE_STARTUP]);
    \endcode
*/
/**************************************************************************/
typedef enum
{
  TASKHANDLE_STARTUP,
  TASKHANDLE_MONITOR,
  TASKHANDLE_LED,
  TASKHANDLE_TCS3414,
  TASKHANDLE_TPS851,
  TASKHANDLE_UIP,
#if configGENERATE_RUN_TIME_STATS == 1
  TASKHANDLE_RUNTIMESTATS,
#endif
  TASKHANDLE_LAST
}
taskHandle_e;

void * taskHandles [TASKHANDLE_LAST];

#endif
