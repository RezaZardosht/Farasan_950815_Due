//------------------------------------------------------------------------------
// Copyright (C) 2011, Robert Johansson, Raditex AB
// All rights reserved.
//
// rSCADA 
// http://www.rSCADA.se
// info@rscada.se
//
//------------------------------------------------------------------------------

/**
 * @file   mbus-serial.h
 * 
 * @brief  Functions and data structures for sending M-Bus data via RS232.
 *
 */

#ifndef MBUS_SERIAL_H
#define MBUS_SERIAL_H

//#include <termios.h>
#include "mbus.h"
#include "mbusprotocol.h"
#define SerialMBUS Serial2

//SoftwareSerial *mbus_serial_connect(char *device);
int                 mbus_serial_disconnect(HardwareSerial  *handle);
int                 mbus_serial_send_frame(mbus_frame *frame);
int                 mbus_serial_recv();
int                 mbus_serial_set_baudrate(HardwareSerial *handle, int baudrate);
#endif /* MBUS_SERIAL_H */



