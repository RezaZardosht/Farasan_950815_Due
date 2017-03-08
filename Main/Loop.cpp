//C:\Program Files (x86)\Arduino\hardware\tools\avr\lib\gcc\avr\4.8.1\include;C:\Program Files (x86)\Arduino\libraries
//set(${CMAKE_PROJECT_NAME}_BOARD uno)
//set(${CMAKE_PROJECT_NAME}_PORT COM3)

// bluetooth change baudrate
//Serial1.begin(Current BaudRate);
//Serial1.print("AT");
//  Serial1.print("AT+BAUD4");
//Serial.println("AT115200");
#include "loop.h"
#include "DuePWM_P/DuePWM.h"
#include "IEC_C_Com/IEC_C_protocol.h"
#include <DueTimer.h>
//#include "Loop.h"

#define IF_SERIAL_DEBUG_LOOP(x)

#if defined (_VARIANT_ARDUINO_DUE_X_)
// your Arduino Due code here
#include <malloc.h>
#include <DueFlashStorage.h>

DueFlashStorage dueFlashStorage;
extern char _end;
extern "C" char *sbrk(int i);
char *ramstart=(char *)0x20070000;
char *ramend=(char *)0x20088000;
unsigned int freemeMory() {
    char *heapend = sbrk(0);
    register char *stack_ptr asm ("sp");
    struct mallinfo mi = mallinfo();
    return stack_ptr - heapend + mi.fordblks;
}

unsigned int DynaimicRam() {
    struct mallinfo mi = mallinfo();
    return mi.uordblks;
}
unsigned int StaticRam(){
    return &_end - ramstart;
}
unsigned int StackRam(){

    register char *stack_ptr asm ("sp");
    return   ramend - stack_ptr;

}
#endif
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

unsigned int freemeMory() {
  extern int __heap_start, *__brkval;
  int v;
  int fr = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  Serial.print("Free ram: ");
  Serial.println(fr);

}
#endif


#define DEBUG_loop true
//#define MAXVolumeDefined  2000

void SetStutrupValues();

void InitializeNFC();

byte BackColor[3] = {192, 192, 192};
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
unsigned long FlowMicroSecDiff = 0;
unsigned long PreFlowMicroSecDiff = 0;
unsigned long LastCallMillisCountFlowInterrupt20 = 0;
float CurrentFlow, MovingAvgFlow[110];
unsigned int TestCounterFlow=0;
#define PWM_FREQ1  200
#define PWM_FREQ2  4000

//////////////////////////// TODO CGPRS_SIM800 gprs;
RTC_DS1307 rtc;

//IEC62056_21_Serial IEC6205621_Com;

DuePWM pwm(PWM_FREQ1, PWM_FREQ2);
//////////////////////////////   Fonts
UTFT myGLCD(CTE32HR, 38, 39, 40, 41);

extern uint8_t BigFont[];
extern uint8_t SmallFont[];     //SmallFn
extern uint8_t PNumFontB24[];     //SmallFn
extern uint8_t PersianFontSmall[];     // persian small font
extern uint8_t Text_1[];     //شروع دوره
extern uint8_t Text_3[];
extern uint8_t Text_4[];
extern uint8_t Text_5[];
extern uint8_t Text_6[];
extern uint8_t Text_7[];
extern uint8_t Text_8_Fellow[];
extern uint8_t Text_9_m3_h[];
extern uint8_t Text_10[];
extern uint8_t Text_11[];
extern uint8_t Text_12[];
extern uint8_t Text_13[];
extern uint8_t Text_14[];
extern uint8_t Text_15[];
extern uint8_t Text_16[];
extern uint8_t Text_17[];
extern unsigned int cat[];// bitmap

extern EventsStructDefine EventsStruct[];
int DisplayBrightValue = 170;
boolean Last_ShowTempTotalDuration_Charzh = false;
//DefaultBacklight backlight;
boolean SdError = false;
boolean ForceRelayMode = false;
////////////////////////////////////////////////////

struct StructPreCheckValueInMmory {
    char TTFName[10];
    char KodeVersion[10];
    int kk;
} PreCheckValueInMmory;

StructTotalValues TotalValues;


const char TTFName[10] = "TTFCheck";
const char KodeVersion[10] = "1.0.1";
long CurCalibrateFlow = 0;
unsigned long TimeMill = 0, NewTimeMill = 0;
int WaitForLoop = 0;

int Add_PreCheckValueIn_EEP;
int Add_TotalValues_EEP;
int Add_CalibrateConfig;

char CurrSPDate[50];
unsigned long freememoryTrace;

StructTotalValues &SumTotal(unsigned long IncLitre,
                            StructTotalValues &CurrTotalValues);

void Set_FlashAdresses() {
    Add_PreCheckValueIn_EEP = 20;
    Add_TotalValues_EEP = sizeof(StructPreCheckValueInMmory)
                          + sizeof(StructPreCheckValueInMmory) % 4 + Add_PreCheckValueIn_EEP;
    Add_CalibrateConfig = sizeof(StructTotalValues)
                          + sizeof(StructTotalValues) % 4 + Add_TotalValues_EEP;
}

template<typename T>
T &EEProm_Get(int Address, T &t) {
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    EEPROM.get(Address, t);
#endif
#if defined (_VARIANT_ARDUINO_DUE_X_)
    uint8_t *ptr = (uint8_t*) &t;
    byte* b = dueFlashStorage.readAddress(Address); // byte array which is read from flash at adress 4
    memcpy(ptr, b, sizeof(T)); // copy byte array to temporary struct
#endif
    return t;
}

template<typename T>
T &EEProm_Put(int Address, T &t) {
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    EEPROM.put(Address, t);
#endif
#if defined (_VARIANT_ARDUINO_DUE_X_)
    const uint8_t *ptr = (const uint8_t*) &t;
    byte b2[sizeof(T)]; // create byte array to store the struct
    memcpy(b2, ptr, sizeof(T)); // copy the struct to the byte array
    dueFlashStorage.write(Address, b2, sizeof(T)); // write byte array to flash
#endif
}

