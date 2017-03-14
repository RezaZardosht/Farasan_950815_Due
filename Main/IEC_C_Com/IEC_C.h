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
 * @file   IEC_C.h
 * 
 * @brief  Main include file for the Freescada libIEC_C library.
 *
 * Include this file to access the libIEC_C API:
\verbatim
#include <IEC_C.h>
\endverbatim
 *
 */

/*! \mainpage libIEC_C
 *
 * These pages contain automatically generated documentation for the libIEC_C
 * API. For examples on how to use the libIEC_C library, see the applications
 * in the bin directory in the source code distribution.
 *
 * For more information, see http://www.rscada.se/libIEC_C
 *link
 */

#ifndef _IEC_C_H_
#define _IEC_C_H_

//#undefine HANDHELD
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
//#include "../loop.h"
#include "IEC_C_protocol.h"
#include "IEC_C_serial.h"
#include "IEC_Cconfig.h"
#include "../PublicFuncVar.h"

//#include <Arduino.h>
//int ardprintf(char *str, ...);
byte Get_IEC_ManufactureXXXIdentByte(int byteNum);
int  Get_IEC_Identify_Len();
byte Get_IEC_IdentifyByte(int byteNum);
void IEC_C_CheckModeTimeOut(boolean reset=false);
int MY_ModBus_Adress();
typedef enum irc_state  {
    Wait_SignOn,
    Wait_SelectMode,
    Wait_Password,
    InMode_DataReadOut,
    InMode_ProgramingP1,
    InMode_ProgramingP2,
    HandHeldRouter
}_IEC_state;
typedef enum handHeld_rfid  {
    HandHeld,
    RFID,
    nothing

}_HandHeldRFID_Communication;

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
int IEC_C_init();


#endif /* _IEC_C_H_ */

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

#define DataPackRecieveOK 0

int ReadInformToRFidCard(_RFIDCardInform *RFIDCardInform__) ;
int WriteInformToRFidCard(_RFIDCardInform *RFIDCardInform__) ;



char *GetPassword_1();
char *GetPassword_2();
char *FtoStr(float f, char (&FStr)[20], int DecPoint);
int printf_New(char *str, ...);
unsigned int freemeMory();
