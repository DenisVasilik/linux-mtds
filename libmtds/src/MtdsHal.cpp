/************************************************************************/
/*																		*/
/*	MtdsHal.cpp	--	MTDS HAL Layer for Microblaze/Zynq					*/
/*																		*/
/************************************************************************/
/*	Author: 	Thomas Kappenman										*/
/*	Copyright 2016, Digilent, Inc. All rights reserved.					*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This module contains the hardware abstraction layer implementation	*/
/*	for the MTDS library for use on the MPIDE and Arduino platforms.	*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	2016-10-1(TommyK): Created based on MtdsHal.cpp for Arduino			*/
/*	2016-10-30(SamB): Code/comment cleanup, change Init function name	*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <stdexcept>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdexcept>

#include "ProtoDefs.h"
#include "mtds.h"
#include "MtdsHal.h"
 
/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

/* Symbols used for initializing and operating the SPI controller.
*/
#define SPI_MODE0 ((0 << _SPICON_CKP)|(1 << _SPICON_CKE))		// CKP = 0 CKE = 1
#define SPI_MODE1 ((0 << _SPICON_CKP)|(0 << _SPICON_CKE))		// CKP = 0 CKE = 0
#define SPI_MODE2 ((1 << _SPICON_CKP)|(1 << _SPICON_CKE))		// CKP = 1 CKE = 1 
#define SPI_MODE3 ((1 << _SPICON_CKP)|(0 << _SPICON_CKE))		// CKP = 1 CKE = 0

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/***	MtdsHalInit(pinSel)
**
**	Parameters:
**		pinSel		- digital pin number of select pin
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function performs any system initialization required for the
**		platform. This can include initializing timers, settting up pin
**		direction, and so on. The pinSel parameter is specifically used in
**		the Arduino/MPIDE environment and may or may not be used on other
**		platforms.
*/
static int mtdsHandle = 0;

void MtdsHalInit(int pinSel) {
	mtdsHandle = open(MTDS_DEVICE_NAME, O_NONBLOCK);

    if (mtdsHandle < 0)
    {
        std::ostringstream message;
        message << "Could not open device " << MTDS_DEVICE_NAME << " errno:" << errno;
        // throw std::runtime_error(message.str());
    }
}

/* ------------------------------------------------------------ */
/***	MtdsHalEnableStatusPin(idPin
**
**	Parameters:
**		idPin		- status pin id number of pin to enable
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Enable the specified status pin. This means make it be an input.
**
**		When building for the Multi-Touch Display Shield, the pin numbers for
**		the status pins are defined in the MtdsHal.h header file. When building
**		for the PmodMtds, we have to compute the pin numbers based on the Pmod
**		connector the board is plugged into. We can get this as a constant offset
**		from the pin number of ch SPI CS pin, pinMtdsSel. For now, we assume that
**		if we are being built for a board with Pmod connectors, we are using the
**		PmodMtds.
*/

void MtdsHalEnableStatusPin(int idPin) {
	
}

/* ------------------------------------------------------------ */
/***	MtdsHalGetStatusPin(idPin)
**
**	Parameters:
**		idPin		- id of status pin to read
**
**	Return Values:
**		Returns true if pin is high, false if pin is low
**
**	Errors:
**		none
**
**	Description:
**		Return the state of the specified status pin.
*/

bool MtdsHalGetStatusPin(int idPin) {
	return false;
}

/* ------------------------------------------------------------ */
/***	MtdsHalResetDisplay(pinSel)
**
**	Parameters:
**		pinSel		- digital pin number for SPI CS pin
**
**	Return Values:
**		Returns true if reset actually performed, false if not
**
**	Errors:
**		none
**
**	Description:
**		This function is used to reset the display board on hardware
**		and platforms where the display board can be reset. 
**		The Multi-Toucn	Display Shield doesn't support software controlled
**		reset and is automatically reset whenever the host board is reset.
**		The PmodMTDS doesn't get reset automatically and needs to be reset
**		under software control when the system initializes.
**		The pinSel parameter can be used to determine which pin needs to
**		be toggled to reset the display.
*/

bool MtdsHalResetDisplay(int pinSel) {
	return false;
}

/* ------------------------------------------------------------ */
/***	MtdsHalInitSpi(pspiInit, frq)
**
**	Parameters:
**		pspiInit		- indicator for the SPI port number
**		frq				- spi clock frequency to use
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function initializes the SPI port being used by the platform
**		for communications between the host and the display interface. Depending
**		on the platform, one of both of the paramters may be ignored.
*/

