//------------------------------------------------------------------------------
// Copyright (C) 2010, Raditex AB
// All rights reserved.
//
// rSCADA 
// http://www.rSCADA.se
// info@rscada.se
//
//------------------------------------------------------------------------------

/**
 * @file   mbus.h
 * 
 * @brief  Main include file for the Freescada libmbus library.
 *
 * Include this file to access the libmbus API:
\verbatim
#include <mbus.h>
\endverbatim
 *
 */

/*! \mainpage libmbus 
 *
 * These pages contain automatically generated documentation for the libmbus
 * API. For examples on how to use the libmbus library, see the applications
 * in the bin directory in the source code distribution.
 *
 * For more information, see http://www.rscada.se/libmbus
 *link
 */

#ifndef _MBUS_H_
#define _MBUS_H_
#define  M_BusSerial Serial1
#define  DebugSerial Serial
#include "mbusprotocol.h"
#include "mbusprotocolaux.h"
#include "mbusserial.h"
#include "mbus_config.h"
#include <stdarg.h>

//#include <Arduino.h>
int ardprintf(char *str, ...);
int MY_ModBus_Adress();

//
//
//
/*
0x79 	Set Secondary Address
0x7A   	Set Primary Address
0x14    Volume (Total)  0.01 m3
0x4E    Volume flow (Max) l/s
0x26    Operating time(Pump Run Time) h
0xFD  	A standard VIFE from extension table follows
0xFF 	A further manufacturer specific VIFE follows

 Follow by VIFE = FD
 0x0C 	Firmware Version
0x19  	Security key
0x30  	Start Date of Intervals
0x38  	Duration of intervals

 Follow by VIFE = FF
 0x11	Volume (Remaining of Current Interval)  0.01m3
0x12	Credit(Volume)  m3
0x13 	Power Down  //date
0x14	Power Up   //date
0x15	Replace Battery  //0-1
0x16	Application error  value
0x17	Firmware activated // date
0x18	Credit Assignment //
0x19	Strong DC Magnetic field Detected //date
0x1A	Meter cover removed //date
0x1B 	Event log cleared   //date
0x1C	Flow rate Exceeded  //date
0x1D 	Permitted volume threshold exceeded //date
0x1E	Electrical Current Disconnected //date
0x1F 	Electrical Current Connected //date
 0x20  NULL
0x21	Tampered Water Flow Detected // date
0x22 	Successful Authentication //date
0x23	Authentication failed //date
0x24 	Operational key changed //date
0x25	Secret1 for secure algorithm has changed //date
0x26 	Secret2 for secure algorithm has changed //date
0x27	Clock adjusted
0x28 	Clock adjusted 2
0x29	Set the status of connect- disconnect event
0x2A	Reset the status of connect- disconnect event
0x2B	Set the start date of irrigation year
0x2C 	Set duration of intervals
0x2D	Set permitted volume threshold for intervals
0x2E 	Set security key
0x2F	Master key changed
0x31	M-Bus cable disconnected
 */
int mbus_init();


#endif /* _MBUS_H_ */

;
#ifndef ARDPRINTF
#define ARDPRINTF
#undef ARDBUFFER
#endif
#define SERIAL_DEBUG

//#undef SERIAL_DEBUG
#ifdef SERIAL_DEBUG
#define IF_SERIAL_DEBUG(x) ({x;})
#else
#define IF_SERIAL_DEBUG(x)
#endif
mbus_Ndata_record *MBUS_CumulativeVolume();
mbus_Ndata_record *MBUS_MAX_Daily_Volume_Fellow();
mbus_Ndata_record *MBUS_CumulativPumpHourWork();
mbus_Ndata_record *MBUS_RemainingVolume();

mbus_Ndata_record *MBUS_MakeHeaderOfRequestParameters();
void MBUS_MakeArrayOfRequestParameters(mbus_frame *replay_frame);
int add_NrecordPackFrame(mbus_frame *replay_frame,mbus_Ndata_record *Nrecord);
void mbus_Ndata_record_free(mbus_Ndata_record *record);