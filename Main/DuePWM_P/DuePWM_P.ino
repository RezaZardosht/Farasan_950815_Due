#include "DuePWM.h"


#define PWM_FREQ1  2
#define PWM_FREQ2  5000
uint32_t pwm_duty = 127; // 50% duty cycle

// DuePWM can be instantiated using a default constrcutor like this:
// DuePWM pwm();
// OR
// DuePWM pwm;
// In these cases it will use the default values of PWM_FREQUENCY
// defined in variant.h which is currently 1kHz and is equivalent to:
// DuePWM pwm(PWM_FREQUENCY, PWM_FREQUENCY);
//DuePWM pwm( PWM_FREQ1, PWM_FREQ2 );

int button_52, button_48, button_46, button_2, button_11;
int button_11_prevVal = false;
boolean   out_49 = false, out_47 = false, out_45 = false, out_43 = false, out_51 = false;
char inputString[100];

void setup__()
{
  pinMode(52, INPUT);
  pinMode(48, INPUT);
  pinMode(46, INPUT);
  pinMode(2, INPUT);
  pinMode(11, INPUT);
  pinMode(49, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(51, OUTPUT);

  Serial.begin(115200);
  //////////////////////////pwm.setFreq1( PWM_FREQ1 );
 //////////////////////////// pwm.setFreq2( PWM_FREQ2 );
 ///////////////////////////// pwm.setFreq1( 200);
 /////////////////////////////// pwm.setFreq2( 4000 );

  // I have done away with pwm_set_resolution in the interest of speed
  // The arduino standard seems to be 8 bit resolution on all platforms
  // so in the interest of speed and efficiency, just assume it's 8 bit.
  // Set PWM Resolution
  //pwm_set_resolution(16);

  // Setup PWM Once (Up to two unique frequencies allowed
  //-----------------------------------------------------
 /////////////////////////////////////// pwm.pinFreq1( 9 );  // Pin 6 freq set to "pwm_freq1" on clock A

  // Write PWM Duty Cycle Anytime After PWM Setup
  //-----------------------------------------------------

  // Pin 6 should now show 10kHz and 7, 8, and 9, 20kHz respectively.
  // Duty cycles remain unchanged.

  //   delay(30000);  // 30sec Delay; PWM signal will stream new freq

  // Force PWM Stop On All Pins
  //-----------------------------
  /*   pwm.stop( 6 );
     pwm.stop( 7 );
     pwm.stop( 8 );
     pwm.stop( 9 );*/
}

void loop__()
{
  char msg[200];
  button_52 = digitalRead(52);//52
  button_48 = digitalRead(48);//48
  button_46 = digitalRead(46);//46
  button_2 = digitalRead(2);//2
  button_11 = digitalRead(11);
  sprintf(msg, "52=%d,48=%d,46=%d,2=%d,11=%d,49=%d,inputstring[0]=%c", button_52, button_48, button_46, button_2, button_11, out_51, inputString[0]);
   Serial.println(msg);
  if (inputString[0] == '1')
  {
    out_49 = !out_49;
  }
  if (inputString[0] == '2')
    out_47 = !out_47;
  if (inputString[0] == '3')
    out_45 = !out_45;
  if (inputString[0] == '4')
    out_43 = !out_43;
  if (inputString[0] == '5')
    out_51 = !out_51;
  inputString[0] = '0';
  digitalWrite(49, out_49);
  digitalWrite(47, out_47);
  digitalWrite(45, out_45);
  digitalWrite(43, out_43);
  digitalWrite(51, out_51);
  // pwm.setFreq1( 200);
  //  pwm.setFreq2( 4000 );

  //  pwm_duty = 125;
 //////////////////////////// pwm.pinDuty( 9, pwm_duty ); // 100% duty cycle on Pin 6
  if ( button_11_prevVal != button_11)
  {
    button_11_prevVal = button_11 ;
    if (button_11 == 0)
      pwm_duty = (pwm_duty <= 200) ? (pwm_duty + 50) : 100;
  }
  /*  delay(1000);
    pwm.pinDuty( 9, 150 );  // 100% duty cycle on Pin 7
    delay(1000);
     pwm.pinDuty( 9, 200 );  // 100% duty cycle on Pin 8
    delay(1000);
    pwm.pinDuty( 9, 250 );  // 100% duty cycle on Pin 9
  */    delay(100);
}
void serialEvent___________() {
  int countInp = 0;
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString[countInp++] = inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    //    if (inChar == '\n') {
    //      stringComplete = true;
    //   }
  }
}


