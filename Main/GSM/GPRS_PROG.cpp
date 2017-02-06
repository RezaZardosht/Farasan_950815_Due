#include "SIM800.h"
//#include "../Loop.h"
CGPRS_SIM800 gprs;
void Setup_GPRS(CGPRS_SIM800 gprs_)
{
 return;
  unsigned long GSMMicroSecDiff = millis();
  boolean gprs_init = false;
  Serial.println("SIM800 TEST");
  // ShowMessageOnStartup("SIM800 TEST");
  for (int i = 1; i < 10; i++) {
    GSMMicroSecDiff = millis();
    Serial.print("Resetting...");
      //   ShowMessageOnStartup("Resetting...");
    gprs_init = gprs_.init();
    while (!gprs_init   && (GSMMicroSecDiff+ 10000) > millis()) {
      gprs_init = gprs_.init();
      // gprs_init =;
      Serial.write('.');
      Serial.print(millis());
      Serial.write(',');
      Serial.println(GSMMicroSecDiff);
    }
    if (!gprs_init ) {
      Serial.println("Error init network ");
        //   ShowMessageOnStartup("Error init network ");
      Serial.println(i);
      continue;
    }
    if (!gprs_init ) return;
    Serial.println("OK");
      //  ShowMessageOnStartup("OK");
    Serial.print("Setting up network...");
      //   ShowMessageOnStartup("Setting up network...");

    byte ret = gprs_.setup(APN);
    if (ret == 0)
      break;
    Serial.print("Error code:");
      //   ShowMessageOnStartup("Error code:");
    Serial.println(ret);
    Serial.println(gprs_.buffer);
  }
  Serial.println("OK");
    //   ShowMessageOnStartup("OK network");
  delay(3000);

  if (gprs_.getOperatorName()) {
    Serial.print("Operator:");
      //    ShowMessageOnStartup("Operator:");
    Serial.println(gprs_.buffer);
      //  ShowMessageOnStartup(gprs_.buffer);
  }
  int ret = gprs_.getSignalQuality();
  if (ret) {
    Serial.print("Signal:");
    Serial.print(ret);
    Serial.println("dB");
  }
  Serial.print("checkSMS()");
  for (; ; )
  {
    if (gprs_.httpInit()) break;
    Serial.println(gprs_.buffer);
    gprs_.httpUninit();
    delay(1000);
  }
  delay(3000);
}


