

#include "Loop.h"
#include "SerialEvent.h"

//#define SERIAL_DEBUG_1

#ifdef SERIAL_DEBUG_1
#define IF_SERIAL_DEBUG(x) ({x;})
#else
#define IF_SERIAL_DEBUG(x)
#endif
extern StructTotalValues TotalValues;

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
                         {"0-4:24,2,32,255", "SetTaaarefeValues"}
};

void IEC62056_21_Serial::SendReadOutDataToPC() {
    char StrOut[100], TmpStr[10];
    char *RetVal;
    if (!(RetVal = (char *) malloc(500)))
        return;

    Get_ObisValue(Obis_array[0][0], RetVal);
    sprintf(StrOut, "%c%s", (STX_Const), RetVal);
    Serial1.print(StrOut);

    for (int i = 1; i < 25; i++) {
        Get_ObisValue(Obis_array[i][0], RetVal);
        sprintf(StrOut, "%s", RetVal);
        Serial1.print(StrOut);
    }
    sprintf(StrOut, "!%c%c%c", CR_Const, LF_Const, ETX_Const_);
    Serial1.print(StrOut);

    free(RetVal);
    return;

}

void IEC62056_21_Serial::SendProgramingFirstDataToPC() {
    char StrOut[100], TmpStr[10];
    sprintf(StrOut, "%cP0%c(%s)%c,BCC", (SOH_Const), (STX_Const),
            CommunicateSetupPassword, ETX_Const_);
    Serial1.print(StrOut);
}

void IEC62056_21_Serial::SendIdentificationDataToPC() {
}

void IEC62056_21_Serial::Send_Config_IEC62056_ToPC() {
    char TmpMsg[100];
    InCommunicateModeWithPC_const = true;
    InCommunicateModeWithPC_time = millis();
    CommunicateIEC62056Level = 1;
    intbytes TempIntByte;
    TempIntByte.intn = 2000;

    sprintf(TmpMsg, "/AMS234566V03%c%c01%c%c", TempIntByte.chars[0],
            TempIntByte.chars[1], CR_Const, LF_Const);
    Serial1.print(TmpMsg);
}

void IEC62056_21_Serial::Save_Config_IEC62056() {
}

boolean IEC62056_21_Serial::InCommunicateModeWithPC() {
    if (!InCommunicateModeWithPC_const) {
        CommunicateIEC62056Level = 0;
        return false;
    }

    if (get_MilliSecondDiff(
            InCommunicateModeWithPC_time) > MaxWaitForSerialCommunicateTimeOutMillis) {
        InCommunicateModeWithPC_const = false;
        CommunicateIEC62056Level = 0;
        SendEndCommunicate();
        return false;
    }
    InCommunicateModeWithPC_time = millis();
    return true;
}

void IEC62056_21_Serial::SetCommunicateBaudeRate(int SetBaudrate) {

}

///
/// \param SetMode
void IEC62056_21_Serial::SetCommunicateMode(int SetMode) {

    if (SetMode == DataReadOutMode) { // readout
        SendReadOutDataToPC();
        CommunicateIEC62056Mode = DataReadOutMode;
        CommunicateIEC62056Level = 1;
    }
    if (SetMode == ProgramingMode) { // readout
        SendProgramingFirstDataToPC();
        CommunicateIEC62056Mode = ProgramingMode;
        CommunicateIEC62056Level = 2;
    }

    if (SetMode == IdentificationMode) { // readout
        SendIdentificationDataToPC();
        CommunicateIEC62056Mode = IdentificationMode;
        CommunicateIEC62056Level = 1;
    }

}

bool IEC62056_21_Serial::CheckCommunicateCorrectPassword(int PasswordType,
                                                         char *Password) {
    boolean  EqualPass=true;
  //  char msg[100];
 //   sprintf(msg,"Pass--> check,%s , %s , %d , %d",TotalValues.IEC_Password_1,Password,PasswordType,strcmp(TotalValues.IEC_Password_1,Password));
//    for(int i=0 ;i<strlen(Password);i++)
//        Serial.print(Password[i]);
//    Serial.print(";;");
    for(int i=0 ;i<strlen(TotalValues.IEC_Password_1);i++) {
  //      Serial.print(TotalValues.IEC_Password_1[i]);
        if(TotalValues.IEC_Password_1[i] != Password[i]) EqualPass=false;
    }
  //  Serial.println(";;");


    return EqualPass;
}

