//------------------------------------------------------------------------------
// Copyright (C) 2010-2011, Robert Johansson, Raditex AB
// All rights reserved.
//
// rSCADA
// http://www.rSCADA.se
// info@rscada.se
//
//------------------------------------------------------------------------------

/**
   @file   IEC_C-protocol.h

   @brief  Functions and data structures for M-Bus protocol parsing.

*/

#ifndef _IEC_C_PROTOCOL_H_
#define _IEC_C_PROTOCOL_H_

//#include <stdlib.h>
//#include <stdint.h>
#include <assert.h>
#include "../TimeFuncs.h"


#include "IEC_Cconfig.h"
#define MAX_OBIS_ADDRESS_LENGTH 100
#define MAX_OBIS_VALUE_LENGTH 200

/////////////alaki
typedef struct _IEC_OBIS_Ndata_record {
    char Obis_Address[10];
    char Obis_value[10];
    char format[5];

    char *ToString() {
        char msg[230];
        sprintf(msg, format, Obis_Address, Obis_value);
    }

    size_t data_len;

    char msg[150];

    _IEC_OBIS_Ndata_record *next;

} Iec_OBIS_Ndata_record;

typedef struct _IEC_C_frame {

    uint8_t start1;
    uint8_t length1;
    uint8_t length2;
    uint8_t start2;
    uint8_t control;
    uint8_t address;
    uint8_t control_information;
    // variable data field
    uint8_t checksum;
    uint8_t stop;

    uint8_t data[252];
    size_t data_size;

    int type;
    long timestamp;

    //IEC_C_frame_data frame_data;

    _IEC_C_frame *next; // pointer to next IEC_C_frame for multi-telegram replies
    _IEC_OBIS_Ndata_record *Iec_record;
} IEC_C_frame;


#define NITEMS(x) (sizeof(x)/sizeof(x[0]))

//
// Supported handle types
//
#define IEC_C_HANDLE_TYPE_TCP    0
#define IEC_C_HANDLE_TYPE_SERIAL 1

//
// FIXED LENGTH DATA FORMAT


#define IEC__FRAME_TYPE_ACK     0x01
#define IEC_C_FRAME_TYPE_SHORT   0x02
#define IEC_C_FRAME_TYPE_CONTROL 0x03
#define IEC_C_FRAME_TYPE_LONG    0x04

#define IEC_C_FRAME_TYPE_IDENT_REPLAY    0xA5
#define IEC_C_FRAME_TYPE_READOUT_REPLAY    0xA6
#define IEC_C_FRAME_TYPE_PROGRAMING_FIRST_REPLAY    0xA7
#define IEC_C_FRAME_TYPE_RFID_REPLAY  0xA8
#define IEC_C_FRAME_TYPE_REPLAY_INPROGMOD_1  0xA9

#define IEC_C_DATA_READOUTMODE_SELECT  '0'
#define IEC_C_DATA_PROGRAMING_SELECT '1'
#define IEC_C_FRAME_ACK_BASE_SIZE        1
#define IEC_C_FRAME_SHORT_BASE_SIZE      5
#define IEC_C_FRAME_LONG_BASE_SIZE       9


#define IEC_C_FRAME_IDENTIFY_REQ_MIN_SIZE 6
#define IEC_C_FRAME_IDENTIFY_REQ_MAX_SIZE 12
#define IEC_C_FRAME_PASSWORD_MIN_SIZE 13
#define IEC_C_FRAME_PASSWORD_MAX_SIZE 70
#define IEC_C_FRAME_SELECT_MODE_SIZE 6


//
// Frame start/stop bits
////*╗,-▲▼·!"#$%&'()*+,-./♦0:;<=>?"
#define IEC_C_IDENTIFY_BYTE_START     47 //  '/'
#define IEC_C_IDENTIFY_REQ_BYTE_SEC       63 //  '?'
#define IEC_C_ACK_Const 06  //% chould be change to 06
#define IEC_C_NAK_Const 15  //> chould be change to 15
#define IEC_C_SOH_Const 1  //& chould be change to 1
#define IEC_C_STX_Const 2  //? chould be change to 2
#define IEC_C_ETX_Const 3  //@ chould be change to 3
#define IEC_CR_CHARACTER 13  //# chould be change to 13
#define IEC_LF_CHARACTER 10  //$ chould be change to 10


#define IEC_C_FRAME_SHORT_START   0x10

#define IEC_C_FRAME_LONG_START    0x68
#define IEC_C_FRAME_STOP          0x16

#define CommunicateEndRequestPacketLen 5

//
// Event callback functions
//
extern void (*_IEC_C_recv_event)(uint8_t src_type, const uint8_t *buff, size_t len);

extern void (*_IEC_C_send_event)(uint8_t src_type, const uint8_t *buff, size_t len);

void IEC_C_dump_recv_event(uint8_t src_type, const uint8_t *buff, size_t len);

void IEC_C_dump_send_event(uint8_t src_type, const uint8_t *buff, size_t len);

//
// Event register functions
//
void IEC_C_register_recv_event(void (*event)(uint8_t src_type, const uint8_t *buff, size_t len));

void IEC_C_register_send_event(void (*event)(uint8_t src_type, const uint8_t *buff, size_t len));

//
// variable length records
//


//
//
//
int IEC_C_frame_calc_checksum(IEC_C_frame *frame);

int IEC_C_frame_calc_length(IEC_C_frame *frame);

//
// Parse/Pack to bin
//
int IEC_C_parse(uint8_t *data, size_t data_size);


int IEC_C_frame_pack(IEC_C_frame *frame, uint8_t *data, size_t data_size);

Iec_OBIS_Ndata_record *Iec_OBIS_Ndata_record_new(char *Address, char *Value);

void Iec_OBIS_Ndata_record_free(Iec_OBIS_Ndata_record *record);

Iec_OBIS_Ndata_record *
AddNewIec_OBIS_Ndata_record_ToLinkList(Iec_OBIS_Ndata_record *record, char *Address, char *Value);
bool GetSeperatedObisAddressObisValue(uint8_t *data, char *ObisAddress, char *ObisValue);

#endif /* _IEC_C_PROTOCOL_H_ */

