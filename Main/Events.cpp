#include "loop.h"


extern uint8_t  BigFont[] ;
extern UTFT myGLCD;
struct EventsStructDefine {

  boolean EventNegativePositive;// check rise Event in negative or positive mode
  boolean Value;
  boolean SaveOnFile;

} EventsStruct[MAX_Event_DEFINE];


void setEvent(boolean SetValue , int EventNumber) {
  if(EventNumber>=MAX_Event_DEFINE || EventNumber<0  ) return;
  char msg[100];
  if (EventsStruct[ EventNumber].Value == SetValue) return;
  if ((SetValue == true && EventsStruct[ EventNumber].EventNegativePositive == true) || (SetValue == false && EventsStruct[ EventNumber].EventNegativePositive == false)) {
    if (EventsStruct[ EventNumber].SaveOnFile)
      SaveEventsFile( EventNumber);
    sprintf(msg,"%d:éâU", EventNumber);  
    myGLCD.setFont(BigFont);
  //  myGLCD.print("zEAXO ~{A^AG", 100, 250);
  }
  else{
        sprintf(msg,"      ", EventNumber);  
    myGLCD.setFont(BigFont);
    myGLCD.print(msg, 100, 250);

    }
  
  EventsStruct[ EventNumber].Value = SetValue;
}
void ReadEvents()
{
  EventsStruct[PowerDown].EventNegativePositive = true; // check rise Event in negative or positive mode
  EventsStruct[PowerDown].Value = false;
  EventsStruct[PowerDown].SaveOnFile = false;

}

