//------------------------------------------------------------------------------
// Copyright (C) 2011, Robert Johansson, Raditex AB
// All rights reserved.
//
// rSCADA
// http://www.rSCADA.se
// info@rscada.se
//
//------------------------------------------------------------------------------
//#include <avr/pgmspace.h>
//#include <SPI.h>
//#include "MBUS_Com\printf.h"
//#include "MBUS_Com\sscan.h"
#include <TimeLib.h>
#include "MBUS_Com/mbus.h"
#include "PublicFuncVar.h"

//#include <stdint.h>
//#include <SoftwareSerial.h>
//#include <MemoryFree.h>
#include "MBUS_Com/mbus_config.h"

extern StructTotalValues TotalValues;
static int debug = 0;

#define rxPin 2
#define txPin 3


unsigned long LastLillis;
//char LoggSerial[20][100];
int LogerCount = 0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// the setup routine runs once when you press reset:
void setup_Mbus() {
    // initialize serial communication at 9600 bits per second:
    Serial.begin(115200);
    Serial1.begin(115200);
    inputString.reserve(200);
    LastLillis = millis();

    //   printf_begin();

    // IF_SERIAL_DEBUG(printf_P(PSTR("Starting scan bus \n")));

    // int i = scanvalue(rxPin,txPin);

    // IF_SERIAL_DEBUG(printf_P(PSTR("Finished scan bus \n")));
}

// the loop routine runs over and over again forever:
void loop_Mbus() {
    // read the input on analog pin 0:
    //int sensorValue = analogRead(A0);
    // print out the value you read:
    if (LastLillis + 5000 < millis()) {
        LastLillis = millis();
        // Serial.println("hi");
        Serial.print("freeMemory()=");
//        Serial.println(freeMemory());
    }
    // delay(1000);        // delay in between reads for stability
    if (stringComplete) {

        mbus_frame *replay_frame;

        replay_frame = mbus_frame_new(MBUS_FRAME_TYPE_LONG);
        if (replay_frame == NULL) {
            Serial.println("Could not get memory for mbus_frame");
            stringComplete = false;
            return;
        }
        replay_frame->control = 0x08;
        replay_frame->address = 0x03;
        replay_frame->control_information = 0x72;

        mbus_frame_data *frame_data_replay;
        frame_data_replay = mbus_frame_data_new();
        if (frame_data_replay == NULL) {
            Serial.println("Could not get memory for frame_data_replay");
            stringComplete = false;
            return;
        }
        frame_data_replay->type = MBUS_DATA_TYPE_VARIABLE;
        frame_data_replay->data_var.header.id_bcd[0] = 0x64;
        frame_data_replay->data_var.header.id_bcd[1] = 0x16;
        frame_data_replay->data_var.header.id_bcd[2] = 0x10;
        frame_data_replay->data_var.header.id_bcd[3] = 0x23;
        frame_data_replay->data_var.header.manufacturer[0] = 0xC4;
        frame_data_replay->data_var.header.manufacturer[1] = 0x18;
        frame_data_replay->data_var.header.version = 0x01;
        frame_data_replay->data_var.header.medium = 0x03;
        frame_data_replay->data_var.header.access_no = 0x00;
        frame_data_replay->data_var.header.status = 0x00;
        frame_data_replay->data_var.header.signature[0] = 0x00;
        frame_data_replay->data_var.header.signature[1] = 0x00;

        mbus_Ndata_record *mbus_data_record_replay;
        mbus_data_record_replay = mbus_Ndata_record_new();
        if (mbus_data_record_replay == NULL)
            //    if ((mbus_data_record_replay = (mbus_Ndata_record *)malloc(sizeof(mbus_Ndata_record))) == NULL)
        {
            Serial.println("Could not get memory for mbus_data_record_replay");
            stringComplete = false;
            return;
        }
        mbus_data_record_replay->drh.dib.dif = 0x04;
        mbus_data_record_replay->drh.dib.ndife = 0;
        mbus_data_record_replay->drh.vib.vif = 0x15;
        mbus_data_record_replay->drh.vib.nvife = 0;
        mbus_data_record_replay->data_len = 4;
        mbus_data_record_replay->data[0] = 0xfd;
        mbus_data_record_replay->data[1] = 0x85;
        mbus_data_record_replay->data[2] = 0x0A;
        mbus_data_record_replay->data[3] = 0x00;
        mbus_data_record_replay->next = NULL;

        mbus_Ndata_record *mbus_data_record_replay2;
        mbus_data_record_replay2 = mbus_Ndata_record_new();
        if (mbus_data_record_replay2 == NULL)
            //    if ((mbus_data_record_replay = (mbus_Ndata_record *)malloc(sizeof(mbus_Ndata_record))) == NULL)
        {
            Serial.println("Could not get memory for mbus_data_record_replay");
            stringComplete = false;
            return;
        }
        mbus_data_record_replay2->drh.dib.dif = 0x04;
        mbus_data_record_replay2->drh.dib.ndife = 0;
        mbus_data_record_replay2->drh.vib.vif = 0x15;
        mbus_data_record_replay2->drh.vib.nvife = 0;
        mbus_data_record_replay2->data_len = 4;
        mbus_data_record_replay2->data[0] = 0x12;
        mbus_data_record_replay2->data[1] = 0x34;
        mbus_data_record_replay2->data[2] = 0x56;
        mbus_data_record_replay2->data[3] = 0x78;
        mbus_data_record_replay2->next = NULL;
        mbus_data_record_replay->next = mbus_data_record_replay2;


        frame_data_replay->data_var.Nrecord = mbus_data_record_replay;

        Serial.println(mbus_frame_internal_pack(replay_frame, frame_data_replay));
        mbus_frame_calc_checksum(replay_frame);
        mbus_frame_calc_length(replay_frame);

        mbus_serial_send_frame(replay_frame);

        //   mbus_data_record_free(mbus_data_record_replay);
        free(mbus_data_record_replay);
        free(mbus_data_record_replay2);

        mbus_frame_data_free(frame_data_replay);

        mbus_frame_free(replay_frame);

        Serial.println(inputString);
        // clear the string:
        inputString = "";
        stringComplete = false;
    }
}
/*

void serialEvent() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char) Serial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '*') {
            stringComplete = true;
        }
    }

}
*/



