#ifndef LOOP_H
#define LOOP_H

#include <Arduino.h>


#include <stdlib.h>
#include <stdio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <math.h>
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    #include <EEPROM.h>
#endif

#include <TimeLib.h>
#include "timeFuncs.h"
#include "String.h"
#include <LiquidCrystal.h>
#include "PublicFuncs.h"
#include "ReadWrite.h"
#include "Events.h"
#include "UTFT.h"
#include "DS1307/RTClib.h"
#include "RFID/RFID.h"
#include <PN532_HSU.h>
#include <PN532.h>
#include "ControlValve.h"
#include "GSM/SIM800.h"
//#include "SerialEvent.h"
#include "PublicFuncVar.h"
#include "ConvDate/Date_Convert.h"
//#include "
//#undef SERIAL_DEBUG
#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#define IF_SERIAL_DEBUG(x) ({x;})
#else
#define IF_SERIAL_DEBUG(x)
#endif

void Setup_GPRS(CGPRS_SIM800 gprs_);
void ShowMessageOnStartup(String msg);
boolean BatteryCharzheError();
void GetDateFromObisValue(char *value, TimeStruct *RetValue) ;

void GetDateToObisValue(char *value, TimeStruct *RetValue) ;
void IEC_C_CheckModeTimeOut(boolean reset);
struct CharMemoryAlocated *GetDailEventRecords(const char *from_day, const char *to_day);
struct CharMemoryAlocated *GetHourlyLogFile(const char *from_day, const char *to_day);


#endif
