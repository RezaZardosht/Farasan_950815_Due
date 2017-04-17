//------------------------------------------------------------------------------
// Copyright (C) 2010-2011, Robert Johansson, Raditex AB
// All rights reserved.
//
// rSCADA
// http://www.rSCADA.se
// info@rscada.se
//
//------------------------------------------------------------------------------


#include "IEC_C_protocol.h"
#include "IEC_C.h"


_IEC_state IEC_state;
_HandHeldRFID_Communication HandHeldRFID_Communication;

int SerielTesSendFram();

int strtoint(char a[]);

//#include <MemoryFree.h>
void Get_ObisValue(char *Obis, char *RetVal);

void Set_ObisValue(char *Obis, char *OBIS_Value);

static int parse_debug = 1, debug = 1;

#define NITEMS(x) (sizeof(x)/sizeof(x[0]))





//------------------------------------------------------------------------------
// internal data
//------------------------------------------------------------------------------

//
// init event callback
//
void (*_IEC_C_recv_event)(uint8_t src_type, const uint8_t *buff, size_t len) = NULL;

void (*_IEC_C_send_event)(uint8_t src_type, const uint8_t *buff, size_t len) = NULL;

char *Obis_array[][2] = {{"0-4:96,1,0,255",  "SerialKontor"},
                         {"0-4:96,1,1,255",  "SerialConsule"},
                         {"0-4:96,1,2,255",  "CreatedDate"},
                         {"0-4:24,2,1,255",  "Total_UsedVolume"},
                         {"0-4:24,2,4,255",  "Total_UsedHourPump"},
                         {"0-4:24,2,3,255",  "TotalDuration_Charzh"},
                         {"0-4:24,2,2,255",  "LastDateUseMoreThanMaxVolume"},
                         {"0-4:24,2,15,255", "LastDateUseMoreThanMaxFellow"},
                         {"0-4:24,2,16,255", "LastDatePowerOffForUseMoreThanMaxVolume"},
                         {"0-4:24,2,17,255", "LastDatePowerOffForUseMoreThanMaxFellow"},
                         {"0-4:24,2,18,255", "CountOpenDoor"},
                         {"0-4:24,2,19,255", "LastDateUseWaterWhenPowerOff"},
                         {"0-4:24,2,20,255", "VolumeUseWhenPowerOff"},
                         {"0-4:24,2,21,255", "CountOpenConsule"},
                         {"0-4:24,2,22,255", "LastDateSeeElectroMagnetic"},
                         {"0-4:24,2,23,255", "LastTimeSeeElectroMagnetic"},
                         {"0-4:24,2,24,255", "batteryStatus"},
                         {"0-4:24,2,25,255", "InternalError"},
                         {"0-4:24,2,26,255", "NewFirmwareDate"},
                         {"0-4:24,2,27,255", "LastDateCharzhe"},
                         {"0-4:24,2,28,255", "LastUserConnectedDate"},
                         {"0-4:24,2,29,255", "LastUserConnectedCode"},
                         {"0-4:24,2,30,255", "LastDateKontorConfig"},
                         {"0-4:24,2,31,255", "SetDateDurateTaaarefe"},
                         {"0-4:24,2,32,255", "SetTaaarefeValues"},
                         {"0-4:24,2,33,255", "SetMaxFellow"},
                         {"0-4:24,2,34,255", "SetMaxVolume"},
                         {"0-4:24,2,35,255", "GetEventFile"},
                         {"0-4:24,2,36,255", "GetDailyLogFile"},
                         {"0-4:50,2,01,255", "Delete_EventFile"},
                         {"0-4:50,2,02,255", "Delete_HourlyFile"},
                         {"0-4:50,2,03,255", "Delete_DailyFile"},
                         {"0-4:50,2,04,255", "Delete_MonthlyFile"},
                         {"0-4:50,2,05,255", "Read_all_DI"},
                         {"0-4:50,2,06,255", "Set Charzhe"}
};


char BccValue(uint8_t *data, int data_size) {
    char RetVal;
    RetVal = (char) 0;
    for (int i = 1; i < data_size; i++)
        RetVal = RetVal ^ (data[i] & 0xff);
    IF_SERIAL_DEBUG(printf_New("%s:No 177 Bcc= %d \n", __PRETTY_FUNCTION__, RetVal));

    return RetVal;
}

/*
char *Obis_array[][2] = {{"0-4:96,1,0,255",  "SerialKontor"},
                         {"0-4:96,1,1,255",  "SerialConsule"},
                         {"0-4:96,1,2,255",  "CreatedDate"},
                         {"0-4:24,2,1,255",  "Total_UsedVolume"},
                         {"0-4:24,2,4,255",  "Total_UsedHourPump"},
                         {"0-4:24,2,3,255",  "TotalDuration_Charzh"},
                         {"0-4:24,2,2,255",  "LastDateUseMoreThanMaxVolume"},
                         {"0-4:24,2,15,255", "LastDateUseMoreThanMaxFellow"},
                         {"0-4:24,2,16,255", "LastDatePowerOffForUseMoreThanMaxVolume"},
                         {"0-4:24,2,17,255", "LastDatePowerOffForUseMoreThanMaxFellow"},
                         {"0-4:24,2,18,255", "CountOpenDoor"},
                         {"0-4:24,2,19,255", "LastDateUseWaterWhenPowerOff"},
                         {"0-4:24,2,20,255", "VolumeUseWhenPowerOff"},
                         {"0-4:24,2,21,255", "CountOpenConsule"},
                         {"0-4:24,2,22,255", "LastDateSeeElectroMagnetic"},
                         {"0-4:24,2,23,255", "LastTimeSeeElectroMagnetic"},
                         {"0-4:24,2,24,255", "batteryStatus"},
                         {"0-4:24,2,25,255", "InternalError"},
                         {"0-4:24,2,26,255", "NewFirmwareDate"},
                         {"0-4:24,2,27,255", "LastDateCharzhe"},
                         {"0-4:24,2,28,255", "LastUserConnectedDate"},
                         {"0-4:24,2,29,255", "LastUserConnectedCode"},
                         {"0-4:24,2,30,255", "LastDateKontorConfig"},
                         {"0-4:24,2,31,255", "SetDateDurateTaaarefe"},
                         {"0-4:24,2,32,255", "SetTaaarefeValues"},
                         {"0-4:24,2,33,255", "SetMaxFellow"},
                         {"0-4:24,2,34,255", "SetMaxVolume"},
                         {"0-4:24,2,35,255", "GetEventFile"},
                         {"0-4:24,2,36,255", "GetDailyLogFile"},
                         {"0-4:50,2,01,255", "Delete_EventFile"},
                         {"0-4:50,2,02,255", "Delete_HourlyFile"},
                         {"0-4:50,2,03,255", "Delete_DailyFile"},
                         {"0-4:50,2,04,255", "Delete_MonthlyFile"},
                         {"0-4:50,2,05,255", "Read_all_DI"},
                         {"0-2:01,1,01,001", "RFID_Charzhe"},
                         {"0-2:01,1,02,002", "RFID_Dates"}

};
*/


