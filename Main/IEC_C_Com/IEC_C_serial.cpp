//------------------------------------------------------------------------------
// Copyright (C) 2011, Robert Johansson, Raditex AB
// All rights reserved.
//
// rSCADA
// http://www.rSCADA.se
// info@rscada.se
//
//------------------------------------------------------------------------------

//#include <unistd.h>



#include <errno.h>
#include <string.h>

#include "IEC_C.h"
//#include "Serial.h"
//2048
#define PACKET_BUFF_SIZE 256
//extern char LoggSerial[20][100];
//extern int  LogerCount;

//------------------------------------------------------------------------------
/// Set up a serial connection handle.
//------------------------------------------------------------------------------
/*goker
  SoftwareSerial *
  IEC_C_serial_connect(char *device)
  {
    SoftwareSerial *handle= &Serial;
    DebugSerial..begin(2400);



    // Use blocking read and handle it by serial port VMIN/VTIME setting
    if ((handle->fd = open(handle->device, O_RDWR | O_NOCTTY)) < 0)
    {
        fprintf(stderr, "%s: failed to open tty.", __PRETTY_FUNCTION__);
        return NULL;
    }

    memset(&(handle->t), 0, sizeof(handle->t));
    handle->t.c_cflag |= (CS8|CREAD|CLOCAL);
    handle->t.c_cflag |= PARENB;

    // No received data still OK
    handle->t.c_cc[VMIN]  = 0;

    // Wait at most 0.2 sec.Note that it starts after first received byte!!
    // I.e. if CMIN>0 and there are no data we would still wait forever...
    //
    // The specification mentions link layer response timeout this way:
    // The time structure of various link layer communication types is described in EN60870-5-1. The answer time
    // between the end of a master send telegram and the beginning of the response telegram of the slave shall be
    // between 11 bit times and (330 bit times + 50ms).
    //
    // For 2400Bd this means (330 + 11) / 2400 + 0.05 = 188.75 ms (added 11 bit periods to receive first byte).
    // I.e. timeout of 0.2s seems appropriate for 2400Bd.

    handle->t.c_cc[VTIME] = 2; // Timeout in 1/10 sec

    cfsetispeed(&(handle->t), B2400);
    cfsetospeed(&(handle->t), B2400);

  #ifdef IEC_C_SERIAL_DEBUG
    printf("%s: t.c_cflag = %x\n", __PRETTY_FUNCTION__, handle->t.c_cflag);
    printf("%s: t.c_oflag = %x\n", __PRETTY_FUNCTION__, handle->t.c_oflag);
    printf("%s: t.c_iflag = %x\n", __PRETTY_FUNCTION__, handle->t.c_iflag);
    printf("%s: t.c_lflag = %x\n", __PRETTY_FUNCTION__, handle->t.c_lflag);
  #endif

    tcsetattr(handle->fd, TCSANOW, &(handle->t));

    return handle;
  }
*/
//------------------------------------------------------------------------------
// Set baud rate for serial connection
//------------------------------------------------------------------------------
int
IEC_C_serial_set_baudrate(int baudrate) {
    //  handle->begin(baudrate);
    if (baudrate < 0)
        return -1;
    IF_SERIAL_DEBUG(printf_New("%s:New badRate %c \n", __PRETTY_FUNCTION__,baudrate));

    IECuseSerial.flush();   // wait for send buffer to empty
    delay(20);    // let last character be sent
    IECuseSerial.end();
    switch (baudrate) {
        case '0':
            IECuseSerial.begin(300, SERIAL_7E1);
            IF_SERIAL_DEBUG(printf_New("%s:change badRate %c \n", __PRETTY_FUNCTION__,baudrate));
            break;
        case '1':
            IECuseSerial.begin(600, SERIAL_7E1);
            IF_SERIAL_DEBUG(printf_New("%s:change badRate %c \n", __PRETTY_FUNCTION__,baudrate));
            break;
        case '2':
            IECuseSerial.begin(1200, SERIAL_7E1);
            break;
        case '3':
            IECuseSerial.begin(2400, SERIAL_7E1);
            break;
        case '4':
            IECuseSerial.begin(4800, SERIAL_7E1);
            break;
        case '5':
            IECuseSerial.begin(9600, SERIAL_7E1);
            IF_SERIAL_DEBUG(printf_New("%s:change badRate %c \n", __PRETTY_FUNCTION__,baudrate));
            break;
        case '6':
            IECuseSerial.begin(19200, SERIAL_7E1);
            break;
        default:
            return -1; // unsupported baudrate
    }
    delay(100);

    return 0;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int IEC_C_serial_disconnect(HardwareSerial *handle) {
    handle->end();
    /*goker   if (handle == NULL)
       {
           return -1;
       }

       close(handle->fd);

       free(handle);
    */
    return 0;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int
IEC_C_serial_send_frame(IEC_C_frame *frame) {
//    uint8_t buff[PACKET_BUFF_SIZE];
    int len, ret;
    // char Tmsg[100];

    IF_SERIAL_DEBUG(printf_New("%s: Entered \n", __PRETTY_FUNCTION__));

    if (frame == NULL) {
        IF_SERIAL_DEBUG(printf_New("%s: Frame == NULL \n", __PRETTY_FUNCTION__));
        return -1;
    }
    uint8_t *Nbuff = (uint8_t *) malloc(1);
    IF_SERIAL_DEBUG(printf_New("\n%%%%%%%%%   1 %s: %d:\n ", __PRETTY_FUNCTION__, Nbuff));

    if ((len = IEC_C_frame_pack(frame, Nbuff, 0 /*sizeof(buff)*/)) <= -1) {
        IF_SERIAL_DEBUG(printf_New("IEC_C_serial_send_frame: IEC_C_frame_pack failed\n",0));
        free(Nbuff);
        return -1;
    }
    IF_SERIAL_DEBUG(printf_New("\n%%%%%%%%%    2 %s: %d:\n ", __PRETTY_FUNCTION__, Nbuff));
    //#ifdef IEC_C_SERIAL_DEBUG
    // if debug, dump in HEX form to stdout what we write to the serial port
    IF_SERIAL_DEBUG(printf_New("%s: Dumping IEC frame [%d bytes]:\n ", __PRETTY_FUNCTION__, len));
    for (int i = 0; i < len; i++) {
        IF_SERIAL_DEBUG(printf_New("%02X, ", Nbuff[i]));
    }
    IF_SERIAL_DEBUG(printf_New("\n", 0));
    for (int i = 0; i < len; i++) {
        IF_SERIAL_DEBUG(printf_New("%c, ", Nbuff[i]));
    }

    IF_SERIAL_DEBUG(printf_New("\n ---------> IN writing , len=%d <------------ \n", len));

    //#endif
    for (int j = 0; j < len; j++)
        IECuseSerial.write(Nbuff[j]);
    /*  if ((ret = IECuseSerial.write(Nbuff, len)) == len) {
          //
          // call the send event function, if the callback function is registered
          //
          if (_IEC_C_send_event)
              _IEC_C_send_event(IEC_C_HANDLE_TYPE_SERIAL, Nbuff, len);
      } else {
          IF_SERIAL_DEBUG(printf_New("%s: Failed to write frame to socket (ret = %d: )\n", "IEC_C_serial_send_frame", ret));

          //       return -1;
      }
  */
    //
    // wait until complete frame has been transmitted
    //
    //  handle->flush();
    free(Nbuff);
    return 0;
}

//------------------------------------------------------------------------------
//
int SerielTesSendFram() {
    IECuseSerial.write("WWWWWWWWWWWWWWWWWW", 10);

}

//------------------------------------------------------------------------------
int
IEC_C_serial_recv() {
    uint8_t buff[PACKET_BUFF_SIZE];
    int len, remaining, nread, timeouts;


    //    printf_P(PSTR("%s: Entered \n"), __PRETTY_FUNCTION__);
    IF_SERIAL_DEBUG(printf_New("%s: Entered \n", __PRETTY_FUNCTION__));


    memset(buff, 0, sizeof(buff));

    //
    // read data until a packet is received
    //
    remaining = 1; // start by reading 1 byte
    len = 0;
    timeouts = 0;
    //   printf_P(PSTR("%s: Entered3 \n"), __PRETTY_FUNCTION__);
    IF_SERIAL_DEBUG(printf_New("%s: Entered3 \n", __PRETTY_FUNCTION__));
    unsigned long start_millis;
    start_millis = millis();

    do {
        //       printf_P(PSTR("%s: Attempt to read %d bytes [len = %d]\n"), __PRETTY_FUNCTION__, remaining, len);
        IF_SERIAL_DEBUG(printf_New("%s: Attempt to read %d bytes [len = %d]\n", __PRETTY_FUNCTION__, remaining, len));


        while (IECuseSerial.available() > 0 && len < PACKET_BUFF_SIZE && ((millis() - start_millis) < 700)) {
            buff[len] = IECuseSerial.read();
            len++;

            start_millis = millis();
            IF_SERIAL_DEBUG(printf_New("millis = %ld", millis()));

        }

    } while ((remaining = IEC_C_parse(buff, len)) > 0 && ((millis() - start_millis) < 500));
    if(remaining == DataPackRecieveOK)
        IEC_C_CheckModeTimeOut(true);
    if (len == 0) {
        // No data received
        return -1;
    }

    //
    // call the receive event function, if the callback function is registered
    //
    if (_IEC_C_recv_event)
        _IEC_C_recv_event(IEC_C_HANDLE_TYPE_SERIAL, buff, len);

    if (remaining != 0) {
        // Would be OK when e.g. scanning the bus, otherwise it is a failure.
        IF_SERIAL_DEBUG(printf_New("%s: M-Bus layer failed to receive complete data.\n", __PRETTY_FUNCTION__));

        return -2;
    }

    if (len == -1) {
        IF_SERIAL_DEBUG(printf_New("%s: M-Bus layer failed to parse data.\n", __PRETTY_FUNCTION__));

        return -1;
    }
    /*        sprintf(Tmsg,"len =%d , remaining = %d ,buff[0]\n",  len, remaining,buff[0]);
            DebugSerial.print(Tmsg);
            for(int i=0;i<len;i++)
            {
                sprintf(Tmsg,"%.2X ", buff[i] & 0xFF);
                DebugSerial.print(Tmsg);

              }*/
    return 0;
}