void MtdsHalInitSpi(uint32_t pspiInit, uint32_t frq) {
    struct mtds_data mtdsData = {0};

    if (ioctl(mtdsHandle, IOC_INIT, &mtdsData) < 0)
    {
        std::ostringstream message;
        message << "Ioctl error " << MTDS_DEVICE_NAME << " errno:" << errno;
        // throw std::runtime_error(message.str());
    }
}

/* ------------------------------------------------------------ */
/***	MtdsHalEnableSlave(fEn)
**
**	Parameters:
**		fEn		- true to enable, false to disable
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Enable/Disable the SPI slave (display board).
*/

void MtdsHalEnableSlave(bool fEn) {
	struct mtds_data mtdsData = {0};

	mtdsData.slave = fEn;

    if (ioctl(mtdsHandle, IOC_ENABLE_SLAVE, &mtdsData) < 0)
    {
        std::ostringstream message;
        message << "Ioctl error " << MTDS_DEVICE_NAME << " errno:" << errno;
		// throw std::runtime_error(message.str());
		printf("Ioctl error\r\n");
    }
}

/* ------------------------------------------------------------ */
/***	MtdsHalSpiReady() 
**
**	Parameters:
**		none
**
**	Return Values:
**		Returns true if the SPI port is ready to accept a byte
**
**	Errors:
**		none
**
**	Description:
**		Determine if the SPI port can accept a byte
*/

bool MtdsHalSpiReady() {
	struct mtds_data mtdsData = {0};

    if (ioctl(mtdsHandle, IOC_READY, &mtdsData) < 0)
    {
        std::ostringstream message;
        message << "Ioctl error " << MTDS_DEVICE_NAME << " errno:" << errno;
        // throw std::runtime_error(message.str());
    }

	return mtdsData.isReady;
}

/* ------------------------------------------------------------ */
/***	MtdsHalPutSpiByte(bSnd)
**
**	Parameters:
**		bSnd		- byte to write to the SPI port
**
**	Return Values:
**		Returns the byte read from the SPI port
**
**	Errors:
**		none
**
**	Description:
**		Exchange a byte with the SPI slave device (display board)
*/
uint8_t MtdsHalPutSpiByte(uint8_t bSnd) {
	struct mtds_data mtdsData = {0};

	mtdsData.bSnd = bSnd;

    if (ioctl(mtdsHandle, IOC_WRITE, &mtdsData) < 0)
    {
        std::ostringstream message;
        message << "Ioctl error " << MTDS_DEVICE_NAME << " errno:" << errno;
        // throw std::runtime_error(message.str());
    }

	return mtdsData.bRcv;
}

/* ------------------------------------------------------------ */
/***	MtdsHalTmsElapsed()
**
**	Parameters:
**		none
**
**	Return Values:
**		Returns the number of milliseconds elapsed since startup
**
**	Errors:
**		none
**
**	Description:
**		Returns the number of milliseconds that have elapsed since the host
**		started running. This is used for determining timeout for failed communications
**		between the host and the display device and for timing how long API functions
**		take to execute.
**		This function is not strictly required, and can always just return 0 if the
**		hardware resources aren't available to implement it. If it isn't implemented,
**		detection of lost communications between the host and the display board won't
**		work correctly and the host may lock up if communications isn't working reliably.
*/

uint32_t MtdsHalTmsElapsed() {
	return 0;
}

/* ------------------------------------------------------------ */
/***	MtdsHalDelayMs(tmsDelay)
**
**	Parameters:
**		tmsDelay	- number of milliseconds to delay
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Delay the specified number of milliseconds and then return
*/

void MtdsHalDelayMs(uint32_t tmsDelay) {

}

/* ------------------------------------------------------------ */
/***	MtdsHalTusElapsed()
**
**	Parameters:
**		none
**
**	Return Values:
**		Returns the number of microseconds elapsed since reset
**
**	Errors:
**		none
**
**	Description:
**		Return the number of microseconds elapsed since reset. This is only used
**		to time how long MTDS commands take to execute, so if it isn't implemented
**		or isn't accurate the only problem will be that the API function that
**		returns the time for a command to execute won't work correctly.
*/

uint32_t MtdsHalTusElapsed() {
	return 0;
}

/* ------------------------------------------------------------ */
/***	MtdsHalDelayUs(usDelay)
**
**	Parameters:
**		usDelay		- number of microseconds to delay
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Delay the specified number of microseconds and then return. The delay time does not
**		need to be very precise, but must be at least as long as the number of microseconds
**		requested.
*/

void MtdsHalDelayUs(uint32_t usDelay) {

}


/* ------------------------------------------------------------ */
/*				XXXX Object Class Implementation				*/
/* ------------------------------------------------------------ */

/***	ProcName
**
**	Parameters:
**
**	Return Values:
**
**	Errors:
**
**	Description:
**
*/

/* ------------------------------------------------------------ */

/************************************************************************/

