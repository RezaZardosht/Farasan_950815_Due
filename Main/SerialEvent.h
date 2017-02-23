#include "Loop.h"
#ifndef SERIALEVENT_H

#define SERIALEVENT_H

#define DEBUG_Serial true
#define MaxSerialRecieveByte  160
#define MAX_inputString_Len 200
#define MaxWaitForSerialCommunicateTimeOutMillis  100000
#define CR_Const 13  //# chould be change to 13
#define LF_Const 10  //$ chould be change to 10
#define ACK_Const 06  //% chould be change to 06
#define NAK_Const 15  //> chould be change to 15
#define SOH_Const 1  //& chould be change to 1
#define DataReadOutMode 0

#define STX_Const 2  //? chould be change to 2
#define ETX_Const_ 3  //@ chould be change to 3

#define ProgramingMode  1
#define IdentificationMode 6





class IEC62056_21_Serial {
  private:
    char Current_OBIS_Address[50];
    char Current_OBIS_Value[50];

  public:
    int MAXVolumeDefined_ = 0;
    int count_reciev = 0;
    int InRecivingDate = 0;
    char inputString[MAX_inputString_Len];

    int SerialRecieve = 0;
    int errorNumber = 0;
    bool stringComplete = false;
    bool InRecievingSerial = false;
    bool InCommunicateModeWithPC_const = false;
    unsigned long   InCommunicateModeWithPC_time = 0;
    int CommunicateIEC62056Level = 0;
    int CommunicateIEC62056Mode = 0;
    char CommunicateSetupPassword[20] = "MY_FirstPassword";
    void SendReadOutDataToPC() ;
    void SendProgramingFirstDataToPC() ;
    void SendIdentificationDataToPC() ;
    void Send_Config_IEC62056_ToPC() ;
    void Save_Config_IEC62056() ;
    boolean InCommunicateModeWithPC() ;
    void  SetCommunicateBaudeRate(int SetBaudrate  ) ;
    void  SetCommunicateMode( int SetMode  ) ;
    bool  CheckCommunicateCorrectPassword(int PasswordType, char *Password) ;
    int  CommunicateProgramModeCheckPassword();
    void  CommunicateProgramModeReadWriteRequestFindOBIS_Value(char TypeR_W);
//    void  checkInputString();
    void  ExternSerialEvent1() ;
    void  SendToBluetooth() ;
    void  Error(const char *StrErr);
    void  CheckCommunicateProgamingModeReadWriteRequestReplay(char *OBIS_Address, char *OBIS_Value, char TypeR_W);
    void  SendOBIS_Value(char *OBIS_Address, char *OBIS_Value);
    void  SendAck() ;
    void  SendEndCommunicate();
    void  DoCommunicateProgamingModeWriteRequestProces(char *OBIS_Address, char *OBIS_Value);
    void  GetStartEndDateFromObisValue(char *OBIS_Value, char *DateFrom,char *DateTo);
    void GetDateFromObisValue(char *value, TimeStruct *RetValue);

    void GetDateToObisValue(char *value, TimeStruct *RetValue);
    void  ShoeLevelCommunicate();


    //private:
};


#endif
