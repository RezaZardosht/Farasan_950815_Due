#include "loop.h"
//#undef SERIAL_DEBUG
//#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#define IF_SERIAL_DEBUG(x) ({x;})
#else
#define IF_SERIAL_DEBUG(x)
#endif
#define MaxMonthlySaveFileRecord 24
#define MaxDailySaveFileRecord 62
#define MaxHourlySaveFileRecord 24*62
#define MaxEventsSaveFileRecord 100

/*void SavetodayCsvFile(char filename[20],char rowline[100]) {

  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.println(rowline);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial__println("error opening datalog");
  }

  }

  /*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/
template<typename T>
T *SaveFile_StructType_new() {
    T *data;

    if ((data = (T *) malloc(sizeof(T))) == NULL) {
        return NULL;
    }
    data->Next = NULL;

    return data;
};

template<typename T>
void SaveFile_StructType_free(T *data) {
    if (data) {
        T *Next = (T *) data->Next;
        free(data);
        if (Next)
            SaveFile_StructType_free<T>(Next);
    }
    delay(100);
};

char *stringFormat(char *msg, struct EventsSaveFile_Struct MdailyStruct) {
    char mmsg[100];
    memcpy(mmsg, MdailyStruct.today, sizeof(MdailyStruct.today));
    mmsg[sizeof(MdailyStruct.today)]=0;
    sprintf(msg, "%s %d\r\n", mmsg, MdailyStruct.Events);
    return msg;
}

char *stringFormat(char *msg, struct HourlySaveFile_Struct MdailyStruct) {
    char mmsg[100];
    memcpy(mmsg, MdailyStruct.today, sizeof(MdailyStruct.today));
    mmsg[sizeof(MdailyStruct.today)]=0;
    sprintf(msg, "%s %02X %lu\r\n", mmsg, MdailyStruct.profile, MdailyStruct.Duration_Volume);
    return msg;
}

char *stringFormat(char *msg, struct DailySaveFile_Struct MdailyStruct) {
    char mmsg[100];
    memcpy(mmsg, MdailyStruct.today, sizeof(MdailyStruct.today));
    mmsg[sizeof(MdailyStruct.today)]=0;
    sprintf(msg, "%s,%02X,%lu,%lu,%lu,%lu\r\n", mmsg, MdailyStruct.profile,
            MdailyStruct.Duration_Volume,
            MdailyStruct.TotalDuration_Charzh, MdailyStruct.V_MaxFlowIn24Hour, MdailyStruct.V_PUmpTotalHour);
    return msg;
}

char *stringFormat(char *msg, struct MonthlySaveFile_Struct MdailyStruct) {
    char mmsg[100];
    memcpy(mmsg, MdailyStruct.today, sizeof(MdailyStruct.today));
    mmsg[sizeof(MdailyStruct.today)]=0;
    sprintf(msg, "%s,%02X,%lu,%lu,%lu\r\n",mmsg, MdailyStruct.profile, MdailyStruct.Duration_Volume,
            MdailyStruct.TotalDuration_Charzh, MdailyStruct.V_PUmpTotalHour);
    return msg;
}


void SD_renameFile(char *PrevFileName, char *NewFileName) {
    if (!SD.exists(PrevFileName))return;
    if (SD.exists(NewFileName))SD.remove(NewFileName);
    byte b2;

    File NewFile;
    NewFile = SD.open(NewFileName, FILE_WRITE);
    File PrevFile;
    PrevFile = SD.open(PrevFileName, FILE_READ);
    for (int count = 0; count < PrevFile.size(); count++) {
        if (PrevFile.available()) {
            b2 = PrevFile.read();
            NewFile.write(b2);
        } else {
            Serial.print(F("Unable to read the example block in "));
            Serial.print(count, DEC);
            Serial.println(F(" bytes read."));
        }
    }
    NewFile.close();
    PrevFile.close();
    SD.remove(PrevFileName);


}

template<typename T>
T &SaveFileOnSD(T &TodaySaveFile, char *FileName, int MaxRow) {
    char MMg[20] = "00000000";
    int RecNo = 0;
    T *MdailyStruct;
    int RetSdfile_read = 1;
 //    MdailyStruct = SaveFile_StructType_new<T>();
    File myFile = SD.open(FileName, FILE_WRITE);
    if (myFile) {
        unsigned long MaxDailySaveFileThatAllow = (MaxRow) * sizeof(T);
        if (myFile.size() > MaxDailySaveFileThatAllow) {
            myFile.close();
            if (SD.exists("Tmpdaily.log"))SD.remove("Tmpdaily.log");
            SD_renameFile(FileName, "Tmpdaily.log");
            File TmpmyFile;
            TmpmyFile = SD.open("Tmpdaily.log", FILE_READ);
            myFile = SD.open(FileName, FILE_READ);
            RecNo = 0;
            RetSdfile_read = SDFile_Read<T>(TmpmyFile, *MdailyStruct, RecNo++);
            RetSdfile_read = SDFile_Read<T>(TmpmyFile, *MdailyStruct, RecNo++);
            while (RetSdfile_read != NULL && RecNo < 100) {
                SDFile_Write<T>(myFile, *MdailyStruct);
                RetSdfile_read = SDFile_Read<T>(TmpmyFile, *MdailyStruct, RecNo++);
            }
            TmpmyFile.close();
            if (SD.exists("Tmpdaily.log"))SD.remove("Tmpdaily.log");
        }
        SDFile_Write<T>(myFile, TodaySaveFile);

        IF_SERIAL_DEBUG(Serial.print("Writing to  "));
        IF_SERIAL_DEBUG(Serial.print(FileName));
        myFile.close();
        IF_SERIAL_DEBUG(Serial.println("  done."));
    } else {
        // if the file didn't open, print an error:
        IF_SERIAL_DEBUG(Serial.print("error opening  "));
        IF_SERIAL_DEBUG(Serial.println(FileName));
    }
    char SprinMsg[200];

//    stringFormat(SprinMsg,TodaySaveFile);
//    Serial.print("show by save File");
//    Serial.println(SprinMsg);

    SaveFile_StructType_free<T>(MdailyStruct);

}

void SaveDailyFile(StructTotalValues TotalValues) {
    struct DailySaveFile_Struct MdailyStruct;
    char *Msg;
    if (!(Msg = (char *) malloc(20)))
        return;
    strcpy(MdailyStruct.today, GetStrCurrentDay(Msg));
    MdailyStruct.profile = GetCharCurrentProfile();
    free(Msg);
    MdailyStruct.V_MaxFlowIn24Hour = TotalValues.V_MaxFlowIn24Hour;
    MdailyStruct.TotalDuration_Charzh = TotalValues.TotalDuration_Charzh;
    MdailyStruct.Duration_Volume = TotalValues.Duration_Volume;
    MdailyStruct.V_PUmpTotalHour = TotalValues.UsedHourPump_CountBySec;
    SaveFileOnSD<DailySaveFile_Struct>(MdailyStruct, "dailyLog.log", MaxDailySaveFileRecord);
}

void SaveMonthlyFile(StructTotalValues TotalValues) {
    struct MonthlySaveFile_Struct MonthlyStruct;
    char *Msg;
    if (!(Msg = (char *) malloc(20)))
        return;
    strcpy(MonthlyStruct.today, GetStrCurrentDay(Msg));
    MonthlyStruct.profile = GetCharCurrentProfile();
    free(Msg);
    MonthlyStruct.TotalDuration_Charzh = TotalValues.TotalDuration_Charzh;
    MonthlyStruct.Duration_Volume = TotalValues.Duration_Volume;
    MonthlyStruct.V_PUmpTotalHour = TotalValues.UsedHourPump_CountBySec;
    SaveFileOnSD<MonthlySaveFile_Struct>(MonthlyStruct, "Monthly.log", MaxMonthlySaveFileRecord);
}

void SaveHourlyFile(StructTotalValues TotalValues) {
    struct HourlySaveFile_Struct HourlyStruct;
    char *Msg;
    if (!(Msg = (char *) malloc(20)))
        return;
    sprintf(Msg, "%s%02d", GetStrCurrentDay(Msg), hour());
  //  Serial.println(Msg);
    memcpy(HourlyStruct.today, Msg, 10);
    free(Msg);
    HourlyStruct.profile = GetCharCurrentProfile();
    HourlyStruct.Duration_Volume = TotalValues.Duration_Volume;

    SaveFileOnSD<HourlySaveFile_Struct>(HourlyStruct, (char *) "Hourly.log", MaxHourlySaveFileRecord);
}

void SaveEventsFile(int Event) {
    struct EventsSaveFile_Struct EventslyStruct;
    char *Msg, *MsgD;
    if (!(Msg = (char *) malloc(20)))
        return;
    if (!(MsgD = (char *) malloc(20)))
        return;
    sprintf(Msg, "%s:%s", GetStrCurrentDay(Msg), GetCurrentStrHour(MsgD));
//    printf_New(Msg, 0);
    memset(EventslyStruct.today, 0, sizeof(EventslyStruct.today));
    memcpy(EventslyStruct.today, Msg, strlen(Msg));
    //   memcpy(EventslyStruct.today, "950101:142002", 14);
    free(Msg);
    free(MsgD);
    EventslyStruct.Events = Event;
    SaveFileOnSD<EventsSaveFile_Struct>(EventslyStruct, (char *) "EvwntLog.log", MaxEventsSaveFileRecord);


}

template<typename T>
T *GetDaileRecords(char *fileName, char *from_date, char *to_date) {

    char MMg[40] = "00000000";
    int RetSdfile_read = 1, RecNo = 0;
    T *MdailyStruct;
    T *MdailyStructNext, *MdailyStructFirst;
    File fin;
    fin = SD.open(fileName, FILE_READ);
    MdailyStruct = SaveFile_StructType_new<T>();
    MdailyStructFirst = SaveFile_StructType_new<T>();
    MdailyStructNext = MdailyStructFirst;

    int Ret = -1;
    RecNo = 0;
    Ret = SDFile_Read<T>(fin, *MdailyStruct, RecNo);
    while (Ret != -1 && RecNo++ < 100) {
        memcpy(MMg, MdailyStruct->today, 8);
        MMg[8] = 0;
        if (strcmp(MMg, from_date) >= 0 && strcmp(MMg, to_date) <= 0)
        {
            MdailyStructNext->Next = MdailyStruct;
            MdailyStruct = SaveFile_StructType_new<T>();
            MdailyStructNext = MdailyStructNext->Next;
        }
        Ret = SDFile_Read<T>(fin, *MdailyStruct, RecNo);
    }
    fin.close();
    MdailyStructNext->Next = NULL;
    MdailyStruct->Next = NULL;
    if (MdailyStruct)
        SaveFile_StructType_free<T>(MdailyStruct);

    return MdailyStructFirst;
}

struct EventsSaveFile_Struct *SerialDemoIECSendEventFile() {
    int RecNo = 0;
    struct EventsSaveFile_Struct *MdailyStruct;
    struct EventsSaveFile_Struct *MdailyStructNext, *MdailyStructFirst;
    MdailyStructFirst = SaveFile_StructType_new<struct EventsSaveFile_Struct>();
    MdailyStructNext = MdailyStructFirst;
//    MdailyStruct = MdailyStructFirst;
    while (RecNo < 100) {
        strcpy(MdailyStructNext->today, "13950110:140315");
        MdailyStructNext->Events = RecNo;
        RecNo++;
        MdailyStruct = MdailyStructNext;
        MdailyStructNext = SaveFile_StructType_new<struct EventsSaveFile_Struct>();
        MdailyStruct->Next = MdailyStructNext;
    }
    strcpy(MdailyStructNext->today, "13950110:140315");
    MdailyStructNext->Events = RecNo;
    return MdailyStructFirst;
}


template<typename T>
struct CharMemoryAlocated *FileSaveStructToChar(T *MdailyStruct_2) {
    struct CharMemoryAlocated *ReadEventFile;
    T *FirsMdailyStruc_2,*TempMdailyStruc_2;
//    MdailyStruct = SerialDemoIECSendEventFile();
     FirsMdailyStruc_2 = MdailyStruct_2;
    TempMdailyStruc_2 = MdailyStruct_2;

    char msg[500];
    int MemoreyRequest = 0;
    while (TempMdailyStruc_2 != NULL) {
        MemoreyRequest = MemoreyRequest + strlen(stringFormat(msg, *TempMdailyStruc_2));
//        Serial.println(msg);
        TempMdailyStruc_2 = TempMdailyStruc_2->Next;
    }

    // MemoreyRequest = MemoreyRequest ;
    if ((ReadEventFile = (struct CharMemoryAlocated *) malloc(sizeof(struct CharMemoryAlocated))) != NULL) {
        ReadEventFile->memory = (char *) malloc(MemoreyRequest);
        ReadEventFile->size = MemoreyRequest;
        TempMdailyStruc_2 = FirsMdailyStruc_2;
        int Npoint = 0;
        while (TempMdailyStruc_2 != NULL) {
            stringFormat(msg, *TempMdailyStruc_2);
            if (Npoint + strlen(msg) <= ReadEventFile->size) {
                memcpy(ReadEventFile->memory + Npoint, msg, strlen(msg));
                Npoint = Npoint + strlen(msg);
//                    for (int i = 0; i < Npoint; i++)     printf_New("<%c>", ReadEventFile->memory[i]);
            }
            TempMdailyStruc_2 = TempMdailyStruc_2->Next;
        }


 //       SaveFile_StructType_free(FirsMdailyStruc_2);
    } else {
        printf_New("could not allocate memory_1", 0);
    }

 //   free(msg);
    return ReadEventFile;

}

template<typename T>void readFileTest(char *filename) {//"HourlyLog.log",EvwntLog.log
    T *MdailyStruct, *MdailyStructFirst;
    MdailyStructFirst = GetDaileRecords<T>((char *) filename, (char *) "00000000",
                                                                      (char *) "99999999");
    if (!MdailyStructFirst->Next) {
        if (MdailyStructFirst)
            SaveFile_StructType_free<T>(MdailyStructFirst);
        printf_New("No Data Found", 0);
        return;
    }
    if (MdailyStructFirst) {
        MdailyStruct = MdailyStructFirst->Next;
        struct CharMemoryAlocated *ReadEventFile = NULL;
            ReadEventFile = FileSaveStructToChar<T>(MdailyStruct);
        if (ReadEventFile) {
            for (int i = 0; i < ReadEventFile->size; i++) printf_New("%c", ReadEventFile->memory[i]);
        }
        if (ReadEventFile->memory)
            free(ReadEventFile->memory);
        if (ReadEventFile)
            free(ReadEventFile);
        if (MdailyStructFirst)
            SaveFile_StructType_free<T>(MdailyStructFirst);
    }
}
void readFileTestEvent(){
    readFileTest<struct EventsSaveFile_Struct>((char *) "EvwntLog.log");
}
void readFileTestHourly(){
    readFileTest<struct HourlySaveFile_Struct>((char *) "Hourly.log");
}
int DemoDate = 0;

void DemoSaveGetEventFile() {
    char MMg[100] = "00000000";
    int RecNo = 0;
    byte *ptr;
    struct EventsSaveFile_Struct *MdailyStruct, *MdailyStructFirst;
    MdailyStruct = SaveFile_StructType_new<struct EventsSaveFile_Struct>();
    MdailyStructFirst = MdailyStruct;
    File dumpFile = SD.open("EvwntLog.log", FILE_WRITE);

    sprintf(MMg, "139501%02d:%02d%02d%02d", DemoDate++, hour(), minute(), second());
    memcpy(MdailyStruct->today, MMg, strlen(MMg));
    MdailyStruct->Events = 101;
    SDFile_Write<struct EventsSaveFile_Struct>(dumpFile, *MdailyStruct);

    memset(MdailyStruct->today, 0, sizeof(MdailyStruct->today));
    MdailyStruct->Events = 0;
    int Ret = 0;
    Ret = SDFile_Read<struct EventsSaveFile_Struct>(dumpFile, *MdailyStruct, 0);
    memset(MMg, 0, 18);
    memcpy(MMg, MdailyStruct->today, 18);
    printf_New("============= %d:%s %d,\n", RecNo++, MMg, MdailyStruct->Events);
    if (dumpFile) {
        RecNo = 0;
        Ret = SDFile_Read<struct EventsSaveFile_Struct>(dumpFile, *MdailyStruct, RecNo);
        while (Ret != -1 && RecNo++ < 10) {
            memcpy(MMg, MdailyStruct->today, 18);
            printf_New("============= %d:%s %d,\n", RecNo, MMg, MdailyStruct->Events);
            Ret = SDFile_Read<struct EventsSaveFile_Struct>(dumpFile, *MdailyStruct, RecNo);

        }
        dumpFile.close();

    } else {
        Serial.println("error opening file");
    }
    SaveFile_StructType_free<struct EventsSaveFile_Struct>(MdailyStructFirst);
    printf_New("=============ENDDEmo,\n",0);
   // delay(1000);
}

char *TypenameS(struct DailySaveFile_Struct t) {
    return "DailySaveFile_Struct";
}

char *TypenameS(struct EventsSaveFile_Struct temp1) {
    return "EventsSaveFile_Struct";
}

char *TypenameS(struct HourlySaveFile_Struct t) {
    return "HourlySaveFile_Struct";
}

char *TypenameS(struct MonthlySaveFile_Struct t) {
    return "StructTotalValues";
}

void writeStructType() {
    struct EventsSaveFile_Struct *MdailyStruct, *FirsMdailyStruct;
    MdailyStruct = SaveFile_StructType_new<struct EventsSaveFile_Struct>();
    printf_New("%s , %s \n", TypenameS(*MdailyStruct), TypenameS(*FirsMdailyStruct));
    SaveFile_StructType_free(MdailyStruct);
}

struct CharMemoryAlocated *GetDailEventRecords(const char *from_day, const char *to_day) {
    struct CharMemoryAlocated *ReadEventFile = NULL;
    struct EventsSaveFile_Struct *MdailyStruct, *MdailyStructFirst;
    MdailyStructFirst = GetDaileRecords<struct EventsSaveFile_Struct>((char *) "EvwntLog.log", (char *) from_day,
                                                                      (char *) to_day);
    if (!MdailyStructFirst->Next) {
        if (MdailyStructFirst)
            SaveFile_StructType_free<struct EventsSaveFile_Struct>(MdailyStructFirst);
        printf_New("No Data Found", 0);
        return NULL;
    }
    if (MdailyStructFirst) {
        MdailyStruct = MdailyStructFirst->Next;
        ReadEventFile = FileSaveStructToChar<struct EventsSaveFile_Struct>(MdailyStruct);
        if (ReadEventFile) {
            for (int i = 0; i < ReadEventFile->size; i++) Serial.print(ReadEventFile->memory[i]);
        }
        if (MdailyStructFirst)
            SaveFile_StructType_free<struct EventsSaveFile_Struct>(MdailyStructFirst);
    }
    return ReadEventFile;
}
struct CharMemoryAlocated *GetHourlyLogFile(const char *from_day, const char *to_day) {
    struct CharMemoryAlocated *ReadEventFile = NULL;
    struct HourlySaveFile_Struct *MdailyStruct, *MdailyStructFirst;
    MdailyStructFirst = GetDaileRecords<struct HourlySaveFile_Struct>((char *) "Hourly.log", (char *) from_day,
                                                                      (char *) to_day);
    if (!MdailyStructFirst->Next) {
        if (MdailyStructFirst)
            SaveFile_StructType_free<HourlySaveFile_Struct>(MdailyStructFirst);
        printf_New("No Data Found", 0);
        return NULL;
    }
    if (MdailyStructFirst) {
        MdailyStruct = MdailyStructFirst->Next;
        ReadEventFile = FileSaveStructToChar<HourlySaveFile_Struct>(MdailyStruct);
        if (ReadEventFile) {
            for (int i = 0; i < ReadEventFile->size; i++) Serial.print(ReadEventFile->memory[i]);
        }
        if (MdailyStructFirst)
            SaveFile_StructType_free<HourlySaveFile_Struct>(MdailyStructFirst);
    }
    return ReadEventFile;
}

void Delete_File(char *FileName){
    if (SD.exists(FileName))SD.remove(FileName);//EvwntLog.log);
}