#define ARDBUFFER 16

/*
    int l=2;
  char *j = "test";
  long k = 123456789;
  char s = 'g';
  float f = 2.3;

  printf_New("test %d %l %c %s %f", l, k, s, j, f);
*/


int MY_ModBus_Adress() {
    return 1;
}

mbus_Ndata_record *MBUS_CumulativeVolume() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x06;
    mbus_data_record_replay2->drh.dib.ndife = 0;
    mbus_data_record_replay2->drh.vib.vif = 0x14;
    mbus_data_record_replay2->drh.vib.nvife = 0;
    mbus_data_record_replay2->data_len = 6;

    /* mbus_data_record_replay2->data[0] = 0x12;
      mbus_data_record_replay2->data[1] = 0x34;
      mbus_data_record_replay2->data[2] = 0x56;
      mbus_data_record_replay2->data[3] = 0x78;
      mbus_data_record_replay2->data[4] = 0x56;
      mbus_data_record_replay2->data[5] = 0x78;*/
    longlongbytes ik;
    ik.longn = TotalValues.Total_UsedVolume;
    for (int i = 0; i < 6; i++)
        mbus_data_record_replay2->data[i] = ik.chars[i];
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_MAX_Daily_Volume_Fellow() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x16;
    mbus_data_record_replay2->drh.dib.ndife = 0;
    mbus_data_record_replay2->drh.vib.vif = 0x4E;
    mbus_data_record_replay2->drh.vib.nvife = 0;
    mbus_data_record_replay2->data_len = 6;
