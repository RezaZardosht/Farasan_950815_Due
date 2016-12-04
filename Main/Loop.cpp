//C:\Program Files (x86)\Arduino\hardware\tools\avr\lib\gcc\avr\4.8.1\include;C:\Program Files (x86)\Arduino\libraries
//set(${CMAKE_PROJECT_NAME}_BOARD uno)
//set(${CMAKE_PROJECT_NAME}_PORT COM3)

// bluetooth change baudrate
//Serial1.begin(Current BaudRate);
//Serial1.print("AT");
//  Serial1.print("AT+BAUD4");
//Serial.println("AT115200");
#include "loop.h"


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
#define MAXVolumeDefined  2000

void SetStutrupValues();

int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
unsigned long FlowMicroSecDiff = 0;
unsigned long PreFlowMicroSecDiff = 0;
unsigned long LastCallMillisCountFlowInterrupt20 = 0;
float CurrentFlow, MovingAvgFlow[110];


CGPRS_SIM800 gprs;
RTC_DS1307 rtc;
UTFT myGLCD(CTE32HR, 38, 39, 40, 41);
IEC62056_21_Serial IEC6205621_Com;

//////////////////////////////   Fonts
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
extern uint8_t Text_8[];
extern uint8_t Text_9[];
extern uint8_t Text_10[];
extern uint8_t Text_11[];
extern uint8_t Text_12[];
extern uint8_t Text_13[];
extern uint8_t Text_14[];
extern uint8_t Text_15[];
extern uint8_t Text_16[];
extern uint8_t Text_17[];

extern unsigned int cat[];// bitmap
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
long TimeMill = 0;
int WaitForLoop = 0;

int Add_PreCheckValueIn_EEP;
int Add_TotalValues_EEP;
int Add_CalibrateConfig;

