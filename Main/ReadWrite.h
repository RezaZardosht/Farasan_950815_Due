#ifndef READWRITE_H
#define READWRITE_H

#include <SPI.h>
#include <SD.h>

template<typename T>
int SDFile_Read(File sdfile, T &t, int RecNum);

template<typename T>
T &SDFile_Write(File sdfile, T &t);

template<typename T>
T &SDFile_Write(File sdfile, T &t) {
    byte *buff = (byte *) &t; // to access example as bytes
    int count;
    if (!sdfile) {
        Serial.print(F("Unable to open for write: "));
    }
  //  byte b2[sizeof(T)]; // create byte array to store the struct
  //  memcpy(b2, buff, sizeof(T)); // copy the struct to the byte array
    sdfile.seek( sdfile.size());
//    count = sdfile.write(b2, sizeof(T));
    count = sdfile.write(buff,sizeof(T));

    if (count != sizeof(T)) {
        Serial.print(F("Unable to write the example block to "));
        Serial.print(count, DEC);
        Serial.println(F(" bytes written."));
    }

    // ========== SD close file ===========================================
}

template<typename T>
int SDFile_Read(File sdfile, T &t, int RecNum) {
    byte *ptr = (byte *) &t; // to access copyto as bytes
  //  byte b2[sizeof(T)]; // create byte array to store the struct
    //  memcpy(b2, buff, sizeof(T)); // copy the struct to the byte array
    sdfile.seek(RecNum * sizeof(T));
    if (sdfile.position() + sizeof(T) > sdfile.size()) {

        printf_New("sdfile.position()=%d , sizeof(T)=%d  sdfile.size()=%d \n",sdfile.position() , sizeof(T) , sdfile.size());

        return -1;
    }

    int count;
  //  for (count = 0; count < sizeof(T); count++) {
        if (sdfile.available()) {
            sdfile.read(ptr,sizeof(t));
        } else {
            Serial.print(F("Unable to read the example block in "));
            Serial.print(count, DEC);
            Serial.println(F(" bytes read."));
        }
   // }
  //  memcpy(ptr, b2, sizeof(T));
    return 1;
}

void SavetodayCsvFile(char filename[20], char rowline[100]);


#endif
