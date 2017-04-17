#include "loop.h"

void Show_EventsOnScreen(char *msg);

//int CurrentLevelShowOnScreen = 0;
//int MaxNumOfCurrentError = 0;
const int EventErrors[] = {PowerDown, ApplicationError, StrongDCMagneticFieldDetected, MeterCoverRemoved,
                           PermittedVolumeThresholdEexceeded, ElectricalCurrentDisconnected, TamperedWaterFlowDetected,
                           MBusDisconnected, ErrorInFreeMemory, ErrorInternal_SDCard, ErrorInternal_RTC,
                           ErrorInternal_RFID};


EventsStructDefine EventsStruct[MAX_Event_DEFINE];
unsigned long WaitTimeShowErrors = 0;
//int EventsErrorsOnScreenListShow[MAX_Event_DEFINE];
//int chekTest[MAX_Event_DEFINE] = {0, 0, 1, 1, 0, 1, 0, 0};
int CureentShowing = 0;


void Setalltestzero2() {
    EventsStruct[0].Value = 0;
    EventsStruct[1].Value = 0;
    EventsStruct[2].Value = 1;
    EventsStruct[3].Value = 1;
    EventsStruct[4].Value = 0;
    EventsStruct[5].Value = 1;
    EventsStruct[6].Value = 1;
    EventsStruct[7].Value = 1;
    EventsStruct[8].Value = 1;
    EventsStruct[9].Value = 0;
    EventsStruct[10].Value = 1;
    EventsStruct[11].Value = 1;
    EventsStruct[12].Value = 0;

}

void ShowErrorsOnScreen() {
    if (get_MilliSecondDiff(WaitTimeShowErrors) < 5000) {
        return;
    }
    WaitTimeShowErrors = millis();

    int last_CurrentShow = CureentShowing;
    int k = 0, j = ++CureentShowing;
    boolean FoundNext = false;
//    int arrsize = sizeof(chekTest) / sizeof(chekTest[0]);
//    while ((k < arrsize) && (FoundNext == false)) {
//        if (chekTest[j] == 1) {
//            CureentShowing = j;
//            FoundNext = true;
//        }
//        j = (j < (MAX_Event_DEFINE - 1)) ? ++j : 0;
//        k++;
//    }
//    for(int i=0;i<MAX_Event_DEFINE;i++)
//        if(EventsStruct[i].Value==true)
//            printf_New("\nErrror[%d=%d,%d]\n",i,EventsStruct[i].ErrorShowOnScreen,EventsStruct[i].Value==true);
    while ((k < MAX_Event_DEFINE) && (FoundNext == false)) {
        if (EventsStruct[j].ErrorShowOnScreen == true && EventsStruct[j].Value == true) {
            CureentShowing = j;
            FoundNext = true;
        }
        j = (j < (MAX_Event_DEFINE - 1)) ? ++j : 0;
        k++;
    }
    if (FoundNext == false)CureentShowing = last_CurrentShow;
    if (CureentShowing >= 0)
        if (EventsStruct[CureentShowing].Value == false ||
            EventsStruct[CureentShowing].ErrorShowOnScreen == false)
            CureentShowing = -1;

    char msg[100];
    sprintf(msg, "      ", 0);
    if (CureentShowing >= 0) {
        sprintf(msg, "E:%03d", CureentShowing);
    }
    Show_EventsOnScreen(msg);

}
//find last showing error
//loop from last showing error to find next error must be show

/*void SetEventsErrrsOnScreen(int ErrorNum) {
    boolean ErrorInList = true, SetValue = false;

    for (int i = 0; i < MAX_Event_DEFINE; i++) {
        if (EventsErrorsOnScreenListShow[i] == ErrorNum)
            ErrorInList = false;
    }
    if (ErrorInList == true) {
        for (int i = 0; i < MAX_Event_DEFINE; i++) {
            if (EventsErrorsOnScreenListShow[i] == -1)
                if (SetValue == false) {
                    EventsErrorsOnScreenListShow[i] = ErrorNum;
                    SetValue = true;
                }
        }
    }
}

void ShowErrorsOnScreen__1() {


    if (get_MilliSecondDiff(WaitTimeShowErrors) < 5000) {
        return;
    }
    WaitTimeShowErrors = millis();
    CurrentLevelShowOnScreen = (CurrentLevelShowOnScreen < ((sizeof(EventErrors)/EventErrors[0]) - 1)) ? ++CurrentLevelShowOnScreen : 0;
    char msg[100];
    sprintf(msg, "      ");
  *//*  myGLCD.setFont(BigFont);
    myGLCD.print(msg, 100, 250);

    for (int i = 0; i < MAX_Event_DEFINE; i++) {
        if (EventsStruct[i].ErrorValue && CurrentLevelShowOnScreen == i) {
            sprintf(msg, "%d:éâU", i);
            myGLCD.setFont(BigFont);
        }

    }
*//*}*/
void nothingFunc() {}

void ShowIconBattery(boolean Show);

void ShowBatteryLevel() {
    ShowIconBattery(true);
}

void HideBatteryLevel() {
    ShowIconBattery(false);
}

void InitializeEvents() {

    for (int i = 0; i < MAX_Event_DEFINE; i++) {
        EventsStruct[i].SaveOnFile = true;
        EventsStruct[i].EventNegativePositive = true;
        EventsStruct[i].ErrorShowOnScreen = false;
        EventsStruct[i].onSetEventOn = nothingFunc;
        EventsStruct[i].onSetEventOff = nothingFunc;

        for (int k = 0; k < (sizeof(EventErrors) / EventErrors[0]); k++) {
            if (EventErrors[k] == i)
                EventsStruct[i].ErrorShowOnScreen = true;
        }
    }
    EventsStruct[StrongDCMagneticFieldDetected].EventNegativePositive = false;
    EventsStruct[PowerDown].onSetEventOn = &ShowBatteryLevel;
    EventsStruct[PowerDown].onSetEventOff = &HideBatteryLevel;
    EventsStruct[PowerUp].onSetEventOn = &HideBatteryLevel;
    EventsStruct[PowerUp].onSetEventOff = &ShowBatteryLevel;

}

void setEvent(int EventNumber, boolean SetValue) {
    if (EventNumber >= MAX_Event_DEFINE || EventNumber < 1) return;
    if (EventsStruct[EventNumber].Value == SetValue) return;
    if ((SetValue == true && EventsStruct[EventNumber].EventNegativePositive == true) ||
        (SetValue == false && EventsStruct[EventNumber].EventNegativePositive == false)) {

        EventsStruct->ErrorValue = true;
        EventsStruct[EventNumber].onSetEventOn();
        if (EventsStruct[EventNumber].SaveOnFile)
            SaveEventsFile(EventNumber);
    } else {
        EventsStruct->ErrorValue = false;
        EventsStruct[EventNumber].onSetEventOff();

    }

    EventsStruct[EventNumber].Value = SetValue;
}

boolean getEvent(int EventNumber) {
    if ((EventsStruct[EventNumber].EventNegativePositive == true && EventsStruct[EventNumber].Value == true) ||
        (EventsStruct[EventNumber].EventNegativePositive == false && EventsStruct[EventNumber].Value == false))
        return true;
    return false;
}

void ReadEvents() {
    EventsStruct[PowerDown].EventNegativePositive = true; // check rise Event in negative or positive mode
    EventsStruct[PowerDown].Value = false;
    EventsStruct[PowerDown].SaveOnFile = false;

}

