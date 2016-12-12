#include "loop.h"
#include "Loop.h"


extern uint8_t BigFont[];
extern UTFT myGLCD;
int CurrentLevelShowOnScreen = 0;
int MaxNumOfCurrentError = 0;
const int EventErrors[] = {PowerDown, PowerUp, ApplicationError, StrongDCMagneticFieldDetected, MeterCoverRemoved,
                           PermittedVolumeThresholdEexceeded, ElectricalCurrentDisconnected, TamperedWaterFlowDetected,
                           MBusDisconnected};



EventsStructDefine EventsStruct[MAX_Event_DEFINE];
unsigned long WaitTimeShowErrors=0;
void ShowErrorsOnScreen() {


    if(get_MilliSecondDiff(WaitTimeShowErrors)<5000){
        return;
    }
    WaitTimeShowErrors = millis();
    CurrentLevelShowOnScreen = (CurrentLevelShowOnScreen<(sizeof(EventErrors)-1))?++CurrentLevelShowOnScreen:0;
    char msg[100];
    sprintf(msg, "      ");
    myGLCD.setFont(BigFont);
    myGLCD.print(msg, 100, 250);

     for (int i = 0; i < MAX_Event_DEFINE; i++) {
        if(EventsStruct[i].ErrorValue && CurrentLevelShowOnScreen==i){
            sprintf(msg, "%d:éâU", i);
            myGLCD.setFont(BigFont);
        }

    }
}
void InitializeEvents() {

    for (int i = 0; i < MAX_Event_DEFINE; i++) {
        EventsStruct[i].SaveOnFile = true;
        EventsStruct[i].EventNegativePositive = true;
        EventsStruct[i].ErrorShowOnScreen = false;
        for (int k = 0; k < sizeof(EventErrors); k++) {
            if (EventErrors[k] == i)
                EventsStruct[i].ErrorShowOnScreen = true;
        }
    }
    EventsStruct[StrongDCMagneticFieldDetected].EventNegativePositive = false;
}

void setEvent(boolean SetValue, int EventNumber) {
    if (EventNumber >= MAX_Event_DEFINE || EventNumber < 1) return;

    if (EventsStruct[EventNumber].Value == SetValue) return;
    if ((SetValue == true && EventsStruct[EventNumber].EventNegativePositive == true) ||
        (SetValue == false && EventsStruct[EventNumber].EventNegativePositive == false)) {
        EventsStruct->ErrorValue=true;
        if (EventsStruct[EventNumber].SaveOnFile)
            SaveEventsFile(EventNumber);

          //  myGLCD.print("zEAXO ~{A^AG", 100, 250);
    } else {
        EventsStruct->ErrorValue=false;

    }

    EventsStruct[EventNumber].Value = SetValue;
}

void ReadEvents() {
    EventsStruct[PowerDown].EventNegativePositive = true; // check rise Event in negative or positive mode
    EventsStruct[PowerDown].Value = false;
    EventsStruct[PowerDown].SaveOnFile = false;

}

