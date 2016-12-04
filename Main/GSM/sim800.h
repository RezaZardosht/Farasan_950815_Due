/*************************************************************************
  SIM800 GPRS/HTTP Library
  Distributed under GPL v2.0
  Written by Stanley Huang <stanleyhuangyc@gmail.com>
  For more information, please visit http://arduinodev.com
*************************************************************************/

#include <Arduino.h>

// change this to the pin connect with SIM800 reset pin
#define SIM800_RESET_PIN 7
#define GSMSleepPermit 8
#define con Serial
#define GPRS_NOCOMMAND  0
#define GPRS_COMMAND_WAIT_Return 1
#define GPRS_COMMAND_SEND_SMS 2
#define GPRS_COMMAND_USSD 3

#define MAX_Recieved_Length  200

#define APN "connect"

// change this to the serial UART which SIM800 is attached to
#define SIM_SERIAL Serial3

// define DEBUG to one serial UART to enable debug information output
#define DEBUG Serial

typedef enum {
  HTTP_DISABLED = 0,
  HTTP_READY,
  HTTP_CONNECTING,
  HTTP_READING,
  HTTP_ERROR,
} HTTP_STATES;

typedef struct {
  float lat;
  float lon;
  uint8_t year; /* year past 2000, e.g. 15 for 2015 */
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} GSM_LOCATION;

class CGPRS_SIM800 {
  public:
    CGPRS_SIM800(): httpState(HTTP_DISABLED) {}
    // initialize the module
    bool init();
    // setup network
    byte setup(const char* apn);
    // get network operator name
    bool getOperatorName();
    // check for incoming SMS
    bool checkSMS();
    // get signal quality level (in dB)
    int getSignalQuality();
    // get GSM location and network time
    bool getLocation(GSM_LOCATION* loc);
    // initialize HTTP connection
    bool httpInit();
    // terminate HTTP connection
    void httpUninit();
    // connect to HTTP server
    bool httpConnect(const char* url, const char* args = 0);
    // check if HTTP connection is established
    // return 0 for in progress, 1 for success, 2 for error
    byte httpIsConnected();
    // read data from HTTP connection
    void httpRead();
    // check if HTTP connection is established
    // return 0 for in progress, -1 for error, bytes of http payload on success
    int httpIsRead();
    // send AT command and check for expected response
    byte sendCommand(const char* cmd, unsigned int timeout = 2000, const char* expected = 0);
    // send AT command and check for two possible responses
    byte sendCommand(const char* cmd, const char* expected1, const char* expected2, unsigned int timeout = 2000);
    // toggle low-power mode
    void  gotoSleep() ;
    void  WakeupofSleep();
    char *ReadRemainCharzhe();
    // check if there is available serial data
    void SendCommandType(int CommandType);
    void RecievedSMS();
    void checkRecievdData();
    
      bool available()
      {
        return SIM_SERIAL.available();
      }
      char buffer[ MAX_Recieved_Length-1];
      byte httpState;
private:
      int     CGPRS_COMMAND_TYPE ;
      int  buffer_Counter=0;
      byte checkbuffer(const char* expected1, const char* expected2 = 0, unsigned int timeout = 2000);
      void purgeSerial();
      byte m_bytesRecv;
      uint32_t m_checkTimer;
    };