//
//  trace callbacks
//
void
IEC_C_dump_recv_event(uint8_t src_type, const uint8_t *buff, size_t len) {
    //  IEC_C_hex_dump("RECV", buff, len);
}

void
IEC_C_dump_send_event(uint8_t src_type, const uint8_t *buff, size_t len) {
    //  IEC_C_hex_dump("SEND", buff, len);
}

//------------------------------------------------------------------------------
/// Register a function for receive events.
//------------------------------------------------------------------------------
void
IEC_C_register_recv_event(void (*event)(uint8_t src_type, const uint8_t *buff, size_t len)) {
    _IEC_C_recv_event = event;
}

//------------------------------------------------------------------------------
/// Register a function for send events.
//------------------------------------------------------------------------------
void
IEC_C_register_send_event(void (*event)(uint8_t src_type, const uint8_t *buff, size_t len)) {
    _IEC_C_send_event = event;
}

//------------------------------------------------------------------------------
/// Return a string that contains an the latest error message.
//------------------------------------------------------------------------------
char *
IEC_C_error_str() {
    return NULL;
}

void
IEC_C_error_str_set(char *message) {
    if (message) {
        IF_SERIAL_DEBUG(printf_New(("%s"), message));
    }
}

void
IEC_C_error_reset() {
    IF_SERIAL_DEBUG(printf_New(("no errors")));
}


//------------------------------------------------------------------------------
//
// M-Bus FRAME RELATED FUNCTIONS
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Allocate an M-bus frame data structure and initialize it according to which
/// frame type is requested.
//------------------------------------------------------------------------------
IEC_C_frame *
IEC_C_frame_new(int frame_type, uint8_t *data = NULL, size_t data_size = -1) {
    IEC_C_frame *frame = NULL;
    Iec_OBIS_Ndata_record *NrecordRFID = NULL;
    Iec_OBIS_Ndata_record *NrecordNRFID = NULL;

    if ((frame = (IEC_C_frame *) malloc(sizeof(IEC_C_frame))) != NULL) {
        memset((void *) frame, 0, sizeof(IEC_C_frame));
        frame->next = NULL;
        frame->type = frame_type;
        switch (frame->type) {
            case IEC_C_FRAME_TYPE_READOUT_REPLAY:
                IF_SERIAL_DEBUG(printf_New("%s:No 1 \n", __PRETTY_FUNCTION__));

                Iec_OBIS_Ndata_record *Nrecord;
                if ((Nrecord = Iec_OBIS_Ndata_record_new("123455", "abcd")) == NULL) {
                    IF_SERIAL_DEBUG(printf_New("%s:No 11 \n", __PRETTY_FUNCTION__));
                    return NULL;
                }
                frame->Iec_record = Nrecord;
                Get_ObisValue(Obis_array[0][0], Nrecord->msg);

                frame->start1 = IEC_C_FRAME_TYPE_READOUT_REPLAY;
                //         Iec_OBIS_Ndata_record_free(Nrecord);

                Iec_OBIS_Ndata_record *NrecordN;
//27
                for (int i = 1; i < 25; i++) {

                    NrecordN = AddNewIec_OBIS_Ndata_record_ToLinkList(frame->Iec_record, "i", "a");
                    Get_ObisValue(Obis_array[i][0], NrecordN->msg);
//                    IF_SERIAL_DEBUG(printf_New("%s:No 1======== %s \n", __PRETTY_FUNCTION__, NrecordN->msg));
                }

                /*
                                Iec_OBIS_Ndata_record *NrecordN;

                 NrecordN = frame->Iec_record;
                               for (; NrecordN; NrecordN = NrecordN->next) {
                                   IF_SERIAL_DEBUG(printf_New("%s:No 2======== %s \n", __PRETTY_FUNCTION__, NrecordN->msg));
                                   delay(1000);
                               }*/
                IF_SERIAL_DEBUG(printf_New("%s:No 2 \n", __PRETTY_FUNCTION__));
                break;
            case IEC_C_FRAME_TYPE_RFID_REPLAY:
                IF_SERIAL_DEBUG(printf_New("%s:No 101 \n", __PRETTY_FUNCTION__));

                if ((NrecordRFID = Iec_OBIS_Ndata_record_new("123455", "abcd")) == NULL) {
                    IF_SERIAL_DEBUG(printf_New("%s:No 111 \n", __PRETTY_FUNCTION__));
                    return NULL;
                }
                frame->Iec_record = NrecordRFID;
                Get_ObisValue(Obis_array[0][0], NrecordRFID->msg);

                frame->start1 = IEC_C_FRAME_TYPE_RFID_REPLAY;


                NrecordNRFID = AddNewIec_OBIS_Ndata_record_ToLinkList(frame->Iec_record, "i", "a");
                Get_ObisValue("0-2:01,1,01,001", NrecordNRFID->msg);
                //          IF_SERIAL_DEBUG(printf_New("%s:No 101========Nrecord= %s \n", __PRETTY_FUNCTION__, NrecordNRFID->msg));
                IF_SERIAL_DEBUG(printf_New("%s:No 102 \n", __PRETTY_FUNCTION__));
                break;
            case IEC_C_FRAME_TYPE_REPLAY_INPROGMOD_1:
                IF_SERIAL_DEBUG(printf_New("%s:No 161 \n", __PRETTY_FUNCTION__));


                if ((NrecordRFID = Iec_OBIS_Ndata_record_new("123455", "abcd")) == NULL) {
                    IF_SERIAL_DEBUG(printf_New("%s:No 162 \n", __PRETTY_FUNCTION__));
                    return NULL;
                }
                char Obis_adress__[MAX_OBIS_ADDRESS_LENGTH];
                char ObisValue[MAX_OBIS_VALUE_LENGTH];

                GetSeperatedObisAddressObisValue(data, (char *) Obis_adress__, (char *) ObisValue);
                // Set_ObisValue("0-2:01,1,01,001", (char *) ObisValue);

                frame->Iec_record = NrecordRFID;
                Get_ObisValue(Obis_adress__, NrecordRFID->msg);
                IF_SERIAL_DEBUG(
                        printf_New("%s:NO 163--->OBIS==%s,---->Value=%s,NrecordRFID->msg=%s\n", __PRETTY_FUNCTION__,
                                   Obis_adress__, ObisValue, NrecordRFID->msg));

                frame->start1 = IEC_C_FRAME_TYPE_REPLAY_INPROGMOD_1;


                //      NrecordNRFID = AddNewIec_OBIS_Ndata_record_ToLinkList(frame->Iec_record, "i", "a");
                //      Get_ObisValue("0-2:01,1,01,001", NrecordNRFID->msg);
                //          IF_SERIAL_DEBUG(printf_New("%s:No 101========Nrecord= %s \n", __PRETTY_FUNCTION__, NrecordNRFID->msg));
                break;
            case IEC_C_FRAME_TYPE_PROGRAMING_FIRST_REPLAY:

                frame->start1 = IEC_C_FRAME_TYPE_PROGRAMING_FIRST_REPLAY;

                break;


            case IEC_C_ACK_Const:

                frame->start1 = IEC_C_ACK_Const;


                break;

            case IEC_C_NAK_Const:

                frame->start1 = IEC_C_NAK_Const;

                break;

            case IEC_C_FRAME_TYPE_LONG:

                frame->start1 = IEC_C_FRAME_LONG_START;
                frame->start2 = IEC_C_FRAME_LONG_START;
                frame->stop = IEC_C_FRAME_STOP;

                break;
        }
    }

    return frame;
}