char CurrSPDate[20];

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

    if (!strcmp(Obis, "0-4:96,1,0,255"))
        sprintf(RetVal, "%s(%s)\r\n", Obis, TotalValues.SerialKontor);
    if (!strcmp(Obis, "0-4:96,1,1,255"))
        sprintf(RetVal, " %s(ABCDEF1234)\r\n", Obis, TotalValues.SerialConsule);
    if (!strcmp(Obis, "0-4:96,1,2,255"))
        sprintf(RetVal, " %s(13950501)\r\n", Obis, TotalValues.CreatedDate);
    if (!strcmp(Obis, "0-4:24,2,1,255"))
        sprintf(RetVal, " %s(%lu  * m3)\r\n", Obis,
                TotalValues.Total_UsedVolume);
    if (!strcmp(Obis, "0-4:24,2,4,255"))
        sprintf(RetVal, " %s(%lu * h)\r\n", Obis,
                TotalValues.Total_UsedHourPump);
    if (!strcmp(Obis, "0-4:24,2,3,255"))
        sprintf(RetVal, " %s(%lu * m3)\r\n", Obis,
                TotalValues.TotalDuration_Charzh);
    if (!strcmp(Obis, "0-4:24,2,2,255"))
        sprintf(RetVal, " %s(%s)\r\n", Obis,
                TotalValues.LastDateUseMoreThanMaxVolume);
    if (!strcmp(Obis, "0-4:24,2,15,255"))
        sprintf(RetVal, " %s(%s)\r\n", Obis,
                TotalValues.LastDateUseMoreThanMaxFellow);
    if (!strcmp(Obis, "0-4:24,2,16,255"))
        sprintf(RetVal, " %s(%s)\r\n", Obis,
                TotalValues.LastDatePowerOffForUseMoreThanMaxVolume);
    if (!strcmp(Obis, "0-4:24,2,17,255"))
        sprintf(RetVal, " %s(%s)\r\n", Obis,
                TotalValues.LastDatePowerOffForUseMoreThanMaxFellow);
    if (!strcmp(Obis, "0-4:24,2,18,255"))
        sprintf(RetVal, " %s(%lu)\r\n", Obis, TotalValues.CountOpenDoor);
    if (!strcmp(Obis, "0-4:24,2,19,255"))
        sprintf(RetVal, " %s(%s)\r\n", Obis,
                TotalValues.LastDateUseWaterWhenPowerOff);
    if (!strcmp(Obis, "0-4:24,2,20,255"))
        sprintf(RetVal, " %s(%lu)\r\n", Obis,
                TotalValues.VolumeUseWhenPowerOff);
    if (!strcmp(Obis, "0-4:24,2,21,255"))
        sprintf(RetVal, " %s(%lu)\r\n", Obis, TotalValues.CountOpenConsule);
    if (!strcmp(Obis, "0-4:24,2,22,255"))
        sprintf(RetVal, " %s(%s)\r\n", Obis,
                TotalValues.LastDateSeeElectroMagnetic);
    if (!strcmp(Obis, "0-4:24,2,23,255"))
        sprintf(RetVal, " %s(%s)\r\n", Obis,
                TotalValues.LastTimeSeeElectroMagnetic);
    if (!strcmp(Obis, "0-4:24,2,24,255"))
        sprintf(RetVal, " %s(%d)\r\n", Obis, TotalValues.batteryStatus);
    if (!strcmp(Obis, "0-4:24,2,25,255"))
        sprintf(RetVal, " %s(%d)\r\n", Obis, TotalValues.InternalError);
    if (!strcmp(Obis, "0-4:24,2,26,255"))
        sprintf(RetVal, " %s(%s)\r\n", Obis, TotalValues.NewFirmwareDate);
    if (!strcmp(Obis, "0-4:24,2,27,255"))
        sprintf(RetVal, " %s(%s)\r\n", Obis, TotalValues.LastDateCharzhe);
    if (!strcmp(Obis, "0-4:24,2,28,255"))
        sprintf(RetVal, " %s(%s)\r\n", Obis, TotalValues.LastUserConnectedDate);
    if (!strcmp(Obis, "0-4:24,2,29,255"))
        sprintf(RetVal, " %s(%d)\r\n", Obis, TotalValues.LastUserConnectedCode);
    if (!strcmp(Obis, "0-4:24,2,30,255"))
        sprintf(RetVal, " %s(%s)\r\n", Obis, TotalValues.LastDateKontorConfig);
    if (!strcmp(Obis, "0-4:24,2,31,255"))
        sprintf(RetVal, " %s(%02d%02d;%02d%02d;%02d%02d)\r\n", Obis,
                GetMonthOfForSumofDays(TotalValues.DateTaarefe[0]),
                GetDayOfMonthForSumofDays(TotalValues.DateTaarefe[0]),
                GetMonthOfForSumofDays(TotalValues.DateTaarefe[1]),
                GetDayOfMonthForSumofDays(TotalValues.DateTaarefe[1]),
                GetMonthOfForSumofDays(TotalValues.DateTaarefe[2]),
                GetDayOfMonthForSumofDays(TotalValues.DateTaarefe[2])
        );
    if (!strcmp(Obis, "0-4:24,2,32,255")) {
        char FFstr[4][10];
        for (int k = 0; k < 4; k++) {
            for (int i = 0; i < 10; i++)
                FFstr[k][i] = '\0';
            Dtostrf(TotalValues.Taarefe[k], 4, 2, FFstr[k]);
        }
        sprintf(RetVal, " %s(%s;%s;%s;%s)\r\n", Obis, FFstr[0], FFstr[1],
                FFstr[2], FFstr[3]);
    }
//  return Ret;

}

int FontRowPos(int row) {
    return 105 + row * 30;
}

float MaxFlowIn24Hour() {
    char CurrDate[6];
    sprintf(CurrDate, "%02d%02d%02d", year(), month(), day());

    if (!strcmp(CurrDate, TotalValues.V_MaxFlowIn24Hour_LogDay)) {
        TotalValues.V_MaxFlowIn24Hour = 0.0;
        strcpy(TotalValues.V_MaxFlowIn24Hour_LogDay, CurrDate);
    } else
        TotalValues.V_MaxFlowIn24Hour =
                (CurrentFlow > TotalValues.V_MaxFlowIn24Hour) ?
                CurrentFlow : TotalValues.V_MaxFlowIn24Hour;
    if (TotalValues.V_MaxFlowIn24Hour > TotalValues.MaxFellowAllow)
        setEvent(true, FlowrateExceeded);
    else
        setEvent(false, FlowrateExceeded);
    return TotalValues.V_MaxFlowIn24Hour;
}

unsigned int V_PompTotalSec = 0;

unsigned long PompTotalHour(int incOneSEc = 0) {
    if (incOneSEc > 0)
        TotalValues.V_PUmpTotalHour++;
    if (V_PompTotalSec >= 60) {
        V_PompTotalSec = 0;
        TotalValues.V_PUmpTotalHour++;
    }
    return TotalValues.V_PUmpTotalHour;
}

