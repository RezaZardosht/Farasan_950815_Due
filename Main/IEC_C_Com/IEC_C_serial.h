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
 * @file   IEC_C_-serial.h
 * 
 * @brief  Functions and data structures for sending M-Bus data via RS232.
 *
 */

#ifndef IEC_C_SERIAL_H
#define IEC_C_SERIAL_H




//SoftwareSerial *IEC_C_serial_connect(char *device);
int                 IEC_C_serial_disconnect(HardwareSerial  *handle);
int                 IEC_C_serial_send_frame(IEC_C_frame *frame);
int                 IEC_C_serial_recv();
int                 SerialIECWithPC_recv();
int                 IEC_C_serial_set_baudrate( int baudrate);
#endif /* IEC_C_SERIAL_H */