//------------------------------------------------------------------------------
/// Free the memory resources allocated for the M-Bus frame data structure.
//------------------------------------------------------------------------------
int
IEC_C_frame_free(IEC_C_frame *frame) {
    if (frame) {
        if (frame->Iec_record != NULL)
            Iec_OBIS_Ndata_record_free(frame->Iec_record);
        if (frame->next != NULL)
            IEC_C_frame_free(reinterpret_cast<IEC_C_frame *>(frame->next));

        free(frame);
        return 0;
    }
    return -1;
}

//------------------------------------------------------------------------------
/// Caclulate the checksum of the M-Bus frame. Internal.
//------------------------------------------------------------------------------
uint8_t
calc_checksum(IEC_C_frame *frame) {
    size_t i;
    uint8_t cksum;

    assert(frame != NULL);
    switch (frame->type) {
        case IEC_C_FRAME_TYPE_SHORT:

            cksum = frame->control;
            cksum += frame->address;

            break;

        case IEC_C_FRAME_TYPE_CONTROL:

            cksum = frame->control;
            cksum += frame->address;
            cksum += frame->control_information;

            break;

        case IEC_C_FRAME_TYPE_LONG:

            cksum = frame->control;
            cksum += frame->address;
            cksum += frame->control_information;

            for (i = 0; i < frame->data_size; i++) {
                cksum += frame->data[i];
            }

            break;

        case IEC__FRAME_TYPE_ACK:
        default:
            cksum = 0;
    }

    return cksum;
}

//------------------------------------------------------------------------------
/// Caclulate the checksum of the M-Bus frame. The checksum algorithm is the
/// arithmetic sum of the frame content, without using carry. Which content
/// that is included in the checksum calculation depends on the frame type.
//------------------------------------------------------------------------------
int
IEC_C_frame_calc_checksum(IEC_C_frame *frame) {
    if (frame) {
        switch (frame->type) {
            case IEC__FRAME_TYPE_ACK:
            case IEC_C_FRAME_TYPE_SHORT:
            case IEC_C_FRAME_TYPE_CONTROL:
            case IEC_C_FRAME_TYPE_LONG:
                frame->checksum = calc_checksum(frame);

                break;

            default:
                return -1;
        }
    }

    return 0;
}

///
/// Calculate the values of the lengths fields in the M-Bus frame. Internal.
///
uint8_t
calc_length(const IEC_C_frame *frame) {
    assert(frame != NULL);
    switch (frame->type) {
        case IEC_C_FRAME_TYPE_CONTROL:
            return 3;
        case IEC_C_FRAME_TYPE_LONG:
            return frame->data_size + 3;
        default:
            return 0;
    }
}

//------------------------------------------------------------------------------
/// Calculate the values of the lengths fields in the M-Bus frame.
//------------------------------------------------------------------------------
int
IEC_C_frame_calc_length(IEC_C_frame *frame) {
    if (frame) {
        frame->length1 = frame->length2 = calc_length(frame);
    }

    return 0;
}