int IEC62056_21_Serial::CommunicateProgramModeCheckPassword() {
    char TempChr[100];
    int i, k = 0;
   //  IF_SERIAL_DEBUG(
  //          printf_New("In------------Checking Pass 1=%c || 2=%c,P=%c , (=%c \n",0));
    if ((inputString[2] == '1' || inputString[2] == '2')
        && inputString[1] == 'P'  && inputString[4] == '(' ) // check P1 or P2 request
    {
        for (i = 0; i < 99; i++)
            TempChr[k] = '\0';
        i = 5;
        k = 0;
        while (i < SerialRecieve && inputString[i] != ')') {
            TempChr[k] = inputString[i];
            k++;
            i++;
        }
        String stringObis(inputString);
        String StrTmp;
        if((unsigned int) stringObis.indexOf('(')<(unsigned int) stringObis.indexOf(')'))
            StrTmp = stringObis.substring((unsigned int) stringObis.indexOf('(')+1,(unsigned int) stringObis.indexOf(')'));
   //     Serial1.print("Password_read:");
   //     Serial1.println(TempChr);
        if (CheckCommunicateCorrectPassword(inputString[2] - 48, (char *) StrTmp.c_str())) {
            CommunicateIEC62056Level = 3;
            //           Serial1.println("-------------CommunicateIEC62056Level  = 3 ");
            return ACK_Const;
        } else {
            CommunicateIEC62056Level = 0;
            return NAK_Const;
        }
    }
}

void IEC62056_21_Serial::CommunicateProgramModeReadWriteRequestFindOBIS_Value(
        char TypeR_W) {
    char TempChr[100];
    char OBIS_Address[50], OBIS_Value[50];
    int i, k = 0;
//    Serial1.println( "In-----REEEEEEEEEEEEEEEEEDIIIIIIIIIIIIING---CommunicateIEC62056 ");

    if (inputString[2] == '5') // check P1 or P2 request
    {
        for (k = 0; k < 49; k++) {
            OBIS_Address[k] = '\0';
            OBIS_Value[k] = '\0';
        }
        i = 4;
        k = 0;
        while (i < SerialRecieve && inputString[i] != '(' && i < 45) {
            OBIS_Address[k] = inputString[i];
            k++;
            i++;
        }

        if (i == SerialRecieve || i == 45) // so not found '(' then error
        {
            Error("InComingDataCommunicationReadProgramigMode");
            return;
        }
        k = 0;
        i++;
        while (i < SerialRecieve && inputString[i] != ')') {
            OBIS_Value[k] = inputString[i];
            k++;
            i++;
        }
        if (i == SerialRecieve || i == 45) // so not found '(' then error
        {
            Error("InComingDataCommunicationReadProgramigMode");
            return;
        }

        strcpy(Current_OBIS_Address, OBIS_Address);
        strcpy(Current_OBIS_Value, OBIS_Value);
//        Serial1.println(OBIS_Address);
//        Serial1.println(OBIS_Value);
        CheckCommunicateProgamingModeReadWriteRequestReplay(OBIS_Address,
                                                            OBIS_Value, TypeR_W);
    }
}

