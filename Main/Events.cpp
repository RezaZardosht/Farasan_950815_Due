#include "loop.h"
#include "Loop.h"

void Show_EventsOnScreen(char *msg);

//int CurrentLevelShowOnScreen = 0;
//int MaxNumOfCurrentError = 0;
const int EventErrors[] = {PowerDown, ApplicationError, StrongDCMagneticFieldDetected, MeterCoverRemoved,
                           PermittedVolumeThresholdEexceeded, ElectricalCurrentDisconnected, TamperedWaterFlowDetected,
                           MBusDisconnected};


EventsStructDefine EventsStruct[MAX_Event_DEFINE];
unsigned long WaitTimeShowErrors = 0;
//int EventsErrorsOnScreenListShow[MAX_Event_DEFINE];
//int chekTest[MAX_Event_DEFINE] = {0, 0, 1, 1, 0, 1, 0, 0};
int CureentShowing = 0;

void Setalltestzero() {
  //  Serial.println(sizeof(chekTest));
  //  int arrsize = sizeof(chekTest) / sizeof(chekTest[0]);
    for (int i = 0; i < MAX_Event_DEFINE; i++) {
        EventsStruct[i].Value = 0;
   //     printf_New("%d,", i);

    }
  //  printf_New("\n,", 0);

}

void Setalltestzero1() {
    EventsStruct[0].Value = 0;
    EventsStruct[1].Value = 0;
    EventsStruct[2].Value = 1;
    EventsStruct[3].Value = 0;
    EventsStruct[4].Value = 0;
    EventsStruct[5].Value = 0;
    EventsStruct[6].Value = 0;
    EventsStruct[7].Value = 0;
    EventsStruct[8].Value = 0;
    EventsStruct[9].Value = 0;
    EventsStruct[10].Value = 0;
    EventsStruct[11].Value = 0;
    EventsStruct[12].Value = 0;

}

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
    for(int i=0;i<MAX_Event_DEFINE;i++)
//        if(EventsStruct[i].Value==true)
//            printf_New("[%d=%d,%d]",i,EventsStruct[i].ErrorShowOnScreen,EventsStruct[i].Value==true);
    while ((k < MAX_Event_DEFINE) && (FoundNext == false)) {
        if (EventsStruct[j].ErrorShowOnScreen == true && EventsStruct[j].Value==true) {
            CureentShowing = j;
            FoundNext = true;
        }
        j = (j < (MAX_Event_DEFINE - 1)) ? ++j : 0;
        k++;
    }
    if (FoundNext == false)CureentShowing = last_CurrentShow;
    if (CureentShowing >= 0)
        if (EventsStruct[CureentShowing].Value == false ||EventsStruct[CureentShowing].ErrorShowOnScreen == false)CureentShowing = -1;

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

void InitializeEvents() {

    for (int i = 0; i < MAX_Event_DEFINE; i++) {
        EventsStruct[i].SaveOnFile = true;
        EventsStruct[i].EventNegativePositive = true;
        EventsStruct[i].ErrorShowOnScreen = false;
        for (int k = 0; k < (sizeof(EventErrors) / EventErrors[0]); k++) {
            if (EventErrors[k] == i)
                EventsStruct[i].ErrorShowOnScreen = true;
        }
    }
    EventsStruct[StrongDCMagneticFieldDetected].EventNegativePositive = false;
}

void setEvent( int EventNumber,boolean SetValue) {
    if (EventNumber >= MAX_Event_DEFINE || EventNumber < 1) return;
//   printf_New("{1,%d,%d}",SetValue, EventNumber);
    if (EventsStruct[EventNumber].Value == SetValue) return;
 //   printf_New("{2,%d,%d}",SetValue, EventNumber);
    if ((SetValue == true && EventsStruct[EventNumber].EventNegativePositive == true) ||
        (SetValue == false && EventsStruct[EventNumber].EventNegativePositive == false)) {
  //      printf_New("{3,%d,%d}",SetValue, EventNumber);
        EventsStruct->ErrorValue = true;
        if (EventsStruct[EventNumber].SaveOnFile)
            SaveEventsFile(EventNumber);

        //  myGLCD.print("zEAXO ~{A^AG", 100, 250);
    } else {
        EventsStruct->ErrorValue = false;

    }

    EventsStruct[EventNumber].Value = SetValue;
}

void ReadEvents() {
    EventsStruct[PowerDown].EventNegativePositive = true; // check rise Event in negative or positive mode
    EventsStruct[PowerDown].Value = false;
    EventsStruct[PowerDown].SaveOnFile = false;

}