///////////////////////
void Set_IEC_state(_IEC_state st) {
    if (st == Wait_SignOn) {
        if (IEC_state != Wait_SignOn) {
            IF_SERIAL_DEBUG(
                    printf_New("IEC_RESET to wait sign in))))st=%d)))))),IEC_state=%d)))))))))))", st, IEC_state));

            HandHeldRFID_Communication = nothing;
            IEC_C_serial_set_baudrate('0');
        }
    }
    IEC_state = st;

}

//------------------------------------------------------------------------------
//
//    IEC_C_CheckModeTimeOut
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// check if time out elapsed from last time recieved data
//------------------------------------------------------------------------------
unsigned long IEC_C_CheckModeTimeOutMillis;
#define MaxSecondElapsedForNewLogIn 300
#define MaxSecondWaitdForProgMode 30//60

void IEC_C_CheckModeTimeOut(boolean reset) {

    if (reset == true) {
        IEC_C_CheckModeTimeOutMillis = millis();
        return;
    }
    if ((IEC_C_CheckModeTimeOutMillis + (long) (MaxSecondElapsedForNewLogIn * 1000)) < millis()) {
        Set_IEC_state(Wait_SignOn);
        //    IF_SERIAL_DEBUG(printf_New("IEC_RESET to wait sign in)))))))))))))))))))))", 0));

    } else {
        if ((IEC_state == Wait_SelectMode || IEC_state == InMode_DataReadOut || IEC_state == Wait_Password) &&
            ((IEC_C_CheckModeTimeOutMillis + (long) (MaxSecondWaitdForProgMode * 1000)) < millis())) {
            Set_IEC_state(Wait_SignOn);
        }
    }
     //
    //  IEC_C_CheckModeTimeOutMillis = millis();
}

bool GetSeperatedObisAddressObisValue(uint8_t *data, char *ObisAddress, char *ObisValue) {
    String stringObis((const char *) data);
    String OBIS_Value, OBIS_Address;
    OBIS_Address = stringObis.substring((unsigned int) stringObis.indexOf(IEC_C_STX_Const) + 1,
                                        (unsigned int) stringObis.indexOf('('));
    if ((unsigned int) stringObis.indexOf('(') < (unsigned int) stringObis.indexOf(')'))
        OBIS_Value = stringObis.substring((unsigned int) stringObis.indexOf('(') + 1,
                                          (unsigned int) stringObis.indexOf(')'));

    OBIS_Value.toCharArray(ObisValue, OBIS_Value.length() + 1);
    OBIS_Address.toCharArray(ObisAddress, OBIS_Address.length() + 1);

}

bool CommunicateProgramModeOBIS(char passwordType, uint8_t *data, char *RetObisVal) {

    /* String stringObis((const char *) data);
     String OBIS_Value, OBIS_Address;
     OBIS_Address = stringObis.substring((unsigned int) stringObis.indexOf(IEC_C_STX_Const) + 1,
                                         (unsigned int) stringObis.indexOf('('));
     if ((unsigned int) stringObis.indexOf('(') < (unsigned int) stringObis.indexOf(')'))
         OBIS_Value = stringObis.substring((unsigned int) stringObis.indexOf('(') + 1,
                                           (unsigned int) stringObis.indexOf(')'));

       OBIS_Value.toCharArray(msg_Value, OBIS_Value.length() + 1);
     OBIS_Address.toCharArray(msg_Add, OBIS_Address.length() + 1);*/
    char msg_Add[MAX_OBIS_ADDRESS_LENGTH];
    char msg_Value[MAX_OBIS_VALUE_LENGTH];
    GetSeperatedObisAddressObisValue(data, (char *) msg_Add, (char *) msg_Value);
    IF_SERIAL_DEBUG(printf_New("%s--->OBIS==%s,---->Add=%s\n", __PRETTY_FUNCTION__, msg_Value, msg_Add));
///  find this obis address in which password type
    ///////////////////
    Get_ObisValue((char *) msg_Add, RetObisVal);
    return true;

}

bool CommunicateProgramModeCheckPassword(int passwordType, uint8_t *data) {

    String stringObis((const char *) data);
    String StrTmp;
    if ((unsigned int) stringObis.indexOf('(') < (unsigned int) stringObis.indexOf(')'))
        StrTmp = stringObis.substring((unsigned int) stringObis.indexOf('(') + 1,
                                      (unsigned int) stringObis.indexOf(')'));

    char msg[100];
    StrTmp.toCharArray(msg, StrTmp.length() + 1);
    if (passwordType == 1) {
        IF_SERIAL_DEBUG(printf_New("%s:%s===%s \n", __PRETTY_FUNCTION__, (const char *) msg, GetPassword_1()));
        if (StrTmp.equals(GetPassword_1()))
            return true;
    }
    if (passwordType == 2)
        if (StrTmp.equals(GetPassword_1()))
            return true;
    return false;

}