void IEC62056_21_Serial::ExternSerialEvent1() {
    String inString = "";
    int i, val_ToByteNumber = 0, findnewnum = 0;
    char temp[100], jemp[10];
    //temp = "                        ";
    char inChar;
    char SaveReceiveBytesToShow[MAX_inputString_Len];
    char SaveReceiveBytesToShow2[MAX_inputString_Len];
    int i_SaveReceiveBytesToShow = 0;
    //  Debuglog(1024);
    boolean PakageComplete = false;
    unsigned long start_millis;

    start_millis = millis();
    while (!PakageComplete && ((millis() - start_millis) < 5)) {
        InRecievingSerial = true;
        ////////////////////////////////////////////////////////////////    inChar = (char)Serial1.read();
        inChar = (char) Serial1.read();
        if ((int) inChar == 0) {
            stringComplete = false;
            memset(inputString, 0, sizeof(inputString - 1));
            SerialRecieve = 0;
            errorNumber = 2;

            return;
        }
        if (((int) inChar) > 0 && ((int) inChar) != 255) {
            InRecivingDate = 5;
            start_millis = millis();
            IF_SERIAL_DEBUG(printf_New("-->%c", inChar));

            if (count_reciev < MAX_inputString_Len)
                count_reciev++;
            if (!PakageComplete) {
                inputString[SerialRecieve] = inChar;
                SaveReceiveBytesToShow[count_reciev] = inChar;

                if (SerialRecieve > MaxSerialRecieveByte)
                    SerialRecieve = 0;
                //      DelayTimeMill(500);
                SerialRecieve++;
                /*    if (SerialRecieve == 1 && inputString[0] != 'A')SerialRecieve = 0;
                 if (SerialRecieve == 2 && inputString[1] != 'A')SerialRecieve = 0;
                 if (SerialRecieve == 3 && inputString[2] != 'B')SerialRecieve = 0;
                 if (SerialRecieve == 4 && inputString[3] != 'B')SerialRecieve = 0;*/
                if (SerialRecieve > 10)
                    if (inputString[SerialRecieve - 4] == 'D'
                        && inputString[SerialRecieve - 3] == 'D'
                        && inputString[SerialRecieve - 2] == 'E'
                        && inputString[SerialRecieve - 1] == 'F')
                        PakageComplete = true;
                if (SerialRecieve > 4)
                    if ((inputString[SerialRecieve - 2] == CR_Const//#
                         && inputString[SerialRecieve - 1] == LF_Const)//$
                        || (CommunicateIEC62056Level >= 2
                            && inputString[SerialRecieve - 2] == ETX_Const_))//@
                        PakageComplete = true;
            }
        }
    }
    //  Serial1.println("//-->");
    //for (int i = 0; i < SerialRecieve; i++)Serial1.print(inputString[i]);
    /*while ( Serial1.available()) { // || mySerial1.available()) {
     start_millis = millis();

     InRecievingSerial = true;

     // get the new byte:
     if (Serial1.available())
     inChar = (char)Serial1.read();
     Serial1.print(inChar);
     return;
     //   if (mySerial1.available())
     //     inChar = (char)mySerial1.read();

     }*/
    /* sprintf(SaveReceiveBytesToShow2, "--->");
     for (int ic = 0; ic < count_reciev; ic++)
     sprintf(SaveReceiveBytesToShow2, "%s%c,%d;", SaveReceiveBytesToShow2, inputString[ic], ic);
     Serial__println(SaveReceiveBytesToShow2 );*/

    char sss;
    if (PakageComplete == false) {
        IF_SERIAL_DEBUG(
                printf_New("PAckage not compelet inputString[SerialRecieve - 2]=%c,inputString[SerialRecieve - 1]=%c",
                           inputString[SerialRecieve - 2], inputString[SerialRecieve - 1]));
        stringComplete = false;
        memset(inputString, 0, sizeof(inputString - 1));
        SerialRecieve = 0;
        errorNumber = 2;

        //    Serial1.println("PACAGGGGGGGGGGGGGGGGGGGE Not COOOOOOOOOOOOOOOOOOOOOmplete");
        return;
    }  //
    //  Serial1.println("<--//");
    // /?1!CRLF
    IF_SERIAL_DEBUG(printf_New("PAckage compelet inputString[SerialRecieve - 2]", 0));
    if (inputString[SerialRecieve - 6] == '/' && inputString[SerialRecieve - 5] == '?' &&
        inputString[SerialRecieve - 4] == '1' &&
        inputString[SerialRecieve - 3] == '!' && inputString[SerialRecieve - 2] == CR_Const &&
        inputString[SerialRecieve - 1] == LF_Const) {

        Send_Config_IEC62056_ToPC();  // wait for replay for 2 second;
        IF_SERIAL_DEBUG(printf_New("Config  IEC62056-21", 0));
        return;
    }

    if (!InCommunicateModeWithPC())
        return;
    // ACK0Z0CRLF
    IF_SERIAL_DEBUG(
            printf_New("ACK0Z0CRLF=%c,L= %d,%c,%c,%c,%c ", inputString[SerialRecieve - 3], CommunicateIEC62056Level,
                       inputString[SerialRecieve - 6], inputString[SerialRecieve - 5], inputString[SerialRecieve - 2],
                       inputString[SerialRecieve - 1]));
//%  progmode = inputString[SerialRecieve - 3] - 48
    if (CommunicateIEC62056Level == 1 && inputString[SerialRecieve - 6] == ACK_Const &&
        inputString[SerialRecieve - 5] == '0' &&
        inputString[SerialRecieve - 2] == CR_Const && inputString[SerialRecieve - 1] == LF_Const) {
        SetCommunicateBaudeRate(inputString[SerialRecieve - 4] - 48);
        SetCommunicateMode(inputString[SerialRecieve - 3] - 48);
//        CommunicateIEC62056Level = 2;
    }
    IF_SERIAL_DEBUG(
            printf_New("CheckPAss=%d=1,%d=2,%c=&,%c=P,%c=?,%c=@ \n ", CommunicateIEC62056Mode , CommunicateIEC62056Level, inputString[0],
                                                          inputString[1],inputString[3], inputString[SerialRecieve - 2] ));

    if (CommunicateIEC62056Mode == ProgramingMode && CommunicateIEC62056Level == 2 && inputString[0] == SOH_Const &&
        inputString[1] == 'P' &&
        inputString[3] == STX_Const && inputString[SerialRecieve - 2] == ETX_Const_)
        CommunicateProgramModeCheckPassword();

    if (CommunicateIEC62056Mode == ProgramingMode && CommunicateIEC62056Level == 3 && inputString[0] == SOH_Const &&
        (inputString[1] == 'R' || inputString[1] == 'W') &&
        inputString[3] == STX_Const && inputString[SerialRecieve - 2] == ETX_Const_)
        CommunicateProgramModeReadWriteRequestFindOBIS_Value(inputString[1]);


    char ssst[100];
 //   for (int ic = 0; ic < SerialRecieve; ic++) {
 //       Serial1.print(inputString[ic]);
        // ssst[ic]=inputString[ic];ssst[ic+1]='\0';
   // }
    sprintf(ssst, "==-->%s,%d,%d", ssst, CommunicateIEC62056Mode, CommunicateIEC62056Level);
    Serial1.print(ssst);
    for (int ic = 0; ic < SerialRecieve; ic++) {
        // get the new byte:

        if (SerialRecieve > 10) {
            if (inputString[0] == 'A' && inputString[1] == 'A'
                && inputString[2] == 'B' && inputString[3] == 'B') {
                sprintf(temp, "%c%c%c", inputString[4], inputString[5],
                        inputString[6]);

                IF_SERIAL_DEBUG(printf_New("%c%c%c", inputString[4], inputString[5], inputString[6]));

                if (IsNumber(temp)) {
                    inString = temp;
                    val_ToByteNumber = inString.toInt();
                    sprintf(temp, "SerialRecieve=%d ; val_ToByteNumber=%d",
                            SerialRecieve, val_ToByteNumber);

                    IF_SERIAL_DEBUG(
                            printf_New("SerialRecieve=%d ; val_ToByteNumber=%d", SerialRecieve, val_ToByteNumber));

                    if (val_ToByteNumber < MaxSerialRecieveByte) {
                        /* char tempt[100];
                         sprintf(temp , "SerialRecieve=%d ; val_ToByteNumber=%d", SerialRecieve , val_ToByteNumber);
                         Serial__println( temp );*/

                        if (val_ToByteNumber > 10) {
                            if (SerialRecieve == val_ToByteNumber) {
                                /*  sprintf(temp , "+++++%c=D,%c=D,%c=E,%c=F,%c=A,%c=A,%c=B,%c=B", inputString[val_ToByteNumber - 4] , inputString[val_ToByteNumber - 3], inputString[val_ToByteNumber - 2], inputString[val_ToByteNumber - 1] , inputString[0], inputString[1], inputString[2], inputString[3]);
                                 Serial__println( temp );*/

                                if (inputString[val_ToByteNumber - 4] == 'D'
                                    && inputString[val_ToByteNumber - 3]
                                       == 'D'
                                    && inputString[val_ToByteNumber - 2]
                                       == 'E'
                                    && inputString[val_ToByteNumber - 1]
                                       == 'F' && inputString[0] == 'A'
                                    && inputString[1] == 'A'
                                    && inputString[2] == 'B'
                                    && inputString[3] == 'B') {
                                    //         sprintf(temp, "FFF %c,%c,%c,%c,%c,%c",inputString[6],inputString[7],inputString[8], inputString[9] , inputString[10], inputString[11] );
                                    //        Serial__println(temp);
                                    stringComplete = true;
//									checkInputString();
                                    memset(inputString, 0,
                                           sizeof(inputString - 1));
                                    Debuglog(32);
                                    SerialRecieve = -1;
                                    errorNumber = 0;
                                } else {
                                    stringComplete = false;
                                    memset(inputString, 0,
                                           sizeof(inputString - 1));
                                    SerialRecieve = 0;
                                    errorNumber = 2;
                                    Debuglog(64);
                                }
                                if (SerialRecieve >= val_ToByteNumber) {
                                    stringComplete = false;
                                    memset(inputString, 0,
                                           sizeof(inputString - 1));
                                    SerialRecieve = 0;
                                    errorNumber = 3;
                                    Debuglog(128);
                                }
                            }
                        } else {
                            stringComplete = false;
                            memset(inputString, 0, sizeof(inputString - 1));
                            SerialRecieve = 0;
                            errorNumber = 4;
                            Debuglog(256);
                        }
                    } ///if (val_ToByteNumber < MaxSerialRecieveByte)
                } /// if (IsNumber(temp)) {
                else {
                    stringComplete = false;
                    memset(inputString, 0, sizeof(inputString - 1));
                    SerialRecieve = 0;
                    errorNumber = 2;
                    Debuglog(2048);
                }

            }
        }
    }
    InRecievingSerial = false;
}