/*    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;*/
    longlongbytes ik;
    ik.longn = TotalValues.V_MaxFlowIn24Hour;
    for (int i = 0; i < 6; i++)
        mbus_data_record_replay2->data[i] = ik.chars[i];

    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_CumulativPumpHourWork() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x06;
    mbus_data_record_replay2->drh.dib.ndife = 0;
    mbus_data_record_replay2->drh.vib.vif = 0x26;
    mbus_data_record_replay2->drh.vib.nvife = 0;
    mbus_data_record_replay2->data_len = 6;
/*    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;*/
    longlongbytes ik;
    ik.longn = TotalValues.Total_UsedHourPump;
    for (int i = 0; i < 6; i++)
        mbus_data_record_replay2->data[i] = ik.chars[i];

    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_CuCurrentInterval() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 0x11;
    mbus_data_record_replay2->data_len = 6;
/*    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;*/
    longbytes ik;
    ik.longn = TotalValues.Duration_Volume;
    for (int i = 0; i < 4; i++)
        mbus_data_record_replay2->data[i] = ik.chars[i];
    intbytes in;
    in.intn = 100;
    for (int i = 0; i < 2; i++)
        mbus_data_record_replay2->data[i + 4] = in.chars[i];
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}


mbus_Ndata_record *MBUS_RemainingVolume() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x11;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}


mbus_Ndata_record *MBUS_Credit() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x12;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_PowerDownDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x20;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x13;
    mbus_data_record_replay2->data_len = 4;
    char CurrDate[20];
    sprintf(CurrDate, "%04d%02d%02d", year(), month(), day());
    strcpy((char *)mbus_data_record_replay2->data,CurrDate);
   /* mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;*/
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_PowerUpDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x14;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_ReplaceBatteryRequest() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x15;
    mbus_data_record_replay2->data_len = 1;
    mbus_data_record_replay2->data[0] = 1;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_ApplicationError() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x16;
    mbus_data_record_replay2->data_len = 1;
    mbus_data_record_replay2->data[0] = 1;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_FirmwareActivated() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x17;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_CreditAssignmentDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x18;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_MagneticFieldDetected() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x19;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_MetercoverremovedDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x1A;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_EventLogClearedDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x1B;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_FlowRateExceededDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x1C;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_PermittedVolumeThresholdExceededdDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x1D;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_ElectricalCurrentDisconnectedDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x1E;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_ElectricalCurrentConnectedDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x1F;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_NULL() {
    return NULL;
}

mbus_Ndata_record *MBUS_TamperedWaterFlowDetecteddDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x21;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_SuccessfulAuthenticationDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x22;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_AuthenticationFailedDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x23;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_OperationalKeyChangedDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x24;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_Secret1SecureCchangedDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x25;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_Secret2SecureCchangedDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x26;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_ClockAdjusted() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x27;
    mbus_data_record_replay2->data_len = 1;
    mbus_data_record_replay2->data[0] = 0x1;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_ClockAdjusted2() {
    return NULL;
}

mbus_Ndata_record *MBUS_SetTheStatusOfConnectDisconnectEvent() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x29;
    mbus_data_record_replay2->data_len = 1;
    mbus_data_record_replay2->data[0] = 0x1;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_ReSetTheStatusOfConnectDisconnectEvent() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x2A;
    mbus_data_record_replay2->data_len = 1;
    mbus_data_record_replay2->data[0] = 0x1;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_SetTheStartDateOfIrrigationYear() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x2B;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_SetDurationOfIntervals() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x2C;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_SetPpermittedVolumeThresholdForIntervals() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x2D;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_SetSecurityKeyDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x2E;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_MasterKeyChangedDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x2F;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *MBUS_MBusCableDisconnectedDate() {
    mbus_Ndata_record *mbus_data_record_replay2;
    mbus_data_record_replay2 = mbus_Ndata_record_new();
    if (mbus_data_record_replay2 == NULL) {
        Serial.println("Could not get memory for mbus_data_record_replay");
        return NULL;
    }

    mbus_data_record_replay2->drh.dib.dif = 0x86;
    mbus_data_record_replay2->drh.dib.ndife = 1;
    mbus_data_record_replay2->drh.dib.dife[0] = 0x10;
    mbus_data_record_replay2->drh.vib.vif = 0xFF;
    mbus_data_record_replay2->drh.vib.nvife = 1;
    mbus_data_record_replay2->drh.vib.vife[0] = 0x31;
    mbus_data_record_replay2->data_len = 6;
    mbus_data_record_replay2->data[0] = 0x12;
    mbus_data_record_replay2->data[1] = 0x34;
    mbus_data_record_replay2->data[2] = 0x56;
    mbus_data_record_replay2->data[3] = 0x78;
    mbus_data_record_replay2->data[4] = 0x56;
    mbus_data_record_replay2->data[5] = 0x78;
    mbus_data_record_replay2->next = NULL;
    return mbus_data_record_replay2;

}