//------------------------------------------------------------------------------
//
// PARSER FUNCTIONS
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// PARSE M-BUS frame data structures from binary data.
//------------------------------------------------------------------------------
int
IEC_C_parse(uint8_t *data, size_t data_size) {
    size_t i;
    char ObisValue[MAX_OBIS_VALUE_LENGTH];
    int FrameCompleteSize = -1;

    if (data && data_size > 0) {

        IF_SERIAL_DEBUG(
                printf_New("%s: Attempting to parse binary data [size = %d]\n", __PRETTY_FUNCTION__, data_size));

        for (i = 0; i < data_size && parse_debug; i++) {
            IF_SERIAL_DEBUG(printf_New("%c[%d],", data[i], data[i]));
        }
        IF_SERIAL_DEBUG(printf_New("\n", 0));
//       IF_SERIAL_DEBUG(printf_New("type = %d ", data[0]));
        IEC_C_frame *frame;

// switch (data[0]) {
/* case IEC_C_FRAME_ACK_START:

     // OK, got a valid ack frame, require no more data
     //frame->start1   = data[0];
     //frame->type = IEC_C_FRAME_TYPE_ACK;
     IF_SERIAL_DEBUG(printf_New("%s:OK, got a valid ack frame, require no more data ",__PRETTY_FUNCTION__));

     return 0;
     //return IEC_C_FRAME_BASE_SIZE_ACK - 1; // == 0*/
        if (data_size == 1 && data[0] == IEC_C_SOH_Const) return 5;
        if (data_size == 2 && data[0] == IEC_C_SOH_Const && data[1] == 'B') return 5 - 2;
        if (data_size == 3 && data[0] == IEC_C_SOH_Const && data[1] == 'B' && data[2] == '0') return 5 - 3;
        if (data_size == 3 && data[0] == IEC_C_SOH_Const && data[1] == 'B' && data[2] == '0' &&
            data[3] == IEC_C_ETX_Const)
            return 5 - 4;

        if (data_size >= CommunicateEndRequestPacketLen) {
            if (data[0] == IEC_C_SOH_Const && data[1] == 'B' && data[2] == '0' && data[3] == IEC_C_ETX_Const) {
                IF_SERIAL_DEBUG(
                        printf_New("%s:Xor End:%x ", __PRETTY_FUNCTION__, data[0] ^ data[1] ^ data[2] ^ data[3]));

                Set_IEC_state(Wait_SignOn);

                return DataPackRecieveOK;
            }
        }
#ifdef HANDHELD
        if (IEC_state == HandHeldRouter) {
            SerialIR.write(data, data_size);
            return DataPackRecieveOK;
        }
#endif
        if (IEC_state == Wait_SignOn) {
            IF_SERIAL_DEBUG(printf_New("\n%s:case ----------------->>>Wait_SignOn \n", __PRETTY_FUNCTION__));

            if (data[0] == IEC_C_IDENTIFY_BYTE_START) {
                if (data_size > 2)
                    if (data[1] != IEC_C_IDENTIFY_REQ_BYTE_SEC) {
                        IF_SERIAL_DEBUG(printf_New("%s:case IEC_C_IDENTIFY_REQUEST_SEC: ", __PRETTY_FUNCTION__));
                        return -2;
                    }

                if (data_size < IEC_C_FRAME_IDENTIFY_REQ_MIN_SIZE) {
// OK, got a valid short packet start, but we need more data
                    return IEC_C_FRAME_IDENTIFY_REQ_MIN_SIZE -
                           data_size;
                }
                if (data_size >= IEC_C_FRAME_IDENTIFY_REQ_MAX_SIZE) {
                    IF_SERIAL_DEBUG(printf_New("%s:case IEC_C_FRAME_IDENTIFY_REQ_MAX_SIZE ", __PRETTY_FUNCTION__));
                    return -2;
                }
                FrameCompleteSize = -1;
                for (i = 3; i < data_size; i++) {
                    if (data[i] == '!' && data[i + 1] == IEC_CR_CHARACTER &&
                        data[i + 2] == IEC_LF_CHARACTER) {
                        FrameCompleteSize = i + 3;
                    }
                }
                if (FrameCompleteSize == -1) {
                    IF_SERIAL_DEBUG(
                            printf_New("%s:case IEC_C_FRAME_IDENTIFY_ END Character not founf\n ",
                                       __PRETTY_FUNCTION__));
                    return -4;
                }
                IF_SERIAL_DEBUG(
                        printf_New("%s:case IEC_C_FRAME_IDENTIFY_ END OKKKKKKKKKKKKKKKKKKKKKKKK data[2]=%c\n",
                                   __PRETTY_FUNCTION__, data[2]));

                if (data[2] - 48 == 1) {
#ifndef HANDHELD
                    IEC_C_frame *replay_frame;
                    replay_frame = IEC_C_frame_new(IEC_C_FRAME_TYPE_IDENT_REPLAY);
                    IEC_C_serial_send_frame(replay_frame);

                    IEC_C_frame_free(replay_frame);
                    Set_IEC_state(Wait_SelectMode);
#endif
                    IF_SERIAL_DEBUG(printf_New("%s:OK, got a valid short packet start, but we need more data\n",
                                               __PRETTY_FUNCTION__));


                    HandHeldRFID_Communication = HandHeld;
#ifdef HANDHELD
                    SerialIR.write(data, 6);
                    Set_IEC_state(HandHeldRouter);
#endif
                }
                if (data[2] - 48 == 2) {// RFID Mode
                    Set_IEC_state(InMode_ProgramingP1);
                    HandHeldRFID_Communication = RFID;
                    IF_SERIAL_DEBUG(printf_New("%s:OK, got a valid short packet Start for RFID\n",
                                               __PRETTY_FUNCTION__));
//                   SerielTesSendFram();
                }
                return DataPackRecieveOK;
            }
        }
        if (IEC_state == Wait_SelectMode) {
            IF_SERIAL_DEBUG(printf_New("\n%s:case ----------------->>>Wait_SelectMode \n", __PRETTY_FUNCTION__));
            if (data[0] == IEC_C_ACK_Const) {
                IF_SERIAL_DEBUG(printf_New("%s:case IEC_REQUEST ", __PRETTY_FUNCTION__));
                if (data_size < IEC_C_FRAME_SELECT_MODE_SIZE) {
// OK, got a valid short packet start, but we need more data
                    return IEC_C_FRAME_SELECT_MODE_SIZE -    data_size;
                }
                FrameCompleteSize = -1;

                if (data[1] == '0' && data[4] == IEC_CR_CHARACTER &&
                    data[5] == IEC_LF_CHARACTER) {
                    FrameCompleteSize = 6;
                }

                if (FrameCompleteSize == -1) {
                    IF_SERIAL_DEBUG(printf_New("%s:case IEC_C_FRAMEPrograming_ END Character not founf \n",
                                               __PRETTY_FUNCTION__));
                    return -4;
                }
                IF_SERIAL_DEBUG(printf_New("%s:case IEC_C_FRAME_PROGRAMING_ END OKKKKKKKKKKKKKKKKKKKKKKKK \n",
                                           __PRETTY_FUNCTION__));

                IEC_C_serial_set_baudrate(data[2]);
                IEC_C_frame *replay_frame;
                if ((char) data[3] == IEC_C_DATA_READOUTMODE_SELECT) {
                    replay_frame = IEC_C_frame_new(IEC_C_FRAME_TYPE_READOUT_REPLAY);
                    Set_IEC_state(InMode_DataReadOut);
                }
                if ((char) data[3] == IEC_C_DATA_PROGRAMING_SELECT) {
                    replay_frame = IEC_C_frame_new(IEC_C_FRAME_TYPE_PROGRAMING_FIRST_REPLAY);
                    Set_IEC_state(Wait_Password);
                }
                IF_SERIAL_DEBUG(printf_New("%s:OK, char[3] == %c,(%c)\n",
                                           __PRETTY_FUNCTION__, data[3], IEC_C_DATA_READOUTMODE_SELECT));

                IEC_C_serial_send_frame(replay_frame);

                //               Iec_OBIS_Ndata_record_free(replay_frame->Iec_record);
                IEC_C_frame_free(replay_frame);
                IF_SERIAL_DEBUG(printf_New("%s:OK, got a valid short packet start, but we need more data\n",
                                           __PRETTY_FUNCTION__));
                return DataPackRecieveOK;
            }
        }
        if (IEC_state == Wait_Password) {
            IF_SERIAL_DEBUG(printf_New("\n%s:case ----------------->>Wait_Password \n", __PRETTY_FUNCTION__));
            if (data[0] == IEC_C_SOH_Const) {
                if (data_size < IEC_C_FRAME_PASSWORD_MIN_SIZE) {
// OK, got a valid short packet start, but we need more data
                    return IEC_C_FRAME_PASSWORD_MIN_SIZE -    data_size;
                }

                if (data[1] != 'P' || (data[2] != '1' && data[2] == '2') || data[3] != IEC_C_STX_Const ||
                    data[4] != '(') {
                    IF_SERIAL_DEBUG(
                            printf_New("%s:case IEC_C_FRAMEPCheck PAssword_ Error Start Character not founf ",
                                       __PRETTY_FUNCTION__));
                    return -4;
                }
                bool PacketEndReach = false;
                for (int k = 8; k < data_size && !PacketEndReach; k++) {
                    if (data[k - 2] == ')' && data[k - 1] == IEC_C_ETX_Const &&
                        data[k] == 'B') {
                        PacketEndReach = true;
                    }
                }
                if (PacketEndReach == false) {
                    if (data_size < IEC_C_FRAME_PASSWORD_MAX_SIZE)
                        return IEC_C_FRAME_PASSWORD_MAX_SIZE - data_size;
                    else {
                        return -4;
                    }
                }

                int passwordType = data[2] - 48;
                boolean CommModeOK = CommunicateProgramModeCheckPassword(passwordType, data);
                IF_SERIAL_DEBUG(
                        printf_New(
                                "%s:case IEC_C_FRAME_PROGRAMING_ END OKKKKKKKKKKKKKKKKKKKKKKKK ,Ptype=%d ,CommModeOK=%d\n",
                                __PRETTY_FUNCTION__, passwordType, CommModeOK));

                IEC_C_frame *replay_frame;
                if (CommModeOK == true) {
                    replay_frame = IEC_C_frame_new(IEC_C_ACK_Const);
                    if (passwordType == 1)
                        Set_IEC_state(InMode_ProgramingP1);
                    if (passwordType == 2)
                        Set_IEC_state(InMode_ProgramingP2);
                } else {

                    replay_frame = IEC_C_frame_new(IEC_C_NAK_Const);
                    Set_IEC_state(Wait_SignOn);
                }
                IEC_C_serial_send_frame(replay_frame);
                IEC_C_frame_free(replay_frame);
                return DataPackRecieveOK;
            }

        }
        if (IEC_state == InMode_ProgramingP1 || IEC_state == InMode_ProgramingP2) {
            IF_SERIAL_DEBUG(printf_New("\n%s:case ----------------->>InMode_ProgramingP \n", __PRETTY_FUNCTION__));
            if (data[0] == IEC_C_SOH_Const) {
                if (data_size < IEC_C_FRAME_PASSWORD_MIN_SIZE) {
// OK, got a valid short packet start, but we need more data
                    return IEC_C_FRAME_PASSWORD_MIN_SIZE -  data_size;
                }

                if ((data[1] != 'W' && data[1] != 'R') || (data[2] != '5') || data[3] != IEC_C_STX_Const) {
                    IF_SERIAL_DEBUG(
                            printf_New("%s:case IEC_C_FRAMEPCheck PAssword_ Error Start Character not founf ",
                                       __PRETTY_FUNCTION__));
                    return -4;
                }
                bool PacketEndReach = false;
                for (int k = 8; k < data_size && !PacketEndReach; k++) {
                    if (data[k - 2] == ')' && data[k - 1] == IEC_C_ETX_Const &&
                        data[k] == 'B') {
                        PacketEndReach = true;
                    }
                }
                if (PacketEndReach == false) {
                    if (data_size < IEC_C_FRAME_PASSWORD_MAX_SIZE)
                        return IEC_C_FRAME_PASSWORD_MAX_SIZE -
                               data_size;
                    else {
                        return -4;
                    }
                }
                IF_SERIAL_DEBUG(
                        printf_New("%s:case IEC_C_FRAME_ReadPassword_ END OKKKKKKKKKKKKKKKKKKKKKKKK \n",
                                   __PRETTY_FUNCTION__));

                IEC_C_frame *replay_frame;
                if (IEC_state == InMode_ProgramingP1) {
                    BccValue(data, data_size);
                    IF_SERIAL_DEBUG(printf_New(
                            "%s:case IEC_C_FRAME_PROGRAMING_ END OKKKK33333333333333333KKKKKKRead Wrie = %c\n ",
                            __PRETTY_FUNCTION__, data[1]));
                    if (data[1] == (uint8_t) 'R') {
                        if (HandHeldRFID_Communication == RFID)
                            replay_frame = IEC_C_frame_new(IEC_C_FRAME_TYPE_RFID_REPLAY);
                        else
                            replay_frame = IEC_C_frame_new(IEC_C_FRAME_TYPE_REPLAY_INPROGMOD_1, data, data_size);
                    }
                    if (data[1] == (uint8_t) 'W') {

                        IF_SERIAL_DEBUG(printf_New("%s:OK, got a valid short packet start, but we need more data\n",
                                                   __PRETTY_FUNCTION__));
                        char Obis_adress__[MAX_OBIS_ADDRESS_LENGTH];
                        GetSeperatedObisAddressObisValue(data, (char *) Obis_adress__, (char *) ObisValue);
                        if (HandHeldRFID_Communication == RFID)
                            Set_ObisValue("0-2:01,1,01,001", (char *) ObisValue);
                        else
                            Set_ObisValue(Obis_adress__, (char *) ObisValue);
                        replay_frame = IEC_C_frame_new(IEC_C_NAK_Const);
                    }
                    IEC_C_serial_send_frame(replay_frame);
                    IEC_C_frame_free(replay_frame);
                    return DataPackRecieveOK;
                }
                int CommModeProgType = data[2] - 48;
                if (CommunicateProgramModeOBIS(data[1], data, ObisValue) == true) {
                    replay_frame = IEC_C_frame_new(IEC_C_ACK_Const);
                    if (CommModeProgType == 1)
                        Set_IEC_state(InMode_ProgramingP1);
                    if (CommModeProgType == 2)
                        Set_IEC_state(InMode_ProgramingP2);
                } else {
                    replay_frame = IEC_C_frame_new(IEC_C_NAK_Const);
                    Set_IEC_state(Wait_SignOn);
                }
                IEC_C_serial_send_frame(replay_frame);
                IEC_C_frame_free(replay_frame);
                return DataPackRecieveOK;
            }

        }
        if (data[0] == -1) {
//                IF_SERIAL_DEBUG(printf_New(("Invalid M-Bus frame start.")));
            IF_SERIAL_DEBUG(printf_New("Invalid IEC_C  frame start."));

// not a valid M-Bus frame header (start byte)
            return -4;
        }
    }


    IF_SERIAL_DEBUG(printf_New("Got null pointer to frame, data or zero data_size."));

    return -1;
}