void SimulateFllow(void) {
    SumTotal(10000, TotalValues);
}

void CountFlowInterrupt20() {
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

void IncreseCharzh(int Value, char DateStart[10], char DateEnd[10]) {

    TotalValues.TotalDuration_Charzh = TotalValues.TotalDuration_Charzh + Value;
    memcpy(TotalValues.DateStart, DateStart, 10);
    TotalValues.DateStart[10] = '\0';
    memcpy(TotalValues.DateEnd, DateEnd, 10);
    TotalValues.DateEnd[10] = '\0';
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
    IF_SERIAL_DEBUG(printf_New("First Time loading flash = %d", (PreCheckValueInMmory.kk == 12345) ? 0 : 1));


    if (PreCheckValueInMmory.kk == 12345)
        return false;
    else
        return true;
}

void SaveFirstInitialaizeFlashMemory() {
    strncpy(PreCheckValueInMmory.TTFName, TTFName, sizeof(TTFName));
    strncpy(PreCheckValueInMmory.KodeVersion, KodeVersion, sizeof(KodeVersion));
    PreCheckValueInMmory.kk = 12345;
    EEProm_Put<StructPreCheckValueInMmory>(Add_PreCheckValueIn_EEP,
                                           PreCheckValueInMmory);
    EEProm_Put<StructTotalValues>(Add_TotalValues_EEP, TotalValues);

}

void LoadSavedTotalInFlashMemory() {
    EEProm_Get<StructTotalValues>(Add_TotalValues_EEP, TotalValues);

    IF_SERIAL_DEBUG(printf_New("load flash %f \n", TotalValues.Litre_Volume));
}

void SaveTotalsInFlash() {
    EEProm_Put<StructTotalValues>(Add_TotalValues_EEP, TotalValues);
    IF_SERIAL_DEBUG(printf_New("Save flash %f \n", TotalValues.Litre_Volume));

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
    passNumofyear = PassNumberOfPesianDate(year(), month(), day());

    if (passNumofyear < TotalValues.DateTaarefe[0])return TotalValues.Taarefe[0];
    if (passNumofyear < TotalValues.DateTaarefe[1])return TotalValues.Taarefe[1];
    if (passNumofyear < TotalValues.DateTaarefe[2])return TotalValues.Taarefe[2];
    return TotalValues.Taarefe[4];

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
    unsigned long int IncP_CurrTotalValues = 0, TempP_Litre_Volume =
            P_CurrTotalValues->Litre_Volume;
    while (P_CurrTotalValues->Litre_Volume >= 1000.0) {
        P_CurrTotalValues->Litre_Volume = P_CurrTotalValues->Litre_Volume
                                          - 1000.0;
        IncP_CurrTotalValues++;
    }

    P_CurrTotalValues->Duration_Volume = P_CurrTotalValues->Duration_Volume
                                         + IncP_CurrTotalValues;
    P_CurrTotalValues->Total_UsedVolume = P_CurrTotalValues->Total_UsedVolume
                                          + IncP_CurrTotalValues;
    P_CurrTotalValues->TotalDuration_Charzh =
            P_CurrTotalValues->TotalDuration_Charzh - IncP_CurrTotalValues * GetcurrentTaarefe();

    /* char str_CurrentFlow[10], SaveTempString[100];;
     Dtostrf(F_IncLitre, 4, 2, str_CurrentFlow);
     sprintf(SaveTempString, "@%0ld:%0ld,%ld,%s,%ld,%ld.%ld,%ld",IncP_CurrTotalValues,P_CurrTotalValues->Litre_Volume ,TempP_Litre_Volume, str_CurrentFlow,P_CurrTotalValues->TotalDuration_Charzh,P_CurrTotalValues->Duration_Volume, (long)((int)P_CurrTotalValues->Litre_Volume / 10), P_CurrTotalValues->Total_UsedVolume);
     Serial__println(SaveTempString);
     */

    char tttemp[100], str_temp1[10], str_temp2[10], str_temp3[10],
            str_temp4[10], str_temp5[10], str_temp6[10];
    Dtostrf(TotalValues.K_param, 4, 2, str_temp1);
    Dtostrf(IncLitre / TotalValues.K_param, 4, 2, str_temp2);
    Dtostrf(P_CurrTotalValues->Litre_Volume, 4, 2, str_temp3);
    float tttt = (60000000.0 / FlowMicroSecDiff);
    Dtostrf(tttt, 4, 2, str_temp4);
    float tttt2 = tttt * IncLitre / TotalValues.K_param / 10000.0;
    Dtostrf(tttt2, 4, 2, str_temp5);

    float Temp_CurrentFlow = 0;
    for (int i = 0; i < 99; i++)
        MovingAvgFlow[i + 1] = MovingAvgFlow[i];
    // flow litre/minutes == change volume between 2pulse / diffrent time between 2 pulse

    MovingAvgFlow[0] = tttt2; //( IncLitre / ParametersConfig.K_param) * (60000000.0 / FlowMicroSecDiff) ;// litre/minute
    for (int i = 0; i < 100; i++)
        Temp_CurrentFlow += MovingAvgFlow[i];
    CurrentFlow = Temp_CurrentFlow / 100.0;
    Dtostrf(CurrentFlow, 4, 2, str_temp6);
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
    myGLCD.setBackColor(192, 192, 192); // light gray
    myGLCD.print(CurrSPDate, 40, 15);
}

void LCDShowCurrFlow() {
    char FFstr[20];
    unsigned long TempDuration_Volume = TotalValues.Duration_Volume;
    long TempTotalDuration_Charzh = TotalValues.TotalDuration_Charzh;
    unsigned long TempTotal_UsedVolume = TotalValues.Total_UsedVolume;
    unsigned long TempLitre_Volume = TotalValues.Litre_Volume;
    myGLCD.setFont(PNumFontB24);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(192, 192, 192); // light gray

    memcpy(FFstr, TotalValues.DateStart, 10);
    FFstr[10] = 0;
    myGLCD.print(FFstr, 200, 45);
    memcpy(FFstr, TotalValues.DateEnd, 10);
    FFstr[10] = 0;
    myGLCD.print(FFstr, 15, 45);

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
    } else {
        myGLCD.setFont(Text_7); //(m3)
        myGLCD.print("0", 225, FontRowPos(0));
        myGLCD.setFont(Text_3); //قابل برداشت
        myGLCD.print("0", 225 + 48, FontRowPos(0));
        myGLCD.setFont(PNumFontB24);
        myGLCD.print(FFstr, 15, 105);
    }
    // مصرف دوره
    for (int i = 0; i < 20; i++)
        FFstr[i] = '\0';
    if ((int) TempLitre_Volume / 10 < 100)
        sprintf(FFstr, "%ld.0%ld", TempDuration_Volume,
                (long) ((int) TempLitre_Volume / 10));
    else
        sprintf(FFstr, "%ld.%ld", TempDuration_Volume,
                (long) ((int) TempLitre_Volume / 10));

    for (int i = strlen(FFstr); i < 13; i++)
        FFstr[i] = '-';
    myGLCD.print(FFstr, 15, FontRowPos(1));
    // Serial.println(TempDuration_Volume);
    // مصرف کل
    for (int i = 0; i < 20; i++)
        FFstr[i] = '\0';
    sprintf(FFstr, "%ld", TempTotal_UsedVolume);
    for (int i = strlen(FFstr); i < 13; i++)
        FFstr[i] = '-';
    myGLCD.print(FFstr, 15, FontRowPos(2));

    // جریان
    myGLCD.setFont(Text_9);  //  (m3/m)
    myGLCD.print("0", 335, FontRowPos(3));
    myGLCD.setFont(Text_8);  //  جریان
    myGLCD.print("0", 225 + 190, FontRowPos(3));
    myGLCD.setFont(PNumFontB24);
    for (int i = 0; i < 20; i++)
        FFstr[i] = 0;
    Dtostrf(CurrentFlow, 4, 2, FFstr);
    for (int i = strlen(FFstr); i < 7; i++)
        FFstr[i] = '-';
    myGLCD.print(FFstr, 225, FontRowPos(3));

    //  بیشینه  جریان
    char F2str[20];
    memset(F2str, 0, 20);
    myGLCD.setFont(Text_10);  // بیشینه
    myGLCD.print("0", 135, FontRowPos(3));
    myGLCD.setFont(PNumFontB24);
    float MaxFlowIn24Hour_V = MaxFlowIn24Hour();
    Dtostrf(TotalValues.V_MaxFlowIn24Hour, 4, 2, F2str);
    FFstr[strlen(F2str)] = 0;
    myGLCD.print(F2str, 15, FontRowPos(3));

    memset(F2str, 0, 20);
    //  ساعات کارکرد پمپ
    myGLCD.setFont(Text_11);
    myGLCD.print("0", 225, FontRowPos(4));
    myGLCD.setFont(PNumFontB24);
    sprintf(F2str, "%d", TotalValues.Total_UsedHourPump);
    FFstr[strlen(F2str)] = 0;
    myGLCD.print(F2str, 15, FontRowPos(4));

    // نمایش آنتن موبایل
    myGLCD.setFont(Text_12);
    myGLCD.print("0", 225, 5);

    // نمایش آلارم مصرف
    myGLCD.setFont(Text_13);
    myGLCD.print("0", 255, 5);

    // رله باز
    //  myGLCD.setFont(Text_14);
    // myGLCD.print("0", 285, 5);

    //  // رله بسته
    myGLCD.setFont(Text_15);
    myGLCD.print("0", 285, 5);

    // باطری تمام
    // رله بسته
    myGLCD.setFont(Text_16);
    myGLCD.print("0", 355, 5);

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
    for (int i = 0; i < 20; i++)
        FFstr[i] = '\0';
    Dtostrf(TotalValues.K_param, 4, 2, FFstr);

    //FtoStr(ParametersConfig.K_param , FFstr, 2) ;
    for (int i = strlen(FFstr); i < 13; i++)
        FFstr[i] = '-';

    //myGLCD.print("    ", 410, 15);
    myGLCD.print(FFstr, 410, 15);

    buttonState = digitalRead(Pulse1Pin);
    int i = (buttonState == true) ? 1 : 0;
    //  Serial__println(i);

}

