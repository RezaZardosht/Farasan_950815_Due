//
// Created by Zardosht on 28/02/2017.
//

#ifndef FARASAN_950815_DUE_PUBLICFUNCVAR_H_H
#define FARASAN_950815_DUE_PUBLICFUNCVAR_H_H
#define  IECuseSerial Serial1
#define  DebugSerial Serial
#define SerialIR Serial1
#define SerialIREvent serialEvent1
#define SerialMBUS Serial2
#define SerialMBUSEvent serialEvent2
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
typedef union {
    unsigned long long  longn;
    unsigned char chars[8];
} longlongbytes;
typedef union {
    unsigned long longn;
    unsigned char chars[4];
} longbytes;
typedef union {
    unsigned int intn;
    unsigned char chars[2];
} intbytes;

typedef struct {
    char SerialKontor[11];
    char SerialConsule[11];
    char CreatedDate[11];
    unsigned long Total_UsedVolume = 12;
    unsigned long Total_UsedHourPump = 12;
    //  unsigned long V_PUmpTotalHour = 0;
    unsigned long UsedHourPump_CountBySec = 0;
    unsigned long TotalDuration_Charzh = 1000;
    char LastDateUseMoreThanMaxVolume[11];
    char LastDateUseMoreThanMaxFellow[11];
    char LastDatePowerOffForUseMoreThanMaxVolume[11];
    char LastDatePowerOffForUseMoreThanMaxFellow[11];
    unsigned long CountOpenDoor = 0;
    char LastDateUseWaterWhenPowerOff[11];
    unsigned long VolumeUseWhenPowerOff;
    unsigned long CountOpenConsule = 0;
    char LastDateSeeElectroMagnetic[11];
    char LastTimeSeeElectroMagnetic[9];
    int batteryStatus;
    int InternalError;
    char NewFirmwareDate[11];
    char LastDateCharzhe[11];
    char LastUserConnectedDate[11];
    int LastUserConnectedCode;
    char LastDateKontorConfig[9];

    unsigned long Duration_Volume = 34;
    float Litre_Volume = 10;
    float TaarefeLitre_Volume = 10;
    long lastTotalBuy = 1000;
    float MaxFellowAllow = 100.0;
    long V_MaxFlowIn24Hour = 0.0;
    char V_MaxFlowIn24Hour_LogDay[7];

    unsigned long MaxVolumeAllow = 1000;
    char DateStart[12];// current duration Date start
    char DateEnd[12];  // Current duration Date End

    int  DateTaarefe[4];// number of date from wate year duratoin
    float Taarefe[4];
    unsigned long DateStartKeshavarzi;
    int DateHourSaveTime;// hour that log daily data shuld be save
    int MonthDaySaveTime;// day that log monthly data shuld be save
    float K_param = 0.54;
    char IEC_Password_1[10];
    char IEC_Password_2[10];

} StructTotalValues;

//#define DEBUG  Serial
#define GreaUnSLong  4294967295
#define MAX_Event_DEFINE 50
struct MonthlySaveFile_Struct {
    char today[8];// 13950508
    char profile;
    unsigned long Duration_Volume;
    unsigned long V_PUmpTotalHour;
    unsigned long TotalDuration_Charzh;
    struct MonthlySaveFile_Struct *Next;
};
struct DailySaveFile_Struct {
    char today[8];// 13950205
    char profile;
    unsigned long Duration_Volume;
    unsigned long V_MaxFlowIn24Hour;
    unsigned long V_PUmpTotalHour;
    unsigned long TotalDuration_Charzh;
    struct DailySaveFile_Struct *Next;
};


struct HourlySaveFile_Struct {
    char today[11];//13950205:20  1395-02-02:20
    char profile;
    unsigned long Duration_Volume;
    struct HourlySaveFile_Struct *Next;
};
struct EventsSaveFile_Struct {
    char today[15];//1395020520  13950202:200304
    int Events;
    struct EventsSaveFile_Struct *Next;
};
struct CharMemoryAlocated{
    char *memory;
    unsigned int size;
};

typedef struct  {
    boolean EventNegativePositive;// check rise Event in negative or positive mode
    boolean Value;
    boolean SaveOnFile;
    boolean ErrorShowOnScreen;
    boolean ErrorValue;

}EventsStructDefine;
void TimeStartup();

void TimeLoop();

void ResetTotalPerid();

void ResetPerid();

void SaveCalibrate(float A_row, float B_row);

void SendParametersToPC();

void Debuglog(int errnom);

void SetDateTimeRTC(int hour_, int minute_, int second_, int day_, int month_, int year_);

void IncreseCharzh(int Value, char DateStart[10], char DateEnd[10]);

void SetParameters(int MaxFellow, int MaxVollume = 2, int MaxPeriod = 1, float K_param = 1.0, int Taarefe1_Enable = 1,
                   float Taarefe1_Percent = 1.0, int Taarefe1_Sum_Date_from = 1, int SumTo_1 = 100,
                   int Taarefe2_Enable = 1, float Taarefe2_Percent = 1.0, int Taarefe2_Sum_Date_from = 101,
                   int SumTo_2 = 200, int Taarefe3_Enable = 1, float Taarefe3_Percent = 1.0,
                   int Taarefe3_Sum_Date_from = 201, int SumTo_3 = 366);

void OKConnectWithBlueTooth();

void ShowDisplayStartREadingRFID();