mbus_Ndata_record *(*MBus_ptr_array[32])() = {
        MBUS_RemainingVolume,
        MBUS_Credit,
        MBUS_PowerDownDate,
        MBUS_PowerUpDate,
        MBUS_ReplaceBatteryRequest,
        MBUS_ApplicationError,
        MBUS_FirmwareActivated,
        MBUS_CreditAssignmentDate,
        MBUS_MagneticFieldDetected,
        MBUS_MetercoverremovedDate,
        MBUS_EventLogClearedDate,
        MBUS_FlowRateExceededDate,
        MBUS_PermittedVolumeThresholdExceededdDate,
        MBUS_ElectricalCurrentDisconnectedDate,
        MBUS_ElectricalCurrentConnectedDate,
        MBUS_NULL,
        MBUS_TamperedWaterFlowDetecteddDate,
        MBUS_SuccessfulAuthenticationDate,
        MBUS_AuthenticationFailedDate,
        MBUS_OperationalKeyChangedDate,
        MBUS_Secret1SecureCchangedDate,
        MBUS_Secret2SecureCchangedDate,
        MBUS_ClockAdjusted,
        MBUS_ClockAdjusted2,
        MBUS_SetTheStatusOfConnectDisconnectEvent,
        MBUS_ReSetTheStatusOfConnectDisconnectEvent,
        MBUS_SetTheStartDateOfIrrigationYear,
        MBUS_SetDurationOfIntervals,
        MBUS_SetPpermittedVolumeThresholdForIntervals,
        MBUS_SetSecurityKeyDate,
        MBUS_MasterKeyChangedDate,
        MBUS_MBusCableDisconnectedDate
};

int MBUS_ParametersRequest[32];

void MBUS_MakeArrayOfRequestParameters(mbus_frame *replay_frame) {

    mbus_Ndata_record *mbus_data_record_replay[32];
    mbus_Ndata_record *NRecord;

    for (int i = 0; i < 32; i++)
        MBUS_ParametersRequest[i] = 0;
    MBUS_ParametersRequest[0] = 1;
    MBUS_ParametersRequest[3] = 1;
    MBUS_ParametersRequest[9] = 1;
    MBUS_ParametersRequest[19] = 1;

    for (int i = 0; i < 32; i++) {
        if (MBUS_ParametersRequest[i]) {
            NRecord = MBus_ptr_array[i]();
            add_NrecordPackFrame(replay_frame, NRecord);
        }
    }
    mbus_Ndata_record_free(NRecord);

}

mbus_Ndata_record *MBUS_MakeHeaderOfRequestParameters() {
    mbus_Ndata_record *mbus_data_record_replay_0 = MBUS_CumulativeVolume();
    mbus_Ndata_record *mbus_data_record_replay_1 = MBUS_MAX_Daily_Volume_Fellow();
    mbus_Ndata_record *mbus_data_record_replay_2 = MBUS_CumulativPumpHourWork();

    mbus_data_record_replay_0->next = mbus_data_record_replay_1;
    mbus_data_record_replay_1->next = mbus_data_record_replay_2;

    return mbus_data_record_replay_0;
}

