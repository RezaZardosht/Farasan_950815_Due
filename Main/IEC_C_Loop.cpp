//
// Created by Zardosht on 23/02/2017.
//

#include "IEC_C_Com/IEC_C_protocol.h"
#include "Loop.h"


extern StructTotalValues TotalValues;

char *GetPassword_1() {
    return TotalValues.IEC_Password_1;
}

char *GetPassword_2() {
    return TotalValues.IEC_Password_2;
}

void SendOBIS_Value(char *OBIS_Address, char *OBIS_Value) {
    char StrOut[200], TmpStr[10];
    sprintf(StrOut, "%c%s(%s)!%c%c%c", IEC_C_STX_Const, OBIS_Address,
            OBIS_Value, IEC_CR_CHARACTER, IEC_LF_CHARACTER, IEC_C_ETX_Const);
    //   Serial1.print(StrOut);
}

void CheckCommunicateProgamingModeReadWriteRequestReplay(
        char *OBIS_Address, char *OBIS_Value, char TypeR_W) {
    /* IF_SERIAL_DEBUG(
             printf_New("Iam in CheckCommunicateProgamingModeReadWriteRequestReplay ,Add=%s,Type=%c \n", OBIS_Address,
                        TypeR_W));

     if (!strcmp(OBIS_Address, "0-4:24,0,3,255"))// date log request
         if (TypeR_W == 'R') {
             char DateFrom[20] = "", DateTo[20] = "";
             TimeStruct *DateFromTimeStruct, *DateToTimeStruct;
             if ((DateFromTimeStruct = (TimeStruct *) malloc(sizeof(TimeStruct))) == NULL) {
                 return;
             }
             if ((DateToTimeStruct = (TimeStruct *) malloc(sizeof(TimeStruct))) == NULL) {
                 return;
             }

             GetStartEndDateFromObisValue(OBIS_Value, DateFrom, DateTo);
             if (DateFrom != "")
                 GetDateFromObisValue(DateFrom, DateFromTimeStruct);
             if (DateTo != "")
                 GetDateFromObisValue(DateTo, DateToTimeStruct);

             //  GetDateToObisValue(OBIS_Value, DateToTimeStruct);


             //ReadDailyLogFile(DateFrom_, DateTo_, RetVal);
             char StrOut[100];

             sprintf(StrOut, "--> %d,%d,%d", DateFromTimeStruct->tm_year, DateFromTimeStruct->tm_mon,
                     DateFromTimeStruct->tm_mday);
             IF_SERIAL_DEBUG(printf_New("%s", StrOut));
             sprintf(StrOut, "--> %d,%d,%d", DateToTimeStruct->tm_year, DateToTimeStruct->tm_mon,
                     DateToTimeStruct->tm_mday);
             IF_SERIAL_DEBUG(printf_New("%s", StrOut));


             //        SendOBIS_Value("0-4:24,0,3,255(", RetVal); //I-Meter Log file

             //        free(RetVal);
             free(DateFromTimeStruct);
             free(DateToTimeStruct);
         }

     if (!strcmp(OBIS_Address, "0-4:96,1,0,255")) {//I-Meter serial number
         if (TypeR_W == 'R')
             SendOBIS_Value("0-4:96,1,0,255(", TotalValues.SerialKontor); //I-Meter serial number
         if (TypeR_W == 'W') {
             strcpy(TotalValues.SerialKontor, OBIS_Value); //I-Meter serial number
             SendAck();
         }
     }
     if (!strcmp(OBIS_Address, "0-4:96,1,1,255")) {//SerialConsule
         if (TypeR_W == 'R')
             SendOBIS_Value("0-4:96,1,1,255(", TotalValues.SerialConsule);
         if (TypeR_W == 'W') {
             strcpy(TotalValues.SerialConsule, OBIS_Value);
             SendAck();
         }
     }
     if (!strcmp(OBIS_Address, "0-4:96,1,2,255")) {//CreatedDate
         if (TypeR_W == 'R')
             SendOBIS_Value("0-4:96,1,2,255(", TotalValues.CreatedDate); //I-Meter serial number
         if (TypeR_W == 'W') {
             strcpy(TotalValues.CreatedDate, OBIS_Value); //I-Meter serial number
             SendAck();
         }
     }
     //taarefe
     //
     if (!strcmp(OBIS_Address, "0-4:24,2,31,255")) {//taarefe
         if (TypeR_W == 'R') {
             char RetVal[50];
             sprintf(RetVal, "%02d%02d;%02d%02d;%02d%02d",
                     GetMonthOfForSumofDays(TotalValues.DateTaarefe[0]),
                     GetDayOfMonthForSumofDays(TotalValues.DateTaarefe[0]),
                     GetMonthOfForSumofDays(TotalValues.DateTaarefe[1]),
                     GetDayOfMonthForSumofDays(TotalValues.DateTaarefe[1]),
                     GetMonthOfForSumofDays(TotalValues.DateTaarefe[2]),
                     GetDayOfMonthForSumofDays(TotalValues.DateTaarefe[2])
             );
             SendOBIS_Value("0-4:24,2,31,255", RetVal); //I-Meter serial number
         }
         if (TypeR_W == 'W') {//0302:0604:0912
             char msg[20];
             int day, mont;
             memset(msg, 0, sizeof(msg));
             memcpy(msg, OBIS_Value, 2);
             mont = strtoint(msg);
             memcpy(msg, OBIS_Value + 2, 2);
             day = strtoint(msg);
             TotalValues.DateTaarefe[0] = (mont <= 6) ? ((mont - 1) * 31) + day : 186 + (mont - 7) * 30 + day;

             memcpy(msg, OBIS_Value + 5, 2);
             mont = strtoint(msg);
             memcpy(msg, OBIS_Value + 7, 2);
             day = strtoint(msg);
             TotalValues.DateTaarefe[1] = (mont <= 6) ? ((mont - 1) * 31) + day : 186 + (mont - 7) * 30 + day;

             memcpy(msg, OBIS_Value + 10, 2);
             mont = strtoint(msg);
             memcpy(msg, OBIS_Value + 12, 2);
             day = strtoint(msg);
             TotalValues.DateTaarefe[2] = (mont <= 6) ? ((mont - 1) * 31) + day : 186 + (mont - 7) * 30 + day;

             SendAck();
         }
     }
     if (!strcmp(OBIS_Address, "0-4:50,2,01,255")) {//Delete Event file
         if (TypeR_W == 'W')
             Delete_File((char *) "EvwntLog.log");
     }
     if (!strcmp(OBIS_Address, "0-4:50,2,05,255")) {//ReadDi's
         if (TypeR_W == 'R'){
             char RetVal[100];
             sprintf(RetVal,"%d,%d,%d,%d",digitalRead(PositionSwitchOPEN),digitalRead(PositionSwitchCLOSE),digitalRead(MainPowerOnRelay),digitalRead(Pulse1Pin));
             SendOBIS_Value("0-4:24,2,31,255", RetVal); //I-Meter serial number

         }

     }
     if (!strcmp(OBIS_Address, "0-4:24,2,32,255")) {//taarefevalue
         if (TypeR_W == 'R') {
             char RetVal[50];
             char FFstr[4][10];
             for (int k = 0; k < 4; k++) {
                 for (int i = 0; i < 10; i++)
                     FFstr[k][i] = '\0';
                 Dtostrf(TotalValues.Taarefe[k], 4, 2, FFstr[k]);
             }
             sprintf(RetVal, "%s;%s;%s;%s", FFstr[0], FFstr[1],
                     FFstr[2], FFstr[3]);
             SendOBIS_Value("0-4:24,2,32,255", RetVal); //I-Meter serial number
         }
         if (TypeR_W == 'W') {
             String stringObis = String(OBIS_Value);
             String StrTmp;
             StrTmp = stringObis.substring(0, stringObis.indexOf(';'));
             TotalValues.Taarefe[0] = StrTmp.toFloat();
             stringObis = stringObis.substring(stringObis.indexOf(';') + 1);
             StrTmp = stringObis.substring(0, stringObis.indexOf(';'));
             TotalValues.Taarefe[1] = StrTmp.toFloat();
             stringObis = stringObis.substring(stringObis.indexOf(';') + 1);
             StrTmp = stringObis.substring(0, stringObis.indexOf(';'));
             TotalValues.Taarefe[2] = StrTmp.toFloat();
             StrTmp = stringObis.substring(stringObis.indexOf(';') + 1);
             TotalValues.Taarefe[3] = StrTmp.toFloat();

         }
     }
     if (!strcmp(OBIS_Address, "0-4:24,2,33,255")) {//MaxFellowAllow
         if (TypeR_W == 'R') {
             char RetVal[50];
             char F2str[50];
             memset(F2str, 0, sizeof(F2str) / F2str[0]);
             Dtostrf(TotalValues.MaxFellowAllow, 6, 2, F2str);
             sprintf(RetVal, " %s", F2str);
             SendOBIS_Value("0-4:24,2,33,255", RetVal); //MaxFellowAllow
         }
         if (TypeR_W == 'W') {
             String stringObisValue = String(OBIS_Value);
             TotalValues.MaxFellowAllow = stringObisValue.toFloat();
         }
     }
     if (!strcmp(OBIS_Address, "0-4:24,2,34,255")) {//MaxVolumeAllow
         if (TypeR_W == 'R') {
             char RetVal[50];
             sprintf(RetVal, " %lu", TotalValues.MaxFellowAllow);
             SendOBIS_Value("0-4:24,2,34,255", RetVal); //MaxFellowAllow
         }
         if (TypeR_W == 'W') {
             String stringObisValue = String(OBIS_Value);
             TotalValues.MaxVolumeAllow = (unsigned long) stringObisValue.toInt();
         }
     }
     if (!strcmp(OBIS_Address, "0-4:24,2,35,255")) {//Send Event File
         if (TypeR_W == 'R') {
             char StrOut[100];
             struct CharMemoryAlocated *ReadEventFile;
             ReadEventFile = GetDailEventRecords("00000000", "99999999");
             if (ReadEventFile != NULL) {
                 sprintf(StrOut, "%c%s(", STX_Const, OBIS_Address);
                 Serial1.print(StrOut);
                 for (int i = 0; i < ReadEventFile->size; i++)
                     Serial1.write(ReadEventFile->memory[i]);
                 sprintf(StrOut, ")%c%c%cP0%c%c", CR_Const, LF_Const, SOH_Const, STX_Const, ETX_Const_);
                 Serial1.print(StrOut);

                 delay(1000);
                 sprintf(StrOut, "%c%s(", STX_Const, OBIS_Address);
                 Serial.print(StrOut);
                 for (int i = 0; i < ReadEventFile->size; i++)
                     Serial.write(ReadEventFile->memory[i]);
                 sprintf(StrOut, ")%c%c%cP0%c%c", CR_Const, LF_Const, SOH_Const, STX_Const, ETX_Const_);
                 Serial.print(StrOut);


                 if (ReadEventFile->memory)
                     free(ReadEventFile->memory);
                 if (ReadEventFile)
                     free(ReadEventFile);
             }

         }
     }    if (!strcmp(OBIS_Address, "0-4:24,2,36,255")) {//Send Daily log  File
         if (TypeR_W == 'R') {
             char StrOut[100];
             struct CharMemoryAlocated *ReadEventFile;

             ReadEventFile = GetHourlyLogFile("00000000", "99999999");
             if (ReadEventFile != NULL) {
                 sprintf(StrOut, "%c%s(", STX_Const, OBIS_Address);
                 Serial1.print(StrOut);
                 for (int i = 0; i < ReadEventFile->size; i++)
                     Serial1.write(ReadEventFile->memory[i]);
                 sprintf(StrOut, ")%c%c%cP0%c%c", CR_Const, LF_Const, SOH_Const, STX_Const, ETX_Const_);
                 Serial1.print(StrOut);

                 delay(1000);
                 sprintf(StrOut, "%c%s(", STX_Const, OBIS_Address);
                 Serial.print(StrOut);
                 for (int i = 0; i < ReadEventFile->size; i++)
                     Serial.write(ReadEventFile->memory[i]);
                 sprintf(StrOut, ")%c%c%cP0%c%c", CR_Const, LF_Const, SOH_Const, STX_Const, ETX_Const_);
                 Serial.print(StrOut);


                 if (ReadEventFile->memory)
                     free(ReadEventFile->memory);
                 if (ReadEventFile)
                     free(ReadEventFile);
             }

         }
     }
     //“YYyy-MM-DD hh:mm:ss”
     if (!strcmp(OBIS_Address, "0-0:1,0,0,255")) {
         if (TypeR_W == 'R') {
             SendOBIS_Value("0-0:1,0,0,255", "Get_CurrentDateTime()"); //
         }
         if (TypeR_W == 'W') {
             String stringObis = String(OBIS_Value);
             String StrTmp;
             StrTmp = stringObis.substring(0, (unsigned int) stringObis.indexOf('-'));
             int year_ = (int) StrTmp.toInt();
             stringObis = stringObis.substring((unsigned int) (stringObis.indexOf('-') + 1));
             StrTmp = stringObis.substring(0, (unsigned int) stringObis.indexOf('-'));
             int month_ = (int) StrTmp.toInt();
             stringObis = stringObis.substring((unsigned int) (stringObis.indexOf('-') + 1));
             StrTmp = stringObis.substring(0, (unsigned int) stringObis.indexOf('-'));
             int day_ = (int) StrTmp.toInt();

             stringObis = stringObis.substring(stringObis.indexOf(';') + 1);
             StrTmp = stringObis.substring(0, (unsigned int) stringObis.indexOf(':'));
             int hour_ = (int) StrTmp.toInt();
             stringObis = stringObis.substring((unsigned int) (stringObis.indexOf(':') + 1));
             StrTmp = stringObis.substring(0, (unsigned int) stringObis.indexOf(':'));
             int minute_ = (int) StrTmp.toInt();
             stringObis = stringObis.substring((unsigned int) (stringObis.indexOf(':') + 1));
             StrTmp = stringObis.substring(0, (unsigned int) stringObis.indexOf(':'));
             int second_ = (int) StrTmp.toInt();

             SetDateTimeRTC(hour_, minute_, second_, day_, month_, year_);

             printf_New("hour_=%d, minute_=%d, second_=%d, day_=%d, month_=%d,year_=%d", hour_, minute_, second_, day_,
                        month_, year_);
         }

     }
     //“XXXXX.xxx*m^3”
     if (!strcmp(OBIS_Address, "0-4:24,2,1,255"))
         SendOBIS_Value("0-4:24,2,1,255", "TotalValues.Total_UsedVolume"); //
     //“XXXX.xxx*liter/min”
     if (!strcmp(OBIS_Address, "0-4:24,2,2,255"))
         SendOBIS_Value("0-4:24,2,2,255", "TotalValues.V_MaxFlowIn24Hour"); //The last daily maximum flow rate

     //“XXXXXX.xxx*m^5”
     if (!strcmp(OBIS_Address, "0-4:24,2,3,255"))
         SendOBIS_Value("0-4:24,2,3,255", "TotalValues.TotalDuration_Charzh"); //RemainCharzhe

     //“XXXXXX.xx*hours”
     if (!strcmp(OBIS_Address, "0-4:24,2,4,255"))
         SendOBIS_Value("0-4:24,2,4,255", "TotalValues.Total_UsedHourPump"); //PumpHourWork*/
}