void Get_ObisValue(char *Obis, char *RetVal) {
    char F2str[20];
    if (!strcmp(Obis, "0-4:96,1,0,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis, TotalValues.SerialKontor);
    if (!strcmp(Obis, "0-4:96,1,1,255"))
        sprintf(RetVal, "%s(ABCDEF1234)\r\n", Obis, TotalValues.SerialConsule);
    if (!strcmp(Obis, "0-4:96,1,2,255"))
        sprintf(RetVal, "%s(13950501)\r\n", Obis, TotalValues.CreatedDate);
    if (!strcmp(Obis, "0-4:24,2,1,255"))
        sprintf(RetVal, "%s(%lu  * m3)\r\n", Obis,
                TotalValues.Total_UsedVolume);
    if (!strcmp(Obis, "0-4:24,2,4,255"))
        sprintf(RetVal, "%s(%lu * h)\r\n", Obis,
                TotalValues.Total_UsedHourPump);
    if (!strcmp(Obis, "0-4:24,2,3,255"))
        sprintf(RetVal, "%s(%lu * m3)\r\n", Obis,
                TotalValues.TotalDuration_Charzh);
    if (!strcmp(Obis, "0-4:24,2,2,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis,
                TotalValues.LastDateUseMoreThanMaxVolume);
    if (!strcmp(Obis, "0-4:24,2,15,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis,
                TotalValues.LastDateUseMoreThanMaxFellow);
    if (!strcmp(Obis, "0-4:24,2,16,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis,
                TotalValues.LastDatePowerOffForUseMoreThanMaxVolume);
    if (!strcmp(Obis, "0-4:24,2,17,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis,
                TotalValues.LastDatePowerOffForUseMoreThanMaxFellow);
    if (!strcmp(Obis, "0-4:24,2,18,255"))
        sprintf(RetVal, "%s(%lu)\r\n", Obis, TotalValues.CountOpenDoor);
    if (!strcmp(Obis, "0-4:24,2,19,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis,
                TotalValues.LastDateUseWaterWhenPowerOff);
    if (!strcmp(Obis, "0-4:24,2,20,255"))
        sprintf(RetVal, "%s(%lu)\r\n", Obis,
                TotalValues.VolumeUseWhenPowerOff);
    if (!strcmp(Obis, "0-4:24,2,21,255"))
        sprintf(RetVal, "%s(%lu)\r\n", Obis, TotalValues.CountOpenConsule);
    if (!strcmp(Obis, "0-4:24,2,22,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis,
                TotalValues.LastDateSeeElectroMagnetic);
    if (!strcmp(Obis, "0-4:24,2,23,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis,
                TotalValues.LastTimeSeeElectroMagnetic);
    if (!strcmp(Obis, "0-4:24,2,24,255"))
        sprintf(RetVal, "%s(%d)\r\n", Obis, TotalValues.batteryStatus);
    if (!strcmp(Obis, "0-4:24,2,25,255"))
        sprintf(RetVal, "%s(%d)\r\n", Obis, TotalValues.InternalError);
    if (!strcmp(Obis, "0-4:24,2,26,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis, TotalValues.NewFirmwareDate);
    if (!strcmp(Obis, "0-4:24,2,27,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis, TotalValues.LastDateCharzhe);
    if (!strcmp(Obis, "0-4:24,2,28,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis, TotalValues.LastUserConnectedDate);
    if (!strcmp(Obis, "0-4:24,2,29,255"))
        sprintf(RetVal, "%s(%d)\r\n", Obis, TotalValues.LastUserConnectedCode);
    if (!strcmp(Obis, "0-4:24,2,30,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis, TotalValues.LastDateKontorConfig);
    if (!strcmp(Obis, "0-4:24,2,31,255"))
        sprintf(RetVal, "%s(%02d%02d;%02d%02d;%02d%02d)\r\n", Obis,
                GetMonthOfForSumofDays(TotalValues.DateTaarefe[0]),
                GetDayOfMonthForSumofDays(TotalValues.DateTaarefe[0]),
                GetMonthOfForSumofDays(TotalValues.DateTaarefe[1]),
                GetDayOfMonthForSumofDays(TotalValues.DateTaarefe[1]),
                GetMonthOfForSumofDays(TotalValues.DateTaarefe[2]),
                GetDayOfMonthForSumofDays(TotalValues.DateTaarefe[2])
        );
    if (!strcmp(Obis, "0-4:24,2,32,255")) {
        memset(F2str, 0, sizeof(F2str) / F2str[0]);
        sprintf(RetVal, "%s(%s", Obis, FtoStr(TotalValues.Taarefe[0],F2str , 2));
       memset(F2str, 0, sizeof(F2str) / F2str[0]);
        sprintf(RetVal, "%s;%s",RetVal, FtoStr(TotalValues.Taarefe[1],F2str , 2));
      memset(F2str, 0, sizeof(F2str) / F2str[0]);
        sprintf(RetVal, "%s;%s",RetVal, FtoStr(TotalValues.Taarefe[2],F2str , 2));
       memset(F2str, 0, sizeof(F2str) / F2str[0]);
        sprintf(RetVal, "%s;%s)\r\n",RetVal,FtoStr(TotalValues.Taarefe[3],F2str , 2));

    }
    if (!strcmp(Obis, "0-4:24,2,33,255")) {
        memset(F2str, 0, sizeof(F2str) / F2str[0]);
        sprintf(RetVal, "%s(%u)\r\n", Obis, TotalValues.MaxFellowAllow);
    }
    if (!strcmp(Obis, "0-4:24,2,34,255"))
        sprintf(RetVal, "%s(%lu)\r\n", Obis, TotalValues.MaxVolumeAllow);
    //   if (!strcmp(Obis, "0-4:24,2,34,255"))
    //       sprintf(RetVal, " %s(%lu)\r\n", Obis, TotalValues.MaxVolumeAllow);
    if (!strcmp(Obis, "0-4:50,2,05,255")) {//ReadDi's
        sprintf(RetVal, "%s(%d,%d,%d,%d)\r\n", digitalRead(PositionSwitchOPEN), digitalRead(PositionSwitchCLOSE),
                digitalRead(MainPowerOnRelay), digitalRead(Pulse1Pin));


        if (!strcmp(Obis, "0-4:24,0,3,255"))// date log request
        {
            char DateFrom[20] = "", DateTo[20] = "";
            TimeStruct *DateFromTimeStruct, *DateToTimeStruct;
            if ((DateFromTimeStruct = (TimeStruct *) malloc(sizeof(TimeStruct))) == NULL) {
                return;
            }
            if ((DateToTimeStruct = (TimeStruct *) malloc(sizeof(TimeStruct))) == NULL) {
                return;
            }

            GetStartEndDateFromObisValue(Obis, DateFrom, DateTo);
            if (DateFrom != "")
                GetDateFromObisValue(DateFrom, DateFromTimeStruct);
            if (DateTo != "")
                GetDateFromObisValue(DateTo, DateToTimeStruct);

            //  GetDateToObisValue(OBIS_Value, DateToTimeStruct);


            //ReadDailyLogFile(DateFrom_, DateTo_, RetVal);
            char StrOut[100];

            sprintf(StrOut, "--> %d,%d,%d", DateFromTimeStruct->tm_year, DateFromTimeStruct->tm_mon,
                    DateFromTimeStruct->tm_mday);
            IF_SERIAL_DEBUG_LOOP(printf_New("%s", StrOut));
            sprintf(StrOut, "--> %d,%d,%d", DateToTimeStruct->tm_year, DateToTimeStruct->tm_mon,
                    DateToTimeStruct->tm_mday);
            IF_SERIAL_DEBUG_LOOP(printf_New("%s", StrOut));


            //        SendOBIS_Value("0-4:24,0,3,255(", RetVal); //I-Meter Log file

            //        free(RetVal);
            free(DateFromTimeStruct);
            free(DateToTimeStruct);
        }

        if (!strcmp(Obis, "0-4:24,2,35,255")) {//Send Event File

            char StrOut[100];
            struct CharMemoryAlocated *ReadEventFile;
            ReadEventFile = GetDailEventRecords("00000000", "99999999");
            if (ReadEventFile != NULL) {
                sprintf(StrOut, "%c%s(", IEC_C_STX_Const, Obis);
                SerialIR.print(StrOut);
                for (int i = 0; i < ReadEventFile->size; i++)
                    SerialIR.write(ReadEventFile->memory[i]);
                sprintf(StrOut, ")%c%c%cP0%c%c", IEC_CR_CHARACTER, IEC_LF_CHARACTER, IEC_C_SOH_Const, IEC_C_STX_Const, IEC_C_ETX_Const);
                SerialIR.print(StrOut);

                delay(1000);
                sprintf(StrOut, "%c%s(", IEC_C_STX_Const, Obis);
                Serial.print(StrOut);
                for (int i = 0; i < ReadEventFile->size; i++)
                    Serial.write(ReadEventFile->memory[i]);
                sprintf(StrOut, ")%c%c%cP0%c%c", IEC_CR_CHARACTER, IEC_LF_CHARACTER, IEC_C_SOH_Const, IEC_C_STX_Const, IEC_C_ETX_Const);
                Serial.print(StrOut);


                if (ReadEventFile->memory)
                    free(ReadEventFile->memory);
                if (ReadEventFile)
                    free(ReadEventFile);
            }


        }
        if (!strcmp(Obis, "0-4:24,2,36,255")) {//Send Daily log  File

            char StrOut[100];
            struct CharMemoryAlocated *ReadEventFile;

            ReadEventFile = GetHourlyLogFile("00000000", "99999999");
            if (ReadEventFile != NULL) {
                sprintf(StrOut, "%c%s(", IEC_C_STX_Const, Obis);
                SerialIR.print(StrOut);
                for (int i = 0; i < ReadEventFile->size; i++)
                    SerialIR.write(ReadEventFile->memory[i]);
                sprintf(StrOut, ")%c%c%cP0%c%c", IEC_CR_CHARACTER, IEC_LF_CHARACTER, IEC_C_SOH_Const, IEC_C_STX_Const, IEC_C_ETX_Const);
                SerialIR.print(StrOut);

                delay(1000);
                sprintf(StrOut, "%c%s(", IEC_C_STX_Const, Obis);
                Serial.print(StrOut);
                for (int i = 0; i < ReadEventFile->size; i++)
                    Serial.write(ReadEventFile->memory[i]);
                sprintf(StrOut, ")%c%c%cP0%c%c", IEC_CR_CHARACTER, IEC_LF_CHARACTER, IEC_C_SOH_Const, IEC_C_STX_Const, IEC_C_ETX_Const);
                Serial.print(StrOut);


                if (ReadEventFile->memory)
                    free(ReadEventFile->memory);
                if (ReadEventFile)
                    free(ReadEventFile);
            }
        }
    }
}

int FontRowPos(int row) {
    return 105 + row * 30;
}

long MaxFlowIn24Hour() {
    char CurrDate[20];
    sprintf(CurrDate, "%02d%02d%02d", year(), month(), day());

    if (!strcmp(CurrDate, TotalValues.V_MaxFlowIn24Hour_LogDay)) {
        TotalValues.V_MaxFlowIn24Hour = 0;
        strcpy(TotalValues.V_MaxFlowIn24Hour_LogDay, CurrDate);
    } else
        TotalValues.V_MaxFlowIn24Hour =
                (CurrentFlow > TotalValues.V_MaxFlowIn24Hour) ?
                CurrentFlow : TotalValues.V_MaxFlowIn24Hour;
    return TotalValues.V_MaxFlowIn24Hour;
}

unsigned int V_PompTotalSec = 0;


void SimulateFllow(void) {
    SumTotal(1, TotalValues);
}

void CountFlowInterrupt20() {
    TestCounterFlow++;
    buttonState = digitalRead(Pulse1Pin);
    if (buttonState != lastButtonState) {
        // if the state has changed, increment the counter
        if (buttonState == LOW) {

            SumTotal(1, TotalValues);
        }
        // Delay a little bit to avoid bouncing
    }
    // save the current state as the last state,
    //for next time through the loop
    lastButtonState = buttonState;
    get_MicroSecondDiff(LastCallMillisCountFlowInterrupt20);

    //CurunCalibrateFlow++;
}

/*float CalcRemaiCharzh(float CurrTaarefe, StructTotalValues &P_CurrTotalValues) {
    //	TotalValues.TotalDuration_Charzh = TotalValues.TotalDuration_Charzh - ( TotalValues.Duration_Volume * CalibrateConfig.Taarefe);
    //	return TotalValues.TotalDuration_Charzh;
}*/
void SetCharzhe(int Value) {
    TotalValues.TotalDuration_Charzh = (unsigned long) Value;
    char msg[20];
    strcpy(TotalValues.LastDateCharzhe, GetStrCurrentDay(msg));

}

void IncreseCharzh(int Value, char DateStart[10], char DateEnd[10]) {

    if (TotalValues.TotalDuration_Charzh > 0)
        TotalValues.TotalDuration_Charzh = 0;
    else
        TotalValues.TotalDuration_Charzh = TotalValues.TotalDuration_Charzh + Value;
    memcpy(TotalValues.DateStart, DateStart, 10);
    TotalValues.DateStart[10] = '\0';
    memcpy(TotalValues.DateEnd, DateEnd, 10);
    TotalValues.DateEnd[10] = '\0';
    char msg[20];
    strcpy(TotalValues.LastDateCharzhe, GetStrCurrentDay(msg));

    //   TotalValues.LastDateCharzhe
    //   char msg[100];
    //   sprintf(msg,"%lu,%s,%s",TotalValues.TotalDuration_Charzh,TotalValues.DateStart,TotalValues.DateEnd);
}

/*void SetParameters(int MaxFellow, int MaxVollume, int MaxPeriod, float K_param,
                   int Taarefe1_Enable, float Taarefe1_Percent, int Taarefe1_Sum_Date_from,
                   int SumTo_1, int Taarefe2_Enable, float Taarefe2_Percent,
                   int Taarefe2_Sum_Date_from, int SumTo_2, int Taarefe3_Enable,
                   float Taarefe3_Percent, int Taarefe3_Sum_Date_from, int SumTo_3) {
    //#if DEBUG_loop
    //  Serial__print("K_param ---- > ");
    //  Serial__println(K_param);
    //#endif
    ParametersConfig.K_param = K_param;
    SaveParameters();

}*/

bool FirsTimeLoadFlashMemory() {
    EEProm_Get<StructPreCheckValueInMmory>(Add_PreCheckValueIn_EEP,
                                           PreCheckValueInMmory);
//    if (!strcmp(KodeVersion, PreCheckValueInMmory.KodeVersion))
    //       setEvent(true, FirmwareActivated);
    IF_SERIAL_DEBUG_LOOP(printf_New("First Time loading flash = %d", (PreCheckValueInMmory.kk == 12345) ? 0 : 1));


    if (PreCheckValueInMmory.kk == 12345)
        return false;
    else
        return true;
}

void SaveFirstInitialaizeFlashMemory() {
    strncpy(PreCheckValueInMmory.TTFName, TTFName, sizeof(TTFName));
    strncpy(PreCheckValueInMmory.KodeVersion, KodeVersion, sizeof(KodeVersion));
    PreCheckValueInMmory.kk = 12345;
    strcpy(TotalValues.IEC_Password_1, "123456789");
    strcpy(TotalValues.IEC_Password_2, "123456789");
    EEProm_Put<StructPreCheckValueInMmory>(Add_PreCheckValueIn_EEP,
                                           PreCheckValueInMmory);
    EEProm_Put<StructTotalValues>(Add_TotalValues_EEP, TotalValues);

}

void LoadSavedTotalInFlashMemory() {
    EEProm_Get<StructTotalValues>(Add_TotalValues_EEP, TotalValues);

    IF_SERIAL_DEBUG_LOOP(printf_New("load flash %f \n", TotalValues.Litre_Volume));
}

void SaveTotalsInFlash() {
    EEProm_Put<StructTotalValues>(Add_TotalValues_EEP, TotalValues);
    IF_SERIAL_DEBUG_LOOP(printf_New("Save flash %f \n", TotalValues.Litre_Volume));

    return;
}

int PassNumberOfPesianDate(int yMiladi = 2016, int mMiladi = 3,
                           int dMiladi = 12) {
    //char msb_Date[100]  ;
    int ys, ms, ds;
    m2s(yMiladi, mMiladi, dMiladi, ys, ms, ds);
    //sprintf(CurrSPDate, " % 04d % 02d % 02d", ys, ms, ds);
    return ((ms < 7) ? (31 * ms) : (228 + (ms - 6) * 30)) + ds;
}

float GetcurrentTaarefe() {
    int passNumofyear;
    float CurrentTaarefe;
    passNumofyear = PassNumberOfPesianDate(year(), month(), day());

    if (passNumofyear < TotalValues.DateTaarefe[0])CurrentTaarefe = TotalValues.Taarefe[0];
    else if (passNumofyear < TotalValues.DateTaarefe[1])CurrentTaarefe = TotalValues.Taarefe[1];
    else if (passNumofyear < TotalValues.DateTaarefe[2])CurrentTaarefe = TotalValues.Taarefe[2];
    else CurrentTaarefe = TotalValues.Taarefe[3];
    return CurrentTaarefe;
}

StructTotalValues &SumTotal(unsigned long IncLitre,
                            StructTotalValues &CurrTotalValues) {

    FlowMicroSecDiff = get_MicroSecondDiff(PreFlowMicroSecDiff);  //GreaUnSLong
    PreFlowMicroSecDiff = micros();

    long DiffLitrPass = 0.0;
    StructTotalValues *P_CurrTotalValues =
            (StructTotalValues *) &CurrTotalValues;
    if (IncLitre < 0)
        IncLitre = 0;
    float F_IncLitre = IncLitre * 0.1;
    P_CurrTotalValues->Litre_Volume = P_CurrTotalValues->Litre_Volume
                                      + F_IncLitre / TotalValues.K_param;
    unsigned long int IncP_CurrTotalValues = 0, TaarefeIncP_CurrTotalValues = 0;//, TempP_Litre_Volume =            P_CurrTotalValues->Litre_Volume;
    while (P_CurrTotalValues->Litre_Volume >= 1000.0) {
        P_CurrTotalValues->Litre_Volume = P_CurrTotalValues->Litre_Volume - 1000.0;
        IncP_CurrTotalValues++;
    }

    P_CurrTotalValues->Duration_Volume = P_CurrTotalValues->Duration_Volume
                                         + IncP_CurrTotalValues;
    P_CurrTotalValues->Total_UsedVolume = P_CurrTotalValues->Total_UsedVolume
                                          + IncP_CurrTotalValues;
    if (P_CurrTotalValues->TotalDuration_Charzh < 0)
        P_CurrTotalValues->VolumeUseWhenPowerOff = P_CurrTotalValues->VolumeUseWhenPowerOff
                                                   + IncP_CurrTotalValues;
    P_CurrTotalValues->TaarefeLitre_Volume = P_CurrTotalValues->TaarefeLitre_Volume
                                             + F_IncLitre / TotalValues.K_param * GetcurrentTaarefe();
    while (P_CurrTotalValues->TaarefeLitre_Volume >= 1000.0) {
        P_CurrTotalValues->TaarefeLitre_Volume = P_CurrTotalValues->TaarefeLitre_Volume - 1000.0;
        TaarefeIncP_CurrTotalValues++;
    }
    P_CurrTotalValues->TotalDuration_Charzh =
            (unsigned long) (P_CurrTotalValues->TotalDuration_Charzh -
                             (TaarefeIncP_CurrTotalValues * GetcurrentTaarefe()));


    /* char str_CurrentFlow[10], SaveTempString[100];;
     Dtostrf(F_IncLitre, 4, 2, str_CurrentFlow);
     sprintf(SaveTempString, "@%0ld:%0ld,%ld,%s,%ld,%ld.%ld,%ld",IncP_CurrTotalValues,P_CurrTotalValues->Litre_Volume ,TempP_Litre_Volume, str_CurrentFlow,P_CurrTotalValues->TotalDuration_Charzh,P_CurrTotalValues->Duration_Volume, (long)((int)P_CurrTotalValues->Litre_Volume / 10), P_CurrTotalValues->Total_UsedVolume);
     Serial__println(SaveTempString);
     */

//    char  str_temp1[10], str_temp2[10], str_temp3[10],
//            str_temp4[10], str_temp5[10], str_temp6[10];
//  //  Dtostrf(TotalValues.K_param, 4, 2, str_temp1);
    //  Dtostrf(IncLitre / TotalValues.K_param, 4, 2, str_temp2);
    //  Dtostrf(P_CurrTotalValues->Litre_Volume, 4, 2, str_temp3);
    float tttt = (360000.0 / FlowMicroSecDiff);
    //   printf_New("Fllow->%u,%u,%u,%f,%f\n",FlowMicroSecDiff,IncLitre,IncP_CurrTotalValues,P_CurrTotalValues->Litre_Volume,TotalValues.K_param);
    //   Dtostrf(tttt, 4, 2, str_temp4);
    float tttt2 = tttt * IncLitre / TotalValues.K_param;
    //  Dtostrf(tttt2, 4, 2, str_temp5);

    float Temp_CurrentFlow = 0;
    for (int i = 0; i < 99; i++)
        MovingAvgFlow[i + 1] = MovingAvgFlow[i];
    // flow litre/minutes == change volume between 2pulse / diffrent time between 2 pulse

    MovingAvgFlow[0] = tttt2; //( IncLitre / ParametersConfig.K_param) * (60000000.0 / FlowMicroSecDiff) ;// litre/minute
    for (int i = 0; i < 100; i++)
        Temp_CurrentFlow += MovingAvgFlow[i];
    CurrentFlow = Temp_CurrentFlow / 100.0;
//    Dtostrf(CurrentFlow, 4, 2, str_temp6);
    // sprintf(tttemp, " % ld , % s , % s , % s , % ld , tttt = % s tttt2 =, % s, flow = % s", IncLitre, str_temp1, str_temp2, str_temp3, FlowMicroSecDiff, str_temp4, str_temp5, str_temp6);
    // Serial.println(tttemp);
    return CurrTotalValues;
}

/*void CheckEvents() {
    if (CurrentFlow > TotalValues.MaxFellow); // Maximum Flow that can be use more than it, error rise
    if (TotalValues.Duration_Volume >
        ParametersConfig.MaxVollume); // Maximum volume that can be use more than it, error rise
    // ParametersConfig.MaxPeriod = 1000 ; // Maximum volume for peridic time that can be use more than it, error rise

}*/

void ShowDisplayStartREadingRFID() {
}

void ShowDisplayEndREadingRFID() {
}

void LCDShowCurrDateTime() {
    PassNumberOfPesianDate();
    myGLCD.setFont(PersianFontSmall);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(BackColor[0], BackColor[1], BackColor[2]); // light gray
    myGLCD.print(CurrSPDate, 40, 15);
}

void LCDShowCurrFlow() {

    char FFstr[20];
    unsigned long TempDuration_Volume = TotalValues.Duration_Volume;
    long TempTotalDuration_Charzh = TotalValues.TotalDuration_Charzh;
    unsigned long TempTotal_UsedVolume = TotalValues.Total_UsedVolume;
    unsigned long TempLitre_Volume = TotalValues.Litre_Volume;

    IF_SERIAL_DEBUG_LOOP(printf_New("90300:%u\n", millis()));

    myGLCD.setFont(PNumFontB24);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(BackColor[0], BackColor[1], BackColor[2]); // light gray

    memcpy(FFstr, TotalValues.DateStart, 10);
    FFstr[10] = 0;
    myGLCD.print(FFstr, 200, 45);
    memcpy(FFstr, TotalValues.DateEnd, 10);
    FFstr[10] = 0;
    myGLCD.print(FFstr, 15, 45);
    IF_SERIAL_DEBUG_LOOP(printf_New("90301:%u\n", millis()));

    myGLCD.setColor(255, 255, 255);
    char TotalDuSign = (TempTotalDuration_Charzh >= 0) ? ' ' : '-';
    unsigned long ShowRemainCharzh = abs(TempTotalDuration_Charzh);
    for (int i = 0; i < 20; i++)
        FFstr[i] = '\0';
    sprintf(FFstr, "%ld", ShowRemainCharzh);
    for (int i = strlen(FFstr); i < 13; i++)
        FFstr[i] = '-';

    // Serial.println(TempTotalDuration_Charzh);
    if (TempTotalDuration_Charzh < 0) {
        myGLCD.setColor(255, 0, 0);
        myGLCD.setFont(Text_7); //(m3)
        myGLCD.print("0", 225, FontRowPos(0));
        myGLCD.setFont(Text_6); //اضافه برداشت
        myGLCD.print("0", 225 + 48, FontRowPos(0));
        myGLCD.setFont(PNumFontB24);
        myGLCD.print(FFstr, 15, FontRowPos(0));
        myGLCD.setColor(255, 255, 255);
        Last_ShowTempTotalDuration_Charzh = false;
    } else {
        if (Last_ShowTempTotalDuration_Charzh == false) {
            myGLCD.setFont(PNumFontB24);
            myGLCD.print("  ", 225 + 48, FontRowPos(0));// clear with blank
        }
        Last_ShowTempTotalDuration_Charzh = true;
        myGLCD.setFont(Text_7); //(m3)
        myGLCD.print("0", 225, FontRowPos(0));
        myGLCD.setFont(Text_3); //قابل برداشت
        myGLCD.print("0", 235 + 48, FontRowPos(0));
        myGLCD.setFont(PNumFontB24);
        myGLCD.print(FFstr, 15, 105);
    }
    IF_SERIAL_DEBUG_LOOP(printf_New("90302:%u\n", millis()));

    /////////////////////////////////////////////////////////OK
    // مصرف دوره
    for (int i = 0; i < 20; i++)
        FFstr[i] = '\0';
    if ((int) TempLitre_Volume  < 10)
        sprintf(FFstr, "%ld.00%ld", TempDuration_Volume,
                (long) ((int) TempLitre_Volume ));
    else if ((int) TempLitre_Volume  < 100)
        sprintf(FFstr, "%ld.0%ld", TempDuration_Volume,
                (long) ((int) TempLitre_Volume));
    else
        sprintf(FFstr, "%ld.%ld", TempDuration_Volume,
                (long) ((int) TempLitre_Volume));

    for (int i = strlen(FFstr); i < 13; i++)
        FFstr[i] = '-';
    myGLCD.print(FFstr, 15, FontRowPos(1));
    IF_SERIAL_DEBUG_LOOP(printf_New("90303:%u\n", millis()));

    // Serial.println(TempDuration_Volume);
    // مصرف کل
    for (int i = 0; i < 20; i++)
        FFstr[i] = '\0';
    sprintf(FFstr, "%ld", TempTotal_UsedVolume);
    for (int i = strlen(FFstr); i < 13; i++)
        FFstr[i] = '-';
    myGLCD.print(FFstr, 15, FontRowPos(2));
    IF_SERIAL_DEBUG_LOOP(printf_New("90304:%u\n", millis()));

    // جریان
    myGLCD.setFont(Text_9_m3_h);  //  (m3/m)
    myGLCD.print("0", 335, FontRowPos(3));
    myGLCD.setFont(Text_8_Fellow);  //  جریان
    myGLCD.print("0", 225 + 190, FontRowPos(3));
    myGLCD.setFont(PNumFontB24);
    for (int i = 0; i < 20; i++)
        FFstr[i] = 0;
    FtoStr(CurrentFlow, FFstr, 2);
    for (int i = strlen(FFstr); i < 7; i++)
        FFstr[i] = '-';
    myGLCD.print(FFstr, 225, FontRowPos(3));
    IF_SERIAL_DEBUG_LOOP(printf_New("90305:%u\n", millis()));

    //  بیشینه  جریان
    char F2str[20];
    memset(F2str, 0, 20);
    myGLCD.setFont(Text_10);  // بیشینه
    myGLCD.print("0", 135, FontRowPos(3));
    myGLCD.setFont(PNumFontB24);
    long MaxFlowIn24Hour_V = MaxFlowIn24Hour();
    FtoStr(TotalValues.V_MaxFlowIn24Hour, F2str, 2);
    FFstr[strlen(F2str)] = 0;
    sprintf(F2str,"%u",TotalValues.V_MaxFlowIn24Hour);
    myGLCD.print(F2str, 15, FontRowPos(3));
    IF_SERIAL_DEBUG_LOOP(printf_New("90306:%u\n", millis()));

    memset(F2str, 0, 20);
    //  ساعات کارکرد پمپ
    myGLCD.setFont(Text_11);
    myGLCD.print("0", 225, FontRowPos(4));
    myGLCD.setFont(PNumFontB24);
    sprintf(F2str, "%d", TotalValues.Total_UsedHourPump);
    FFstr[strlen(F2str)] = 0;
    myGLCD.print(F2str, 15, FontRowPos(4));
    IF_SERIAL_DEBUG_LOOP(printf_New("90307:%u\n", millis()));

    ///////////////////////////////////////////////////

    // don't touch
    //myGLCD.setFont(Text_17);
    //myGLCD.print("0", 375, 25);

    /*   if (TiemeOutBlueToothConnection + 5000 > millis()) // we have connection with bluetooth
       {
           myGLCD.setFont(SmallFont);
           myGLCD.print("BL", 380, 15);
       } else {
           myGLCD.setFont(SmallFont);
           myGLCD.print("  ", 380, 15);

       }*/
    //   for (int i = 0; i < 20; i++)
    //       FFstr[i] = 0;
    //   Dtostrf(TotalValues.K_param, 4, 2, FFstr);

    //FtoStr(ParametersConfig.K_param , FFstr, 2) ;
    //  for (int i = strlen(FFstr); i < 13; i++)
    //      FFstr[i] = '-';
    //  myGLCD.print(FFstr, 410, 15);

//    buttonState = digitalRead(Pulse1Pin);
//    int i = (buttonState == true) ? 1 : 0;

}

// نمایش آنتن موبایل
void ShowIcon_Anten(boolean Show) {
    myGLCD.setFont(Text_12);
    myGLCD.print("0", 225, 5);
}

boolean V_ShowIconAlarm = false;

// نمایش آلارم مصرف
void ShowIconAlarm(boolean Show) {
    if (V_ShowIconAlarm)
        myGLCD.setColor(255, 255, 255);
    else
        myGLCD.setColor(BackColor[0], BackColor[1], BackColor[2]); // light gray

    myGLCD.setFont(Text_13);
    myGLCD.print("0", 255, 5);
    V_ShowIconAlarm = !V_ShowIconAlarm;
}

void ShowIconRelayClose(boolean Show);

// رله باز
void ShowIconRelayOpen(boolean Show) {
    if (Show) {
        ShowIconRelayClose(false);
        myGLCD.setColor(255, 255, 255);
    } else
        myGLCD.setColor(BackColor[0], BackColor[1], BackColor[2]); // light gray
    myGLCD.setFont(Text_14);
    myGLCD.print("0", 285, 5);
}

//  // رله بسته
void ShowIconRelayClose(boolean Show) {
    if (Show) {
        ShowIconRelayOpen(false);
        myGLCD.setColor(255, 255, 255);
    } else
        myGLCD.setColor(BackColor[0], BackColor[1], BackColor[2]); // light gray
    myGLCD.setFont(Text_15);
    myGLCD.print("0", 285, 5);
}

///////////////////////////////////oooooooooooooooooooooooook
// باطری تمام

void ShowIconBattery(boolean Show) {
    myGLCD.setFont(Text_16);
    myGLCD.print("0", 355, 5);
}

void ShowData() {
    IF_SERIAL_DEBUG_LOOP(printf_New("90010:%u\n", millis()));
    LCDShowCurrDateTime();
    IF_SERIAL_DEBUG_LOOP(printf_New("90011:%u\n", millis()));
    LCDShowCurrFlow();
    IF_SERIAL_DEBUG_LOOP(printf_New("90012:%u\n", millis()));
    ShowIconAlarm(false);
    IF_SERIAL_DEBUG_LOOP(printf_New("90013:%u\n", millis()));
    printf_New("O=%d,C=%d", get_ValvePosition() == OPEN_VALVE, get_ValvePosition() == CLOSE_VALVE);
    if (get_ValvePosition() != OPEN_VALVE && get_ValvePosition() != CLOSE_VALVE) {
        ShowIconRelayOpen(false);
        ShowIconRelayClose(false);

    } else if (get_ValvePosition() == OPEN_VALVE && get_ValvePosition() == CLOSE_VALVE) {}
    else {
        if (get_ValvePosition() == OPEN_VALVE)
            ShowIconRelayOpen(true);
        if (get_ValvePosition() == CLOSE_VALVE)
            ShowIconRelayClose(true);
    }
    IF_SERIAL_DEBUG_LOOP(printf_New("90014:%u\n", millis()));

}
//////////////////[[[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]]]

///////////////////////call by events to show on screen
void Show_EventsOnScreen(char *msg) {
    myGLCD.setBackColor(BackColor[0], BackColor[1], BackColor[2]); // light gray
    myGLCD.setColor(255, 255, 255);
    myGLCD.setFont(SmallFont);
    myGLCD.print(msg, 420, 10);


}

void FirstGetDateTime() {
    //char CurrDateN[40];
    DateTime_RTC now_ = rtc.now();
    //  now = rtc.now();
    int ys, ms, ds;
    m2s(now_.year(), now_.month(), now_.day(), ys, ms, ds);
    // sprintf(CurrSPDate, "%d/%02d/%02d  %02d:%02d:%02d", ys, ms, ds, now_.hour(), now_.minute(), now_.second());
    setTime(now_.hour(), now_.minute(), now_.second(), now_.day(), now_.month(),
            now_.year());

    sprintf(CurrSPDate, "%d/%02d/%02d  %02d:%02d:%02d", ys, ms, ds, hour(),
            minute(), second());
    IF_SERIAL_DEBUG_LOOP(Serial__println(CurrSPDate));

}

void GetDateTime() {
    int ys, ms, ds;
    m2s(year(), month(), day(), ys, ms, ds);

    sprintf(CurrSPDate, "%d/%02d/%02d  %02d:%02d:%02d", ys, ms, ds, hour(),
            minute(), second());
     IF_SERIAL_DEBUG(printf_New(CurrSPDate, 0));
}

void SetDateTimeRTC(int hour_, int minute_, int second_, int day_, int month_,
                    int year_) {
    setTime(hour_, minute_, second_, day_, month_, year_);
    DateTime_RTC dt = now();
    rtc.adjust(dt);

}

// todo to do
bool get_MainPower() {

    bool MainPower = !digitalRead(MainPowerOnRelay);
    if (MainPower == false) {
        setEvent(PowerDown, true);
        setEvent(PowerUp, false);
    } else {
        setEvent(PowerDown, false);
        setEvent(PowerUp, true);
    }
}

int get_BateryCharzhe() {
    char msg[20];
    static int Prev_BattPower = 0;
    int ColP;
    int BattPower = analogRead(AIBattery);
    if (BattPower == Prev_BattPower)
        return BattPower;
    if (BattPower > 800)
        BattPower = 1000;
    Prev_BattPower = BattPower;
    if (BatteryRemainlife() < BatterRemainLifeConst || BatteryCharzheError())
        setEvent(ReplaceBattery, true);

    /* myGLCD.setColor(255, 0, 0);
     myGLCD.setBackColor(255, 0, 0); // light gray
     myGLCD.fillRect(354, 10, 378, 35);
     myGLCD.fillRect(359, 7, 374, 10);
     // myGLCD.fillScr(192, 192, 192);
     myGLCD.setColor(255, 255, 255);
     // myGLCD.setBackColor(192, 192, 192); // light gray
     sprintf(msg, "%d", (int)( BattPower / 10.0));
     Serial__println(msg);
     myGLCD.setFont(SmallFont);
     if (BattPower < 100)ColP = 364;
     if (BattPower < 1000)ColP = 360;
     if (BattPower == 1000)ColP = 354;
     myGLCD.print(msg, ColP, 12);

     sprintf(msg, "\%%");
     Serial__println(msg);
     myGLCD.setFont(SmallFont);
     myGLCD.print(msg, 365, 22);
     */
    return BattPower;
}

void set_BatteryCharzheRelay(bool value) {
    digitalWrite(CharzheBatteryRelay, value);
    //   printf_New("RelayBatteryValue = %d\n",value);
}

bool FirstCheckMainPower;

void SendSMS(String Value) {
}

const int MinimumForBatteryCharzhe = 700;
const int MaximumForBatteryCharzhe = 1000;
bool SimulatrPermitPumpOn=false;
bool PermitPumpOn() {
    return  SimulatrPermitPumpOn;
    return (get_MainPower() == true && TotalValues.TotalDuration_Charzh > 1.0) ?
           true : false;
}

void resetValvePower() {
}

void ResetTotalPerid() {
    Serial__println(
            "=========================Reset Total DurateData=======================");
}

void ResetPerid() {
    Serial__println(
            "-------------------Reset Duration-------------------------");
}

void Debuglog(int errnom) {
}

boolean Val_PositionSwitchOPEN = false;
boolean Val_PositionSwitchCLOSE = true;

boolean Demo_PositionSwitchOPEN() { return Val_PositionSwitchOPEN; };

boolean Demo_PositionSwitchCLOSE() { return Val_PositionSwitchCLOSE; };

void OpenPermitRelay() {
    int i = get_ValvePosition();
    char ttemp[50];
    // sprintf(ttemp, "ValvePosition =%d", i);
    // myGLCD.print(ttemp, 40, 195);

    char OpenCloseit, msg[10];
    if (get_ValvePosition() == OPEN_VALVE)
        OpenCloseit = 'O';
    if (get_ValvePosition() == CLOSE_VALVE)
        OpenCloseit = 'C';
    if (get_ValvePosition() == ERROR_VALVE)
        OpenCloseit = 'E';

    if (PermitPumpOn()) {
        //Open PermitRealay Output
        //request open valve

        ForceOpenValve();
        sprintf(msg, "FO_%c", OpenCloseit);
        //    myGLCD.print("Loading permit OK     ", 40, 175);
    } else {
        ForceCloseValve();
        sprintf(msg, "FC_%c", OpenCloseit);
        //    myGLCD.print("not permit for loading", 40, 175);
    }
    myGLCD.setBackColor(BackColor[0], BackColor[1], BackColor[2]); // light gray
    myGLCD.setColor(255, 255, 255);
    myGLCD.setFont(SmallFont);
    myGLCD.print(msg, 390, 25);
}

// todo to do
void CheckAndCharzheBattery() {
    int Get_BateryCharzhe = get_BateryCharzhe();
    if (get_MainPower() == true && Get_BateryCharzhe < MinimumForBatteryCharzhe)
        set_BatteryCharzheRelay(DO_On_Relay);
    else if ((get_MainPower() == true
              && Get_BateryCharzhe > MaximumForBatteryCharzhe)
             || get_MainPower() == false)
        set_BatteryCharzheRelay(DO_Off_Relay);
}

void CheckMainPower() {
    if (get_MainPower() == true) {
        FirstCheckMainPower = true;
        return;
    }
    if (FirstCheckMainPower == false)
        return;
    FirstCheckMainPower = false;
    ForceCloseValve();
    set_BatteryCharzheRelay(DO_Off_Relay);
    SendSMS("MainPowerIsOff");

}

//////////////////{{{{{{{{{{{{{{{{{{{{{{}}}}}}}}}}}}}}}}}}}}}}

boolean PumpIsRunninig() {
    return true;
}

void Log_Total_UsedHourPump(int incOneSEc = 0) {
    if (CurrentFlow > 1.0) {
        TotalValues.UsedHourPump_CountBySec += incOneSEc;
        if (TotalValues.UsedHourPump_CountBySec > 3600) {
            TotalValues.UsedHourPump_CountBySec = 0;
            TotalValues.Total_UsedHourPump++;
        }
    }
}

void CheckElectroMagnetic() {
    boolean PinState = digitalRead(ElectroMagneticPin);
    //  EventsStruct[ 7].Value=0;
    //   Serial.print("Pin 9 ==");
    //   Serial.println(PinState);
    if (PinState != EventsStruct[StrongDCMagneticFieldDetected].Value) {
        setEvent(StrongDCMagneticFieldDetected, PinState);
        if (PinState == false) {
            char msg[20];
            strcpy(TotalValues.LastDateSeeElectroMagnetic, GetStrCurrentDay(msg));
            strcpy(TotalValues.LastTimeSeeElectroMagnetic, GetCurrentStrHour(msg));
        }

    }
}

void turnOffAllRelays() {
    digitalWrite(CharzheBatteryRelay, DO_Off_Relay);
    digitalWrite(MainRelayValve, DO_Off_Relay);
    digitalWrite(SelectRelayValve_1_, DO_Off_Relay);
    digitalWrite(SelectRelayValve_2_, DO_Off_Relay);
}

void StartupRelays() {
    pinMode(MainPowerOnRelay, INPUT);
    pinMode(CharzheBatteryRelay, OUTPUT);
    pinMode(ElectroMagneticPin, INPUT_PULLUP);
    pinMode(BtnDisplayLight, INPUT_PULLUP);
    pinMode(Display_LED_PWM_PIN, INPUT);
    pinMode(MainRelayValve, OUTPUT);
    pinMode(SelectRelayValve_1_, OUTPUT);
    pinMode(SelectRelayValve_2_, OUTPUT);
    pinMode(PositionSwitchOPEN, INPUT);
    pinMode(PositionSwitchCLOSE, INPUT);
    pinMode(Pulse1Pin, INPUT);

    //  StartupRelayValves();
    turnOffAllRelays();

}

void CheckMaxFellowVolumeUSeWhenPowerOff() {
    if (TotalValues.Total_UsedVolume > TotalValues.MaxVolumeAllow) {
        if (getEvent(PermittedVolumeThresholdEexceeded) == false) {
            setEvent(PermittedVolumeThresholdEexceeded, true);
            char msg[20];
            strcpy(TotalValues.LastDatePowerOffForUseMoreThanMaxVolume, GetStrCurrentDay(msg));
            strcpy(TotalValues.LastDateUseMoreThanMaxVolume, GetStrCurrentDay(msg));
        }
    } else {
        setEvent(PermittedVolumeThresholdEexceeded, false);
    }
    if (TotalValues.V_MaxFlowIn24Hour > TotalValues.MaxFellowAllow) {
        if (getEvent(FlowrateExceeded) == false) {
            setEvent(FlowrateExceeded, true);
            char msg[20];
            strcpy(TotalValues.LastDatePowerOffForUseMoreThanMaxFellow, GetStrCurrentDay(msg));
            strcpy(TotalValues.LastDatePowerOffForUseMoreThanMaxFellow, GetStrCurrentDay(msg));
        }
    } else {
        setEvent(FlowrateExceeded, false);
    }
    if (TotalValues.TotalDuration_Charzh < 0 && CurrentFlow > 0.1) {
        if (getEvent(ErrorUsingWaterWhenPowerOff) == false) {
            setEvent(ErrorUsingWaterWhenPowerOff, true);
            char msg[20];
            strcpy(TotalValues.LastDateUseWaterWhenPowerOff, GetStrCurrentDay(msg));
        }
    } else {
        setEvent(ErrorUsingWaterWhenPowerOff, false);
    }

}

int button_11_prevVal;
uint32_t pwm_duty = 127; // 50% duty cycle
void SetDisplayPWM() {
    pwm.pinDuty(Display_LED_PWM_PIN, pwm_duty); // 100% duty cycle on Pin 6
    boolean BtnPress=digitalRead(BtnDisplayLight);
    if (button_11_prevVal != BtnPress) {
        IF_SERIAL_DEBUG(printf_New("BtnPress==----->>:%u , \n",BtnPress));
        button_11_prevVal = BtnPress;
        if ( BtnPress == 0)
            pwm_duty = (pwm_duty <= 200) ? (pwm_duty + 50) : 100;
    }

}

void TimeStartup() {

    Serial.begin(115200);
    SerialIR.begin(300, SERIAL_7E1);/// M
    SerialMBUS.begin(115200);
    if (!rtc.begin()) {
        Serial__println("Couldn't find RTC");

    }

    if (!rtc.isrunning()) {
        Serial__println("RTC is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime_RTC(2016, 1, 21, 3, 0, 0)); //(F(__DATE__), F(__TIME__)));
        setEvent(ErrorInternal_RTC, true);
        setEvent(ApplicationError, true);
    }

    if (!SD.begin(10)) {
        Serial__println("SD could not open!");
        SdError = true;
        setEvent(ErrorInternal_SDCard, true);
        setEvent(ApplicationError, true);
    }
    Set_FlashAdresses();
    bool firstcheck = FirsTimeLoadFlashMemory();
    if (firstcheck == true) {
        Serial.println("First Initialize");
        SetStutrupValues();
        SaveFirstInitialaizeFlashMemory();
        WaitForLoop += 3;
    }
//    LoadParameters();
    IF_SERIAL_DEBUG_LOOP(printf_New("load flash %f \n", TotalValues.Litre_Volume));
    LoadSavedTotalInFlashMemory();


    myGLCD.InitLCD();

    StartupRelays();
    for (int k = 0; k < 10; k++) {
        myGLCD.setFont(SmallFont);
        myGLCD.print("0", 15 * k, 100);
        delay(1000);
    }
    // Setup the LCD
    // initialize the button pin as a input:


    myGLCD.setFont(PNumFontB24);
    myGLCD.setColor(192, 192, 192);
    myGLCD.fillRect(0, 0, 479, 13);
    myGLCD.fillScr(192, 192, 192);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(BackColor[0], BackColor[1], BackColor[2]); // light gray
    myGLCD.setFont(Text_1);
    myGLCD.print("0", 380, 45);
    myGLCD.setFont(Text_7);
    myGLCD.print("0", 225, 135);
    myGLCD.setFont(Text_4); //  مصرف دوره
    myGLCD.print("0", 225 + 48, 135);
    myGLCD.setFont(Text_7);
    myGLCD.print("0", 225, 165);
    myGLCD.setFont(Text_5); // مصرف کل
    myGLCD.print("0", 225 + 48, 165);

    delay(2000);
    noInterrupts();
    ///////////////////// simulate fellow
   // TotalValues.K_param = 0.29;
    TotalValues.K_param =0.1;

    Timer5.attachInterrupt(SimulateFllow).setFrequency(1);
    Timer5.start();
    attachInterrupt(digitalPinToInterrupt(Pulse1Pin), CountFlowInterrupt20,
                    CHANGE);
    interrupts();
    ReadEvents();
    FirstGetDateTime();
    //    myGLCD.drawBitmap( 10,  10, 80,  60, cat);
//    IEC6205621_Com.MAXVolumeDefined_ = MAXVolumeDefined;
    ResetTotalPerid();
    InitializeEvents();
    InitializeNFC();
    //  pwm.setFreq1( 200);
    //  pwm.setFreq2( 4000 );
    pwm.pinFreq1(Display_LED_PWM_PIN);  // Pin 6 freq set to "pwm_freq1" on clock A

    freememoryTrace = freemeMory();
}

// initialize the library with the numbers of the interface pins
void TimeLoop() {

    CheckMainRelayValveTimeOut();

    if ((millis() / 1000) < WaitForLoop)
        return;
    if ((millis() % 1000) == TimeMill)
        return;
    TimeMill = millis() % 1000;

    if (TimeMill != 0)
        return;

    //  if(get_MilliSecondDiff(TimeMill)<1000L){
    //     return;
    // }

    /*  Serial.print("millis= ");
      Serial.print(millis());
      Serial.print(",get=");
      Serial.print(NewTimeMill);
      NewTimeMill = millis();
      Serial.print(",New=");
      Serial.println(NewTimeMill);
   */   if (get_MilliSecondDiff(LastCallMillisCountFlowInterrupt20) > 5000)
        CurrentFlow = 0;

    IF_SERIAL_DEBUG(printf_New("------>:%u\n",TestCounterFlow));
    IF_SERIAL_DEBUG_LOOP(printf_New("90001:%u\n", millis()));

    if (freemeMory() != freememoryTrace) {
        SaveEventsFile(ErrorInFreeMemory + 0);
        freememoryTrace = freemeMory();
    }
    //SimulateFllow();  // it should be run on interrupt
    if (freemeMory() != freememoryTrace) {
        SaveEventsFile(ErrorInFreeMemory + 1);
        freememoryTrace = freemeMory();
    }


    GetDateTime();
    IF_SERIAL_DEBUG_LOOP(printf_New("90002  get date time:%u\n", millis()));
//    CheckAlarmEvents();
    if (freemeMory() != freememoryTrace) {
        SaveEventsFile(ErrorInFreeMemory + 2);
        freememoryTrace = freemeMory();
    }
    SaveTotalsInFlash();
    IF_SERIAL_DEBUG_LOOP(printf_New("90003 SaveTotalsInFlash():%u\n", millis()));
    if (freemeMory() != freememoryTrace) {
        SaveEventsFile(ErrorInFreeMemory + 3);
        freememoryTrace = freemeMory();
    }

    ShowData();   // ERRRRRRRRRRRRRRRorr
    IF_SERIAL_DEBUG_LOOP(printf_New("90004  ShowData():%u\n", millis()));
    if (freemeMory() != freememoryTrace) {
        SaveEventsFile(ErrorInFreeMemory + 4);
        freememoryTrace = freemeMory();
    }

    //	CalcCurrPesianDate();
    //CurCalibrateFlow=Calibrate_Flow() ;
    //CalcTaarefe(PassNumberOfPesianDate());
    //todo : Sum total should be calculate in event one second interrupt
    //CalcRemaiCharzh(CalcTaarefe(PassNumberOfPesianDate()), SumTotal(Calibrate_Flow(), TotalValues));
    //CalcTaarefe(PassNumberOfPesianDate());
    //SumTotal(Calibrate_Flow(),TotalValues);

    //	CheckEvents();
    //	TotalValues.Total_UsedVolume  = TotalValues.Total_UsedVolume + 100.0; //+ CurCalibrateFlow / 3600.0 ;

    /*todo ok

         */


    NFC_Loop();
    IF_SERIAL_DEBUG_LOOP(printf_New("90005  NFC_Loop():%u\n", millis()));
    if (freemeMory() != freememoryTrace) {
        SaveEventsFile(ErrorInFreeMemory + 5);
        freememoryTrace = freemeMory();
    }
    OpenPermitRelay();
    IF_SERIAL_DEBUG_LOOP(printf_New("90006  OpenPermitRelay():%u\n", millis()));
    if (freemeMory() != freememoryTrace) {
        SaveEventsFile(ErrorInFreeMemory + 6);
        freememoryTrace = freemeMory();
    }
    CheckAndCharzheBattery();
    IF_SERIAL_DEBUG_LOOP(printf_New("90007  CheckAndCharzheBattery():%u\n", millis()));

    //if mainrelay is on and felo
    Log_Total_UsedHourPump(1);
    CheckMaxFellowVolumeUSeWhenPowerOff();
    IF_SERIAL_DEBUG_LOOP(printf_New("90008  CheckMaxFellowVolumeUSeWhenPowerOff():%u\n", millis()));
    //  SaveTotalsInFlash(TotalValues);
    // CheckMainPower();
    //  int sensorValue = analogRead(A8);
    //  Serial.println(sensorValue);
    if (freemeMory() != freememoryTrace) {
        SaveEventsFile(ErrorInFreeMemory + 7);
        freememoryTrace = freemeMory();
    }
     CheckElectroMagnetic();
    ShowErrorsOnScreen();
    if (freemeMory() != freememoryTrace) {
        SaveEventsFile(ErrorInFreeMemory + 9);
        freememoryTrace = freemeMory();
    }
    SetDisplayPWM();
    IEC_C_CheckModeTimeOut(false);
    IF_SERIAL_DEBUG_LOOP(printf_New("90009  EEEEEEEEEEEEEE):%u\n", millis()));

    Serial.print("Freememory_ =");
    Serial.println(freemeMory());
  }

void readFileTestEvent();

void writeStructType();

void SaveHourlyFile(StructTotalValues TotalValues);

void readFileTestHourly();

void Setalltestzero();

void Setalltestzero1();

void Setalltestzero2();

void serialEvent() {
    char inChar;
    struct EventsSaveFile_Struct *EvntStrdata;
    inChar = (char) Serial.read();
    if ((int) inChar == 0)return;
    Serial.println(inChar);
    if (inChar == 'a');//TotalValues.TotalDuration_Charzh=;
    if (inChar == 'b')SaveHourlyFile(TotalValues);
    if (inChar == 'c')Setalltestzero2();
    if (inChar == '1')SimulatrPermitPumpOn = !SimulatrPermitPumpOn;//ForceCloseValve();//Val_PositionSwitchOPEN = !Val_PositionSwitchOPEN;//SaveHourlyFile(TotalValues);
  //  if (inChar == '2')ForceOpenValve();//Val_PositionSwitchCLOSE = !Val_PositionSwitchCLOSE;//SaveHourlyFile(TotalValues);
    if (inChar == '3')DemoSaveGetEventFile();
    //  if (inChar == '3')readFileTestEvent();
    // if (inChar == '4')//readFileTestHourly();
    if (inChar == '5') {
        DisplayBrightValue = (DisplayBrightValue < 210) ? DisplayBrightValue + 10 : 170;
        Serial.println(DisplayBrightValue);
        BackColor[0] = DisplayBrightValue;
        BackColor[1] = DisplayBrightValue;
        BackColor[2] = DisplayBrightValue;
        myGLCD.setFont(PNumFontB24);
        myGLCD.setColor(BackColor[0], BackColor[1], BackColor[2]);
        myGLCD.fillRect(0, 0, 479, 13);
        myGLCD.fillScr(BackColor[0], BackColor[1], BackColor[2]);
        myGLCD.setColor(255, 255, 255);
        myGLCD.setBackColor(BackColor[0], BackColor[1], BackColor[2]); // light gray
        myGLCD.setFont(Text_1);
        myGLCD.print("0", 380, 45);
        myGLCD.setFont(Text_7);
        myGLCD.print("0", 225, 135);
        myGLCD.setFont(Text_4); //  مصرف دوره
        myGLCD.print("0", 225 + 48, 135);
        myGLCD.setFont(Text_7);
        myGLCD.print("0", 225, 165);
        myGLCD.setFont(Text_5); // مصرف کل
        myGLCD.print("0", 225 + 48, 165);

    }
    if (inChar == 'D')if (SD.exists("Hourly.log"))SD.remove("Hourly.log");//Hourly.log//"EvwntLog.log
    if (inChar == 'N')writeStructType();

}

int IEC_C_serial_recv();

void SerialIREvent() {
    /*  char inChar;
      int dd;
      while (Serial1.available()) {
          inChar = (char) Serial1.read();
          if ((int) inChar == 0)return;
          Serial.println(inChar);
          dd= (int)inChar;
          Serial.println(dd);
      }*/
    //IEC6205621_Com.ExternSerialEvent1();
    int i;
    i = IEC_C_serial_recv();
}
int mbus_serial_recv();
void SerialMBUSEvent() {
    int i;
    i = mbus_serial_recv();
}

void getDump() {
    /*  Serial1.println("AABB3,2,28,913950401.CSVDDEE1");
     Serial1.println("AABB3,4,28,1234567890GGGDDEE1");
     Serial1.println("AABB3,4,28,123456a890GGGDDEE1");
     Serial1.println("AABB3,4,28,123456c890GGGDDEE1");
     Serial1.println("AABB3,4,28,123456g890GGGDDEE1");
     Serial1.println("AABB3,4,28,123456r890GGGDDEE1");
     Serial1.println("AABB3,4,28,123456t890GGGDDEE1");
     Serial1.println("AABB3,3,28,913950401.CSVDDEE1");

     return;*/
    String buf;
    File dumpFile = SD.open("EvwntLog.log");
    if (dumpFile) {
        //    lcd.println(charBuf);
        while (dumpFile.available()) {
            //Serial1.write(dumpFile.read());
            buf = dumpFile.readStringUntil('\n');
            Serial.println(buf); //Printing for debugging purpose           Serial1.println(buf);
        }
        dumpFile.close();
        //Serial1.println("############");

        //    running();
    } else {
        Serial.println("error opening file");
    }

}

int StartupRowShow = 10;

void ShowMessageOnStartup(String msg) {
    myGLCD.setFont(SmallFont);
    myGLCD.print(msg, 20, StartupRowShow);
    StartupRowShow += 10;
}

char *Get_I_Meter_serial_number() {
    return TotalValues.SerialKontor;
}

void SetStutrupValues() {
    sprintf(TotalValues.SerialKontor, "%s", "ABCDEFGHI");
    sprintf(TotalValues.SerialConsule, "%s", "ABCDEF1234");
    sprintf(TotalValues.CreatedDate, "%s", "13950501");
    TotalValues.Total_UsedVolume = 10001;
    TotalValues.Total_UsedHourPump = 10002;
    TotalValues.TotalDuration_Charzh = 10003;
    sprintf(TotalValues.LastDateUseMoreThanMaxVolume, "%s", "13950502");
    sprintf(TotalValues.LastDateUseMoreThanMaxFellow, "%s", "13950503");
    sprintf(TotalValues.LastDatePowerOffForUseMoreThanMaxVolume, "%s",
            "13950504");
    sprintf(TotalValues.LastDatePowerOffForUseMoreThanMaxFellow, "%s",
            "13950505");
    TotalValues.CountOpenDoor = 10005;
    sprintf(TotalValues.LastDateUseWaterWhenPowerOff, "%s", "13950506");
    TotalValues.VolumeUseWhenPowerOff = 10006;
    TotalValues.CountOpenConsule = 1007;
    sprintf(TotalValues.LastDateSeeElectroMagnetic, "%s", "13950507");
    sprintf(TotalValues.LastTimeSeeElectroMagnetic, "%s", "050607");
    TotalValues.batteryStatus = 123;
    TotalValues.InternalError = 124;
    sprintf(TotalValues.NewFirmwareDate, "%s", "13950508");
    sprintf(TotalValues.LastDateCharzhe, "%s", "13950509");
    sprintf(TotalValues.LastUserConnectedDate, "%s", "13950510");
    TotalValues.LastUserConnectedCode = 125;
    sprintf(TotalValues.LastDateKontorConfig, "%s", "13950509");
    TotalValues.DateTaarefe[0] = 15;
    TotalValues.DateTaarefe[1] = 87;
    TotalValues.DateTaarefe[2] = 120;

    TotalValues.Taarefe[0] = 0.9;
    TotalValues.Taarefe[1] = 0.8;
    TotalValues.Taarefe[2] = 0.7;
    TotalValues.Taarefe[3] = 0.6;
}