void ShowData() {
    LCDShowCurrDateTime();
    LCDShowCurrFlow();
}
//////////////////[[[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]]]

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
    IF_SERIAL_DEBUG(Serial__println(CurrSPDate));

}

void GetDateTime() {
    int ys, ms, ds;
    m2s(year(), month(), day(), ys, ms, ds);

    sprintf(CurrSPDate, "%d/%02d/%02d  %02d:%02d:%02d", ys, ms, ds, hour(),
            minute(), second());
   // IF_SERIAL_DEBUG()
            printf_New(CurrSPDate,0);
}

void SetDateTimeRTC(int hour_, int minute_, int second_, int day_, int month_,
                    int year_) {
    setTime(hour_, minute_, second_, day_, month_, year_);
    DateTime_RTC dt = now();
    rtc.adjust(dt);

}

// todo to do
bool get_MainPower() {

    bool MainPower = digitalRead(MainPowerOnRelay);
    if (MainPower == false) {
        setEvent(true, PowerDown);
        setEvent(false, PowerUp);
    } else {
        setEvent(false, PowerDown);
        setEvent(true, PowerUp);
    }
}

int get_BateryCharzhe() {
    char msg[20];
    static int Prev_BattPower = 0;
    int ColP;
    int BattPower = analogRead(AIBattery);
    if (BattPower == Prev_BattPower)
        return BattPower;
    if (BattPower > 1000)
        BattPower = 1000;
    Prev_BattPower = BattPower;
    if (BatteryRemainlife() < BatterRemainLifeConst || BatteryCharzheError())
        setEvent(true, ReplaceBattery);

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
}