//------------------------------------------------------------------------------
/// Pack the M-bus frame into a binary string representation that can be sent
/// on the bus. The binary packet format is different for the different types
/// of M-bus frames.
//------------------------------------------------------------------------------
int
IEC_C_frame_pack(IEC_C_frame *frame, uint8_t *data, size_t data_size) {
    IF_SERIAL_DEBUG(printf_New("##3  %d:\n ", __PRETTY_FUNCTION__, data));
    size_t i, offset = 0;
    int NrecordLength = 0;
    //    IF_SERIAL_DEBUG(printf_New(("%s: Entered\n"), "IEC_C_frame_pack"));

    if (frame && data) {
        if (IEC_C_frame_calc_length(frame) == -1) {
            return -2;
        }

        //    IF_SERIAL_DEBUG(printf_New(("%s: Frame type %d\n"), "IEC_C_frame_pack",frame->type ));
        IF_SERIAL_DEBUG(printf_New("%s:No 3 \n", __PRETTY_FUNCTION__));

        switch (frame->type) {
            case IEC_C_ACK_Const:
                data = (uint8_t *)
                        realloc((uint8_t *)
                                        data, 2);
                data[offset++] = IEC_C_ACK_Const;
                return offset;
            case IEC_C_NAK_Const:
                data = (uint8_t *)
                        realloc((uint8_t *)
                                        data, 2);
                data[offset++] = IEC_C_NAK_Const;
                return offset;
            case IEC_C_FRAME_TYPE_IDENT_REPLAY:
                NrecordLength = Get_IEC_Identify_Len() - 1 + 7;
                data = (uint8_t *) realloc((uint8_t *) data, NrecordLength);
                data[offset++] = IEC_C_IDENTIFY_BYTE_START;

                data[offset++] = Get_IEC_ManufactureXXXIdentByte(0);
                data[offset++] = Get_IEC_ManufactureXXXIdentByte(1);
                data[offset++] = Get_IEC_ManufactureXXXIdentByte(2);

                data[offset++] = 'Z';
                for (int k = 0; k < Get_IEC_Identify_Len() - 1; k++)
                    data[offset++] = Get_IEC_IdentifyByte(k);
                data[offset++] = IEC_CR_CHARACTER;

                data[offset++] = IEC_LF_CHARACTER;
                return offset;

            case IEC_C_FRAME_TYPE_RFID_REPLAY:
            case IEC_C_FRAME_TYPE_REPLAY_INPROGMOD_1:
            case IEC_C_FRAME_TYPE_READOUT_REPLAY:
                Iec_OBIS_Ndata_record *NrecordN;
                NrecordN = frame->Iec_record;
                NrecordLength = 0;
                offset = 0;
                for (; NrecordN; NrecordN = NrecordN->next) {
                    NrecordLength = NrecordLength + strlen(NrecordN->msg) + 2;
                    IF_SERIAL_DEBUG(printf_New("%s:No 722========%s \n", __PRETTY_FUNCTION__, NrecordN->msg));
                }
                IF_SERIAL_DEBUG(printf_New("%s:No 712========%d \n", __PRETTY_FUNCTION__, NrecordLength));

                NrecordLength = NrecordLength + strlen(NrecordN->msg) + 9;
                data = (uint8_t *)
                        realloc((uint8_t *)
                                        data, NrecordLength);
                IF_SERIAL_DEBUG(printf_New("%s:No 714======== %d \n", __PRETTY_FUNCTION__, NrecordLength));

                data[offset++] = IEC_C_STX_Const;
                /*               data[offset++] = Get_IEC_ManufactureXXXIdentByte(0);
                               data[offset++] = Get_IEC_ManufactureXXXIdentByte(1);
                               data[offset++] = Get_IEC_ManufactureXXXIdentByte(2);
               */
                NrecordN = frame->Iec_record;
                char TempMSg[200];
                //               IF_SERIAL_DEBUG(printf_New("%s:No 15======== %d %s,%s\n", __PRETTY_FUNCTION__, strlen(TempMSg), TempMSg,
                //                                         NrecordN->msg));
                char BCC;
                BCC = (char) 0;
                for (; NrecordN; NrecordN = NrecordN->next) {
                    strcpy(TempMSg, NrecordN->msg);
                    for (int k = 0; k < strlen(TempMSg); k++) {
                        data[offset++] = TempMSg[k];
                        BCC = BCC ^ TempMSg[k];
                    }
                    //               data[offset++] = 13;
                    //               data[offset++] = 10;
                }

                data[offset] = '!';
                BCC = BCC ^ data[offset++];

                data[offset] = IEC_CR_CHARACTER;
                BCC = BCC ^ data[offset++];
                data[offset] = IEC_LF_CHARACTER;
                BCC = BCC ^ data[offset++];
                data[offset] = IEC_C_ETX_Const;
                BCC = BCC ^ data[offset++];
                data[offset] = BCC;
                IF_SERIAL_DEBUG(printf_New("frame->start1222222222 = IEC_C_FRAME_TYPE_READOUT_REPLAY;", 0));
                return offset;
            case IEC_C_FRAME_TYPE_PROGRAMING_FIRST_REPLAY:
                data = (uint8_t *) realloc((uint8_t *) data, 9);
                data[offset++] = IEC_C_SOH_Const;
                data[offset++] = 'P';
                data[offset++] = '0';
                data[offset++] = IEC_C_STX_Const;
                data[offset++] = '(';
                data[offset++] = ')';

                data[offset++] = IEC_C_ETX_Const;
                data[offset++] = 'B';
                IF_SERIAL_DEBUG(printf_New("frame->start1333333333333 = IEC_C_FRAME_TYPE_PROGRAMING_FIRST_REPLAY;", 0));
                return offset;
            default:
                return -5;
        }

    }

    return -1;
}