char IEC_ManufactureXXXIdent[5] = "ABC";

byte Get_IEC_ManufactureXXXIdentByte(int byteNum) {
    int I_size = NELEMS(IEC_ManufactureXXXIdent);
    if (byteNum < 0 || byteNum >= I_size)
        return 0;
    return (byte) IEC_ManufactureXXXIdent[byteNum];
}

char IEC_ManufactureIdent[] = "AMSD3456";

int Get_IEC_Identify_Len() {
    return NELEMS(IEC_ManufactureIdent);
}

byte Get_IEC_IdentifyByte(int byteNum) {
    int I_size = NELEMS(IEC_ManufactureIdent);
    if (byteNum < 0 || byteNum >= I_size)
        return 0;
    return (byte) IEC_ManufactureIdent[byteNum];
}

void Set_ObisValue(char *Obis, char *OBIS_Value) {
    IF_SERIAL_DEBUG(printf_New("%s:No 301==>OBIS= %s %u \n", __PRETTY_FUNCTION__, Obis));
    if (!strcmp(Obis, "0-4:96,1,0,255")) {//I-Meter serial number
        strcpy(TotalValues.SerialKontor, OBIS_Value); //I-Meter serial number
    }
    if (!strcmp(Obis, "0-4:96,1,1,255")) {//SerialConsule
        strcpy(TotalValues.SerialConsule, OBIS_Value);
    }
    if (!strcmp(Obis, "0-4:96,1,2,255")) {//CreatedDate
        strcpy(TotalValues.CreatedDate, OBIS_Value); //I-Meter serial number
    }
    //taarefe
    //
    if (!strcmp(Obis, "0-4:24,2,31,255")) {//taarefe
        char msg[20];
        int day, mont;
        memset(msg, 0, sizeof(msg));
        memcpy(msg, OBIS_Value, 2);
        mont = strtoint(msg);
        memcpy(msg, OBIS_Value + 2, 2);
        day = strtoint(msg);
        TotalValues.DateTaarefe[0] = (mont <= 6) ? ((mont - 1) * 31) + day : 186 + (mont - 7) * 30 + day;

        memcpy(msg, OBIS_Value + 5, 2);
        mont = strtoint(msg);
        memcpy(msg, OBIS_Value + 7, 2);
        day = strtoint(msg);
        TotalValues.DateTaarefe[1] = (mont <= 6) ? ((mont - 1) * 31) + day : 186 + (mont - 7) * 30 + day;

        memcpy(msg, OBIS_Value + 10, 2);
        mont = strtoint(msg);
        memcpy(msg, OBIS_Value + 12, 2);
        day = strtoint(msg);
        TotalValues.DateTaarefe[2] = (mont <= 6) ? ((mont - 1) * 31) + day : 186 + (mont - 7) * 30 + day;
    }
    if (!strcmp(Obis, "0-4:50,2,01,255")) {//Delete Event file
        Delete_File((char *) "EvwntLog.log");
    }
    if (!strcmp(Obis, "0-4:50,2,06,255")) {
        SetCharzhe(atoi(OBIS_Value));
    }

    if (!strcmp(Obis, "0-4:24,2,32,255")) {//taarefevalue

        String stringObis = String(OBIS_Value);
        String StrTmp;
        StrTmp = stringObis.substring(0, stringObis.indexOf(';'));
        TotalValues.Taarefe[0] = StrTmp.toFloat();
        stringObis = stringObis.substring(stringObis.indexOf(';') + 1);
        StrTmp = stringObis.substring(0, stringObis.indexOf(';'));
        TotalValues.Taarefe[1] = StrTmp.toFloat();
        stringObis = stringObis.substring(stringObis.indexOf(';') + 1);
        StrTmp = stringObis.substring(0, stringObis.indexOf(';'));
        TotalValues.Taarefe[2] = StrTmp.toFloat();
        StrTmp = stringObis.substring(stringObis.indexOf(';') + 1);
        TotalValues.Taarefe[3] = StrTmp.toFloat();

    }
    if (!strcmp(Obis, "0-4:24,2,33,255")) {//MaxFellowAllow
        String stringObisValue = String(OBIS_Value);
        TotalValues.MaxFellowAllow = stringObisValue.toFloat();
    }
    if (!strcmp(Obis, "0-4:24,2,34,255")) {//MaxVolumeAllow
        String stringObisValue = String(OBIS_Value);
        TotalValues.MaxVolumeAllow = (unsigned long) stringObisValue.toInt();
    }
}