void Get_MBUS_MakeArrayOfRequestParameters() {
    //   mbus_Ndata_record *mbus_data_record_replay = MBUS_MakeArrayOfRequestParameters();
    //  mbus_Ndata_record_free(mbus_data_record_replay);

}
void Mbus_CheckRequesPS(char PSS[]){



    if(PSS[0] && 0x01);
        //cumulatve consumtion volume request
    if(PSS[0] && 0x02);
        //MAximumDailyFellow
    if(PSS[0] && 0x04);
        //cumulatve Pump hour work

    if(PSS[1] && 0x01);
        //cumulatve consumtion volume request
    if(PSS[1] && 0x02);
        //MAximumDailyFellow
    if(PSS[1] && 0x04);
        //cumulatve Pump hour work

    if(PSS[2] && 0x01);
    //cumulatve consumtion volume request
    if(PSS[2] && 0x02);
    //MAximumDailyFellow
    if(PSS[2] && 0x04);
    //cumulatve Pump hour work
    if(PSS[2] && 0x08);
        //Remaining Volume
    if(PSS[2] && 0x10);
        //Assign Credit
    //MBUS_Credit
    if(PSS[2] && 0x20);
        //volume of Water consumtion fraud
    if(PSS[3] && 0x01);
        //Event Power Down event
        //MBUS_PowerDownDate,
     if(PSS[3] && 0x02);
    //Event Power Up event
    //MBUS_PowerUpDate,

    if(PSS[3] && 0x04);
    //Event Replace battery
   // MBUS_ReplaceBatteryRequest,
              if(PSS[3] && 0x08);
    //Event Application error
    //MBUS_ApplicationError,

    if(PSS[3] && 0x10);
    //Event Frimware activated
  //  MBUS_FirmwareActivated,
    if(PSS[3] && 0x20);
    //Event Credit assignment
   // MBUS_CreditAssignmentDate,

    if(PSS[3] && 0x40);
    //Strong DC Magnetic Field detect
   // MBUS_MagneticFieldDetected,
                if(PSS[3] && 0x80);
    //Meter cover remove
   // MBUS_MetercoverremovedDate,
            if(PSS[4] && 0x01);
    //Event log Cleard
   // MBUS_EventLogClearedDate,
              if(PSS[4] && 0x02);
    //Flow rate exceed
   // MBUS_FlowRateExceededDate,
              if(PSS[4] && 0x04);
    //Permit volume Threshold exceed
   // MBUS_PermittedVolumeThresholdExceededdDate,
           if(PSS[4] && 0x08);
    //Electerical connect disconnected
   // MBUS_ElectricalCurrentDisconnectedDate,
            if(PSS[4] && 0x10);
    //Electerical connect Connected
   // MBUS_ElectricalCurrentConnectedDate,
    if(PSS[4] && 0x20);
    //Tempered Wter Flow Detected
    //MBUS_TamperedWaterFlowDetecteddDate,
   if(PSS[4] && 0x40);
    //Successful Authentication
    // MBUS_SuccessfulAuthenticationDate,
   if(PSS[4] && 0x80);
    //Authentication failed
   // MBUS_AuthenticationFailedDate,
   if(PSS[5] && 0x01);
    //Operational key changed
    // MBUS_OperationalKeyChangedDate,
  if(PSS[5] && 0x02);
    //Secret1 for secure algorithm changed
   // MBUS_Secret1SecureCchangedDate
   // MBUS_RemainingVolume,



 if(PSS[5] && 0x04);
    //Secret2 for secure algorithm changed
    //MBUS_Secret2SecureCchangedDate

    if(PSS[5] && 0x08);
    //Clock adjusted
    //MBUS_ClockAdjusted1
  if(PSS[5] && 0x10);
    //MasterKeychanged
    //  MBUS_MasterKeyChangedDate,
 if(PSS[5] && 0x20);
    //MBusCable Disconnected
    //  MBUS_MBusCableDisconnectedDate

}