//------------------------------------------------------------------------------
/// Allocate and initialize a new variable data New record
//------------------------------------------------------------------------------
Iec_OBIS_Ndata_record *
Iec_OBIS_Ndata_record_new(char *Address, char *Value) {
    Iec_OBIS_Ndata_record *record;//=Iec_OBIS_Ndata_record_new();
    if ((record = (Iec_OBIS_Ndata_record *) malloc(sizeof(Iec_OBIS_Ndata_record))) == NULL) {
        IF_SERIAL_DEBUG(printf_New("%s:No 1012 \n", __PRETTY_FUNCTION__));
        return NULL;
    }

    record->next = NULL;

    //  if(record ==NULL) return NULL;
    strcpy(record->Obis_Address, Address);
    strcpy(record->Obis_value, Value);
    return record;
}

//------------------------------------------------------------------------------
/// free up memory associated with a data record and all the subsequent records
/// in its list (apply recursively)
//------------------------------------------------------------------------------
void
Iec_OBIS_Ndata_record_free(Iec_OBIS_Ndata_record *record) {
    if (record) {
        Iec_OBIS_Ndata_record *next = record->next;

        free(record);

        if (next)
            Iec_OBIS_Ndata_record_free(next);
    }
}

//------------------------------------------------------------------------------
/// Return a string containing an XML representation of the M-BUS frame.
//------------------------------------------------------------------------------
Iec_OBIS_Ndata_record *
AddNewIec_OBIS_Ndata_record_ToLinkList(Iec_OBIS_Ndata_record *record, char *Address, char *Value) {
    Iec_OBIS_Ndata_record *iter, *Prevrecord;
    if (record) {
        {
            for (iter = record; iter;) {
                Prevrecord = iter;
                iter = iter->next;
            }
            Prevrecord->next = Iec_OBIS_Ndata_record_new(Address, Value);
        }
    }
    return Prevrecord->next;
}