void ShowDisplayEndREadingRFID();


void SendToBluetooth();


void Send_ParametersValues();

void SaveDailyFileOnMemory(DailySaveFile_Struct TodaySaveFile);

void SaveMonthlyFileOnMemory(MonthlySaveFile_Struct ThisMonthsSaveFile);

char *Get_I_Meter_serial_number();
// change this to the pin connect with SIM800 reset pin
#define SIM800_RESET_PIN 7
#define GSMSleepPermit 8

// change this to the serial UART which SIM800 is attached to
#define SIM_SERIAL Serial3

// define DEBUG to one serial UART to enable debug information output
#define DEBUG Serial

const int Pulse1Pin = 2;   // the pin that the pushbutton is attached to
const int StrongDCMagneticFieldDetected_Pin = 3;
const int MeterCoverRemoved_Pin = 4;
const int Display_LED_PWM_PIN = 9;
const int BtnDisplayLight = 11;
const int ElectroMagneticPin = 12;
const int CharzheBatteryRelay = 43;    // the number of the LED pin
const int MainRelayValve = 45;      // the number of the LED pin
const int MainPowerOnRelay = 46;    // the number of the LED pin
const int SelectRelayValve_1_ = 47;      // the number of the LED pin
const int PositionSwitchCLOSE = 48;    // the number of the LED pin
const int SelectRelayValve_2_ = 49;      // the number of the LED pin
const int AIBattery = 54 + 8;    // the number of the LED pin
const int AIAmperMotor = 54 + 9;    // the number of the LED pin
const int PositionSwitchOPEN = 52;    // the number of the LED pin

//const int  Pulse2Pin = 45;   // the pin that the pushbutton is attached to
//const int  SelectorKeyPin = 47;   // the pin that the pushbutton is attached to
//const int  MainPowerPin = 49;   // the pin that the pushbutton is attached to
//const int  ValveOpenedPin = 51;   // the pin that the pushbutton is attached to
//const int  ValveClosedPin = 53;   // the pin that the pushbutton is attached to
////////////////////////////////    VALVE

const int ValveTimeOut = 12;  // second
const bool DO_Off_Relay = true;
const bool DO_On_Relay = false;
const int OPEN_VALVE = 1;
const int CLOSE_VALVE = 0;
const int ERROR_VALVE = -1;


bool get_MainRelayValVePosition();

unsigned long get_MicroSecondDiff(unsigned long PrevValue);

unsigned long get_MilliSecondDiff(unsigned long PrevValue);


/////////////////////////////  Events
const int PowerDown = 1;//error
const int PowerUp = 2;//error
const int ReplaceBattery = 3;
const int ApplicationError = 4;//error
const int FirmwareActivated = 5;
const int CreditAssignment = 6;
const int StrongDCMagneticFieldDetected = 7;//error
const int MeterCoverRemoved = 8;//error
const int EventLogCleared = 9;
const int FlowrateExceeded = 10;//error
const int PermittedVolumeThresholdEexceeded = 11;//error
const int ElectricalCurrentDisconnected = 12;//error
const int ElectricalCurrentConnected = 13;
const int TamperedWaterFlowDetected = 14;//error
const int SuccessfulAuthentication = 15;
const int AuthenticationFailed = 16;
const int OperationalKeyChanged = 17;
const int Secret1ForSecureAlgorithmHasChanged = 18;
const int Secret2ForSecureAlgorithmHasChanged = 19;
const int ClockAdjusted = 20;
const int MasterKeyChanged = 21;
const int MBusDisconnected = 22;//error
const int ErrorInFreeMemory =30;
const int ErrorInternal_SDCard =31;
const int ErrorInternal_RTC =32;
const int ErrorInternal_RFID =33;
const int ErrorUsingWaterWhenPowerOff=34;
/////////////////////////////Battery
/////////////////////////////Battery
int BatteryRemainlife();

const int BatterRemainLifeConst = 100;


///////////////////////////////  IEC62056-21
void Get_ObisValue(char *Obis, char *RetVal);
///////////////////////////

char *ReadDailyLogFile(char *FromDay, char *ToDay);

int GetDayOfMonthForSumofDays(int days);
int GetMonthOfForSumofDays(int days);
int strtoint(char a[]);

void  InitializeEvents();
//void setup_RFID_NEW(void) ;
//void loop_RFID_NEW(void) ;
void SaveEventsFile(int Event);
void SaveHourlyFile(StructTotalValues TotalValues);
void SaveDailyFile(StructTotalValues TotalValues);
void SaveMonthlyFile(StructTotalValues TotalValues);
void Delete_File(char *FileNama);


void DemoSaveGetEventFile();
unsigned int freemeMory();
void ShowErrorsOnScreen();
void SetCharzhe(int Value);
void GetStartEndDateFromObisValue(char *OBIS_Value, char *DateFrom, char *DateTo) ;

unsigned int freemeMory();
typedef struct {
   unsigned long SatartDay;
    long UsedScharzhe;
    int  DateTaarefePassed[4];// number of date from wate year duratoin
    float Taarefe[4];
    long  CharzheDore[4];
    byte  XorSum;

}_RFIDCardInform;
int ReadInformToRFidCard(_RFIDCardInform *RFIDCardInform__) ;
int WriteInformToRFidCard(_RFIDCardInform *RFIDCardInform__) ;



#endif //FARASAN_950815_DUE_PUBLICFUNCVAR_H_H