void IEC62056_21_Serial::Error(const char *StrErr) {
}

void IEC62056_21_Serial::GetStartEndDateFromObisValue(char *OBIS_Value, char *DateFrom, char *DateTo) {
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

void IEC62056_21_Serial::GetDateFromObisValue(char *value, TimeStruct *RetValue) {
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

void IEC62056_21_Serial::GetDateToObisValue(char *value, TimeStruct *RetValue) {

}

void IEC62056_21_Serial::CheckCommunicateProgamingModeReadWriteRequestReplay(
        char *OBIS_Address, char *OBIS_Value, char TypeR_W) {
    IF_SERIAL_DEBUG(
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
            SendOBIS_Value("0-4:24,2,31,255", RetVal); //I-Meter serial number
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
    if (!strcmp(OBIS_Address, "0-4:24,2,33,255")) {//Send Event File
        if (TypeR_W == 'R') {
            char StrOut[100];
          struct CharMemoryAlocated *ReadEventFile;
            ReadEventFile = GetDailEventRecords("00000000", "99999999");
            if (ReadEventFile != NULL) {
                sprintf(StrOut, "%c%s(", STX_Const, OBIS_Address);
                Serial1.print(StrOut);
                for (int i = 0; i < ReadEventFile->size ; i++)
                    Serial1.write(ReadEventFile->memory[i]);
                sprintf(StrOut, ")%c%c%cP0%c%c", CR_Const, LF_Const,SOH_Const,STX_Const, ETX_Const_);
                Serial1.print(StrOut);
                if (ReadEventFile->memory)
                    free(ReadEventFile->memory);
                if (ReadEventFile)
                    free(ReadEventFile);
            }

        }
    }
    //“YYyy-MM-DD hh:mm:ss”
    if (!strcmp(OBIS_Address, "0-0:1,0,0,255"))
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

        setTime(hour_, minute_, second_, day_, month_,year_);
        printf_New("hour_=%d, minute_=%d, second_=%d, day_=%d, month_=%d,year_=%d",hour_, minute_, second_, day_, month_,year_);

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
        SendOBIS_Value("0-4:24,2,4,255", "TotalValues.Total_UsedHourPump"); //PumpHourWork
}
////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////
/*void  IEC62056_21_Serial::DoCommunicateProgamingModeWriteRequestProces(char *OBIS_Address, char *OBIS_Value) {
 if (!strcmp(OBIS_Address, "0-4:96,1,0,255")) {
 for (int i = 0; i < 10 && OBIS_Value[i] != '\0')
 {
 TotalValues.SerialKontor[i]=OBIS_Value[i];
 TotalValues.SerialKontor[i+1]='\0';
 }
 SendAck();
 }
 //“YYyy-MM-DD hh:mm:ss”
 /*  if (!strcmp(OBIS_Address, "0-0:1,0,0,255"))
 SendOBIS_Value("04:96,1,0,255", "Get_CurrentDateTime()"); //

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
 SendOBIS_Value("0-4:24,2,4,255", "TotalValues.Total_UsedHourPump");//PumpHourWork

 }*/
void IEC62056_21_Serial::SendOBIS_Value(char *OBIS_Address, char *OBIS_Value) {
    char StrOut[200], TmpStr[10];
    sprintf(StrOut, "------->>%c%s(%s)%c%c%c", STX_Const, OBIS_Address,
            OBIS_Value, CR_Const, LF_Const, ETX_Const_);
    Serial1.print(StrOut);
}

void IEC62056_21_Serial::SendAck() {
    char StrOut[20];
    sprintf(StrOut, "%c", ACK_Const);
    Serial1.print(StrOut);
}

void IEC62056_21_Serial::SendEndCommunicate() {
    char StrOut[20];
    sprintf(StrOut, "%cB0%c", SOH_Const, ETX_Const_);
    Serial1.print(StrOut);
}

/*
void IEC62056_21_Serial::checkInputString() {
	char temp[100];
	String StrTemp[15];

	OKConnectWithBlueTooth();

	//  Serial__printlnDebug("---------- > 444Check OK");
	sprintf(temp, "FFF %c,%c,%c,%c,%c,%c", inputString[8], inputString[9],
			inputString[10], inputString[11], inputString[12], inputString[13]);
#if DEBUG_Serial
	Serial__println(temp);
#endif
	if (inputString[7] == '0' && inputString[8] == '0' && inputString[9] == 'F'
			&& inputString[10] == '5' && inputString[11] == 'A'
			&& inputString[12] == '9')
		ResetTotalPerid();
	if (inputString[7] == '1' && inputString[8] == '0' && inputString[9] == 'F'
			&& inputString[10] == '5' && inputString[11] == 'A'
			&& inputString[12] == '9')
		ResetPerid();
	if (inputString[7] == '2' && inputString[8] == '0' && inputString[9] == 'F'
			&& inputString[10] == '5' && inputString[11] == 'A'
			&& inputString[12] == '9')
		IncreseCharzh(5000, "1395/02/01", "1395/09/30");
	// calibrate values
	if (inputString[8] == '2' && inputString[9] == ',') {
		//    StrTemp = "                                      ";
		String ttemp[2] = { "", "" };
		ttemp[0] = "";
		ttemp[1] = "";
		int i = 0, j = 0, k = 0;
		for (i = 10; i < SerialRecieve - 4 && j < 2; i++) {
			if (inputString[i] != ',')
				ttemp[j] += (char) inputString[i];
			else {
				k = 0;
				j++;
			}
		}
		//    sprintf(Temp," % c
		float A_row = 0, B_row = 0;
		//   if (IsNumber(ttemp[0].c_str()) && IsNumber(ttemp[0].c_str())) {
		A_row = ttemp[0].toFloat();
		B_row = ttemp[1].toFloat();
		SaveCalibrate(A_row, B_row);
		// }
	}
	//parameters
	*/
/*
	 MaxFellow = 0;// Maximum Flow that can be use more than it, error rise
	 MaxVollume = 0; // Maximum volume that can be use more than it, error rise
	 MaxPeriod = 0; Maximum volume for peridic time that can be use more than it, error rise
	 Taarefe1_Enable = false;  Taarefe1_Percent = (float) 0.0;   Taarefe1_Sum_Date_from = 0;   Taarefe1_Sum_date_to = 0;
	 Taarefe2_Enable = false;  Taarefe2_Percent = (float) 0.0;   Taarefe2_Sum_Date_from = 0;   Taarefe2_Sum_date_to = 0;
	 Taarefe3_Enable = false;  Taarefe3_Percent = (float) 0.0;   Taarefe3_Sum_Date_from = 0;   Taarefe3_Sum_date_to = 0;
	 *//*

	//  parameters value to arduino
#if DEBUG_Serial
	Serial__print("1--->");
#endif
	Serial1.println(inputString[8]);
	if (inputString[7] == '0' && inputString[8] == '8'
			&& inputString[9] == ',') {
#if DEBUG_Serial
		Serial__println("2--->");
#endif
		String Temp;
		char ttemp[22][10] = { "", "", "", "", "", "", "", "", "", "", "", "",
				"", "", "", "", "", "", "", "", "", "" };
		char ttempDateTime[6][4] = { "", "", "", "", "", "" };
		int i = 0, j = 0, checkNumberLength = -1;
		for (i = 10; i < SerialRecieve - 4 && j < 16; i++) {
			if (inputString[i] != ',') {
				if (checkNumberLength++ < 9)
					ttemp[j][checkNumberLength] += (char) inputString[i];
			} else {
				checkNumberLength = -1;
				j++;
			}

		}

#if DEBUG_Serial
		for (i = 0; i < 16; i++)
			Serial__println(ttemp[i]);
#endif
		int hour_, minute_, second_, day_, month_, year_;
		if (IsNumber(ttemp[0]) && IsNumber(ttemp[1]) && IsNumber(ttemp[2])
				&& IsNumber(ttemp[3]) && IsNumber(ttemp[4])
				&& IsNumber(ttemp[5])) {
			hour_ = atoi(ttemp[0]);
			minute_ = atoi(ttemp[1]);
			second_ = atoi(ttemp[2]);
			day_ = atoi(ttemp[3]);
			month_ = atoi(ttemp[4]);
			year_ = atoi(ttemp[5]);
			SetDateTimeRTC(hour_, minute_, second_, day_, month_, year_);
			setEvent(true, ClockAdjusted);
			setEvent(false, ClockAdjusted);

		}

		//  AABB06708,50,20.3,300,500,400,1,100,1.2,1,200,1.3,1,365,1.4,5A9DDEF
		*/
/*   int MaxFellow = 1000; // Maximum Flow that can be use more than it, error rise
		 int MaxVollume = 1000 ; // Maximum volume that can be use more than it, error rise
		 int MaxPeriod = 1000 ; // Maximum volume for peridic time that can be use more than it, error rise
		 float K_param = 1.0;
		 int Taarefe1_Enable = 1 ;
		 float Taarefe1_Percent = 1.0 ;
		 int Taarefe1_Sum_Date_from = 1 ;
		 int SumTo_1 = 100;
		 int Taarefe2_Enable = 1 ;
		 float Taarefe2_Percent = 1.0 ;
		 int Taarefe2_Sum_Date_from = 101 ;
		 int SumTo_2 = 200 ;
		 int Taarefe3_Enable = 1 ;
		 float Taarefe3_Percent = 1.0 ;
		 int Taarefe3_Sum_Date_from = 201 ;
		 int SumTo_3 = 366;
		 *//*

		int TimeSavePriode = 11;
		int MaxFellow, MaxVollume, MaxPeriod;
		float K_param;
		char tempS[150];

		{
			if (IsNumber(ttemp[6]))
				MaxFellow = atoi(ttemp[6]);
			if (IsNumber(ttemp[7]))
				MaxVollume = atoi(ttemp[7]);
			if (IsNumber(ttemp[8]))
				MaxPeriod = atoi(ttemp[8]);
			if (IsNumber(ttemp[9]))
				K_param = atof(ttemp[9]);
			//   sprintf(tempS, "====> %d,%f,%d,%d,%d\n",TimeSavePriode , m3Price , MaxFellow , MaxVollume ,  MaxPeriod);
			//Serial__println(tempS);
			//     SetParameters(  10000);// ,3000 ,200, 5.4 , 1,1.2,2, 3, 4, 5.1, 6, 7, 8, 9.1, 10, 11);
			SetParameters(MaxFellow, MaxVollume, MaxPeriod, K_param, 1, 1.0, 1,
					100, 1, 1.0, 101, 200, 1, 1.0, 201, 366);

			*/
/*     SetParameters( int MaxFellow ,int MaxVollume ,int MaxPeriod ,float K_param ,int Taarefe1_Enable = 1, float Taarefe1_Percent = 1.0 ,
			 int Taarefe1_Sum_Date_from = 1 ,int SumTo_1 = 100 ,int Taarefe2_Enable = 1 , float Taarefe2_Percent = 1.0 ,int Taarefe2_Sum_Date_from = 101
			 int SumTo_2 = 200 ,int Taarefe3_Enable = 1 ,float Taarefe3_Percent = 1.0 ,int Taarefe3_Sum_Date_from = 201 , int SumTo_3 = 366;);
			 */
/* if (IsNumber(ttemp[5]))
			 ParametersConfig.Taarefe1_Sum_Date_from = atoi( ttemp[5]) ;
			 if (IsNumber(ttemp[6]))
			 ParametersConfig.SumTo_1 = atoi( ttemp[6]) ;
			 if (IsNumber(ttemp[7]))
			 ParametersConfig.Taarefe2_Enable = atoi( ttemp[7]) ;
			 if (IsNumber(ttemp[8]))
			 ParametersConfig.Taarefe2_Percent = atof( ttemp[8]) ;
			 if (IsNumber(ttemp[9]))
			 ParametersConfig.Taarefe2_Sum_Date_from = atoi( ttemp[9]) ;
			 if (IsNumber(ttemp[10]))
			 ParametersConfig.SumTo_2 = atoi( ttemp[10]) ;
			 if (IsNumber(ttemp[11]))
			 ParametersConfig.Taarefe3_Enable = atoi( ttemp[11]) ;
			 if (IsNumber(ttemp[12]))
			 ParametersConfig.Taarefe3_Percent = atof( ttemp[12]) ;
			 if (IsNumber(ttemp[13]))
			 ParametersConfig.Taarefe3_Sum_Date_from = atoi( ttemp[13]) ;
			 if (IsNumber(ttemp[14]))
			 ParametersConfig.SumTo_3 = atoi( ttemp[14]) ;*//*

		}

		//   EEProm_Put<ParametersStruct>(Add_ParametersConfig, ParametersConfig);

		//sprintf(temp, "AABB J=%d inplength= %d,%d,%d,%d,%d,SUM,DDEE\n", j, SerialRecieve , ParametersConfig.MaxFellow, ParametersConfig.MaxVollume, ParametersConfig.MaxPeriod, ParametersConfig.Taarefe1_Enable);
		//Serial__println(temp);
		inputString[8] = '4';
	}
	if (inputString[7] == '1' && inputString[8] == '9'
			&& inputString[9] == ',') {    // request data file
		int i;

		*/
/*   for (i = 10; i < SerialRecieve - 4 && j < 16 ; i++)
		 {
		 if (inputString[i] != ',')
		 {
		 if (checkNumberLength++ < 9)
		 ttemp[j][checkNumberLength] += (char)inputString[i];
		 }
		 else {
		 checkNumberLength = -1;
		 j++;
		 }

		 }*//*

	}
	if (inputString[8] == '4' && inputString[9] == ',')   // response parameters
			{
		SendParametersToPC();
	}
	if (inputString[8] == '5' && inputString[9] == ',') { //SendResponceSerial = true;
														  //    sprintf(temp, "%f,%f", CalibrateConfig.Calibrate_a, CalibrateConfig.Calibrate_b );
#if DEBUG_Serial
		Serial__println(temp);
#endif
	}
	// if (inputString[8] == '6' && inputString[9] == '1' && inputString[10] == 'E' && inputString[11] == '3' && inputString[12] == 'B' && inputString[13] == '2')
	//   ResetTotalCharzhe();
	//  if (inputString[7] == '9' && inputString[8] == '9')
	// { //SendResponceSerial = true;
	//    sprintf(temp, "%f,%f", CalibrateConfig.Calibrate_a, CalibrateConfig.Calibrate_b );
	//   OKVonnectWithBlueTooth();
	//  }

}
*/
void IEC62056_21_Serial::ShoeLevelCommunicate() {
    IF_SERIAL_DEBUG(
            printf_New("InCommunicateModeWithPC_const=%d,CommunicateIEC62056Level=%d,CommunicateIEC62056Mode=%d \n",
                       InCommunicateModeWithPC_const, CommunicateIEC62056Level, CommunicateIEC62056Mode));
}