void GetStartEndDateFromObisValue(char *OBIS_Value, char *DateFrom, char *DateTo) {
    if (OBIS_Value == NULL || DateFrom == NULL || DateTo == NULL) return;
    if (OBIS_Value[10] == ';') {
        memcpy(DateFrom, OBIS_Value, 10);
        if (strlen(OBIS_Value) == 21) {
            memcpy(DateTo, OBIS_Value + 11, 10);
        }
    }
    if (OBIS_Value[0] == ';' && strlen(OBIS_Value) == 11) {
        memcpy(DateTo, OBIS_Value + 1, 10);
    }


}

void GetDateFromObisValue(char *value, struct tm *RetValue) {
    char msg[11];
    DateTimeCharStruct *Mdate;
    if ((Mdate = (DateTimeCharStruct *) malloc(sizeof(DateTimeCharStruct))) == NULL) {
        return;
    }
    if (!RetValue)
        return;
    memcpy(Mdate, value, 10);
    sprintf(msg, "%c%c%c%c", Mdate->year[0], Mdate->year[1], Mdate->year[2], Mdate->year[3]);
    RetValue->tm_year = strtoint(msg);
    sprintf(msg, "%c%c", Mdate->month[0], Mdate->month[1]);
    RetValue->tm_mon = strtoint(msg);
    sprintf(msg, "%c%c", Mdate->day[0], Mdate->day[1]);
    RetValue->tm_mday = strtoint(msg);

    free(Mdate);
}

void GetDateToObisValue(char *value, struct tm *RetValue) {

}