bool FirstCheckMainPower;

void SendSMS(String Value) {
}

const int MinimumForBatteryCharzhe = 100;
const int MaximumForBatteryCharzhe = 110;

bool PermitCharzhe() {
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

    if (PermitCharzhe()) {
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

void Log_Total_UsedHourPump() {
    if (PumpIsRunninig()) {
        TotalValues.UsedHourPump_CountBySec++;
        if (TotalValues.UsedHourPump_CountBySec > 3600) {
            TotalValues.UsedHourPump_CountBySec = 0;
            TotalValues.Total_UsedHourPump++;
        }
    }
}

void TimeStartup() {
    pinMode(MainPowerOnRelay, INPUT);
    pinMode(CharzheBatteryRelay, OUTPUT);

    Serial.begin(115200);
    Serial1.begin(115200);/// M

    if (!rtc.begin()) {
        Serial__println("Couldn't find RTC");
        while (1);
    }

    if (!rtc.isrunning()) {
        Serial__println("RTC is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime_RTC(2016, 1, 21, 3, 0, 0)); //(F(__DATE__), F(__TIME__)));
    }

    if (!SD.begin(10)) {
        Serial__println("SD could not open!");
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
    IF_SERIAL_DEBUG(printf_New("load flash %f \n", TotalValues.Litre_Volume));
    LoadSavedTotalInFlashMemory();

    StartupRelayValves();
    // Setup the LCD
    // initialize the button pin as a input:
    pinMode(Pulse1Pin, INPUT);

    myGLCD.InitLCD();
    myGLCD.setFont(PNumFontB24);
    myGLCD.setColor(192, 192, 192);
    myGLCD.fillRect(0, 0, 479, 13);
    myGLCD.fillScr(192, 192, 192);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(192, 192, 192); // light gray
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

    delay(1000);
    noInterrupts();
    ///////////////////// simulate fellow
    TotalValues.K_param = 33.0;
    //    Timer1.initialize(100000);
    //    Timer1.attachInterrupt(SimulateFllow);//CountFlowInterrupt43);//
    attachInterrupt(digitalPinToInterrupt(Pulse1Pin), CountFlowInterrupt20,
                    CHANGE);
    interrupts();
    ReadEvents();
    FirstGetDateTime();
    //    myGLCD.drawBitmap( 10,  10, 80,  60, cat);
    IEC6205621_Com.MAXVolumeDefined_ = MAXVolumeDefined;
    ResetTotalPerid();

}

// initialize the library with the numbers of the interface pins
void TimeLoop() {

    CheckMainRelayValveTimeOut();

    char SSS[100];
    if ((millis() / 1000) < WaitForLoop)
        return;
    if ((millis() % 1000) == TimeMill)
        return;
    TimeMill = millis() % 1000;

    if (TimeMill != 0)
        return;
    if (get_MilliSecondDiff(LastCallMillisCountFlowInterrupt20) > 5000)
        CurrentFlow = 0;

    SimulateFllow();  // it should be run on interrupt

    GetDateTime();
//    CheckAlarmEvents();
    SaveTotalsInFlash();
    printf_New("---> %d,\n", TotalValues.DateTaarefe[0]);
    ShowData();   // ERRRRRRRRRRRRRRRorr

    Serial.print("Freememory_ =");
    Serial.println(freemeMory());


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
    OpenPermitRelay();
    CheckAndCharzheBattery();

    //if mainrelay is on and felo
    if (get_MainRelayValVePosition() == DO_On_Relay)
        PompTotalHour();
    if (TotalValues.Total_UsedVolume > TotalValues.MaxVolumeAllow)
        setEvent(true, PermittedVolumeThresholdEexceeded);
    else
        setEvent(false, PermittedVolumeThresholdEexceeded);
    //  SaveTotalsInFlash(TotalValues);
    // CheckMainPower();
    //  int sensorValue = analogRead(A8);
    //  Serial.println(sensorValue);
    IEC6205621_Com.ShoeLevelCommunicate();

}

void readFileTest();
void writeStructType();
void serialEvent() {
    char inChar;
    inChar = (char) Serial.read();
    if ((int) inChar == 0)return;
    Serial.println(inChar);
    if (inChar == '1')SaveEventsFile(1);
    if (inChar == '2')readFileTest();
    if (inChar == '3')DemoSaveGetEventFile();
    if (inChar == 'D')if (SD.exists("EvwntLog.log"))SD.remove("EvwntLog.log");
    if(inChar=='N')writeStructType();

}

void serialEvent1() {
    IEC6205621_Com.ExternSerialEvent1();

    /* char inChar;
     inChar = (char)Serial.read();
     if ((int )inChar == 0 )return;
     Serial.println(inChar);
     if (inChar == 'd') getDump();
     if (inChar == 'e') Send_ParametersValues();
     if (inChar == 'f') Serial1.println("AABB3,4,28,1234567890GGGDDEE1");*/

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
    TotalValues.DateTaarefe[0] = 90;
    TotalValues.DateTaarefe[1] = 87;
    TotalValues.DateTaarefe[2] = 15;

    TotalValues.Taarefe[0] = 0.9;
    TotalValues.Taarefe[1] = 0.8;
    TotalValues.Taarefe[2] = 0.7;
    TotalValues.Taarefe[3] = 0.6;
}
