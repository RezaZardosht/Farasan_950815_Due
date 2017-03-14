// IF_SERIAL_DEBUG(printf_New("%s: Entered \n", __PRETTY_FUNCTION__));

#include "../loop.h"

#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"

PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);
#elif 0
#include <PN532_HSU.h>
#include <PN532.h>

PN532_HSU pn532hsu(Serial2);
PN532 nfc(pn532hsu);
#else

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>



PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

#endif
//#undef SERIAL_DEBUG
#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#define IF_SERIAL_DEBUG(x) ({x;})
#else
#define IF_SERIAL_DEBUG(x)
#endif
//PN532_HSU pn532hsu(Serial2);
//PN532 nfc(pn532hsu);
//#define DEBUG_RFID true




// The default Mifare Classic key
static const uint8_t KEY_DEFAULT_KEYAB[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

boolean NFC_Read_Permit = false;
boolean NFC_Write_Permit = true;
byte NFC_Sector_Need = 4;
byte NFC_Sector_DATEStart = 5;
byte NFC_Sector_DATEEnd = 6;
byte NFC_Sector_FINISH = 14;
byte NFC_Byte_Need = 2;
uint8_t Last_uid[] = {0, 0, 0, 0, 0, 0, 0};  // Buffer to store the returned UID
boolean NFCInitializeOK = false;

void InitializeNFC() {

    nfc.begin();
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        IF_SERIAL_DEBUG(printf_New("Didn't find PN53x board"));
        return;
    }
    // Got ok data, print it out!
    IF_SERIAL_DEBUG(printf_New("Found chip PN5 %X", (versiondata >> 24) & 0xFF));
    IF_SERIAL_DEBUG(printf_New("Firmware ver. %d", (versiondata >> 16) & 0xFF));
    IF_SERIAL_DEBUG(printf_New(". %d", (versiondata >> 8) & 0xFF, DEC));

    // configure board to read RFID tags
    nfc.SAMConfig();

    NFCInitializeOK = true;
    return;


}

void NFC_Loop(void) {

    if (!NFCInitializeOK) {
        IF_SERIAL_DEBUG(printf_New("%s:NFC not initialize \n", __PRETTY_FUNCTION__));
        return;
    }
    uint8_t success;                          // Flag to check if there was an error with the PN532
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    uint8_t currentblock;                     // Counter to keep track of which block we're on
    bool authenticated = false;               // Flag to indicate if the sector is authenticated
    uint8_t data[NR_BLOCK_OF_LONGSECTOR], *data_ST;                        // Array to store block data during reads

    uint8_t blockBuffer[NR_BLOCK_OF_LONGSECTOR];                  // Buffer to store block contents
    uint8_t blankAccessBits[3] = {0xff, 0x07, 0x80};
    uint8_t idx = 0;
    uint8_t numOfSector = NR_BLOCK_OF_LONGSECTOR;                 // Assume Mifare Classic 1K for now (16 4-block sectors)

    // Keyb on NDEF and Mifare Classic should be the same
    uint8_t keyuniversal[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    char WriteTONFC[100];//rzh
    int readCounter = 0;
    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    boolean ReadingPrevRFID = true;
    if (success) {
        // Display some basic information about the card

        IF_SERIAL_DEBUG(printf_New("Found an ISO14443A card", 0));
        IF_SERIAL_DEBUG(printf_New("  UID Length: ", 0));
        IF_SERIAL_DEBUG(printf_New("%d", uidLength));
        IF_SERIAL_DEBUG(printf_New(" bytes", 0));
        IF_SERIAL_DEBUG(printf_New("  UID Value: ", 0));
        for (uint8_t i = 0; i < uidLength; i++) {
            if (uid[i] != Last_uid[i])
                ReadingPrevRFID = false;
            Last_uid[i] = uid[i];
            IF_SERIAL_DEBUG(printf_New("%X,", uid[i]));
        }
        if (!ReadingPrevRFID) {
            if (uidLength == 4) {
                //nfc.mifareclassic_FormatNDEF();
                //return;
                // We probably have a Mifare Classic card ...
                //      IF_SERIAL_DEBUG(printf_New("Seems to be a Mifare Classic card (4 byte UID)");
                //     if (NFC_Write_Permit)
                {
                    //         for (int i = 0; i < 16; i++)  data[i] = i;
                    //          WriteNFCBlock(uid, uidLength, NFC_Sector_Need, data);
                }
            }
            //    else{IF_SERIAL_DEBUG(printf_New("Ooops ... this doesn't seem to be a Mifare Classic card!");}
            ReadNFCRead(uid, uidLength, NFC_Sector_Need, data);
            //  for (int i = 0; i < 16; i++)
            //    data[i] = data[i] + 1 ;
            longbytes TempLongCharzhe;
            TempLongCharzhe.chars[0] = data[0];
            TempLongCharzhe.chars[1] = data[1];
            TempLongCharzhe.chars[2] = data[2];
            TempLongCharzhe.chars[3] = data[3];
            TempLongCharzhe.longn = TempLongCharzhe.longn + 5000;

/*            IF_SERIAL_DEBUG(printf_New("%d\n", TempLongCharzhe.longn));
            //  EEPROM.get(Add_TotalConf_EEP,  SaveToMemoryStruct);
            //  SaveToMemoryStruct.TotalDuration_Charzh = SaveToMemoryStruct.TotalDuration_Charzh + 100;
            //  SaveTotal(SaveToMemoryStruct);//TotalDuration_Charzh
            data[0] = TempLongCharzhe.chars[0];
            data[1] = TempLongCharzhe.chars[1];
            data[2] = TempLongCharzhe.chars[2];
            data[3] = TempLongCharzhe.chars[3];

            WriteNFCBlock(uid, uidLength, NFC_Sector_Need, data);
            data[0] = '1';
            data[1] = '3';
            data[2] = '9';
            data[3] = '5';
            data[4] = '/';
            data[5] = '0';
            data[6] = '2';
            data[7] = '/';
            data[8] = '0';
            data[9] = '1';

            WriteNFCBlock(uid, uidLength, NFC_Sector_DATEStart, data);

            data[0] = '1';
            data[1] = '3';
            data[2] = '9';
            data[3] = '5';
            data[4] = '/';
            data[5] = '0';
            data[6] = '9';
            data[7] = '/';
            data[8] = '3';
            data[9] = '0';

            WriteNFCBlock(uid, uidLength, NFC_Sector_DATEEnd, data);
            data[0] = 'F';
            data[1] = 'I';
            data[2] = 'N';
            data[3] = 'I';
            data[4] = 'S';
            data[5] = 'H';
            data[6] = 'D';
            data[7] = 'A';
            data[8] = 'T';
            data[9] = 'A';

            WriteNFCBlock(uid, uidLength, NFC_Sector_FINISH, data);
*/
            uint8_t dateStart[NR_BLOCK_OF_LONGSECTOR];
            uint8_t dateEnd[NR_BLOCK_OF_LONGSECTOR];
            delay(10);
            ReadNFCRead(uid, uidLength, NFC_Sector_DATEStart, dateStart);
            delay(10);
            IF_SERIAL_DEBUG(printf_New("RFID ---<dateStart> %s.", dateStart));
            ReadNFCRead(uid, uidLength, NFC_Sector_DATEEnd, dateEnd);
            delay(10);
            IF_SERIAL_DEBUG(printf_New("RFID ---< dateEnd> %s.", dateEnd));
            for (int i = 0; i < 10; i++)data[i] = (int) '\x0';
            ReadNFCRead(uid, uidLength, NFC_Sector_FINISH, data);
            IF_SERIAL_DEBUG(printf_New("RFID ---< FINISH> %s.", data));

            char DateSt[10], DateEn[10];


            if (data[0] == 'F' && data[1] == 'I' && data[2] == 'N' && data[3] == 'I' && data[4] == 'S' &&
                data[5] == 'H' &&
                data[6] == 'D' && data[7] == 'A' && data[8] == 'T' && data[9] == 'A') {
                for (int i = 0; i < 10; i++) {
                    DateSt[i] = (char) dateStart[i];
                    DateEn[i] = (char) dateEnd[i];
                }
                IncreseCharzh(5000, DateSt, DateEn);
            }
            //   IF_SERIAL_DEBUG(printf_New("Ooops....");

            for (int i = 0; i < 16; i++) {
                IF_SERIAL_DEBUG(printf_New("%X,", data[i]));
            }
        }
    } else// could not read RFID
    {
        int n = sizeof(Last_uid) / sizeof(Last_uid[0]);
        for (int i = 0; i < n; i++)
            Last_uid[i] = 0;  // Buffer to store the returned UID
        IF_SERIAL_DEBUG(printf_New("%s:No success read card \n", __PRETTY_FUNCTION__));

    }

    //   ShowDisplayEndREadingRFID();

}

int WriteNFCBlock(uint8_t uid[], uint8_t uidLength, uint8_t SectorRequesttoWrite, uint8_t *data) {
    uint8_t success = 1;                         // Flag to check if there was an error with the PN532
    bool authenticated = false;               // Flag to indicate if the sector is authenticated
    uint8_t blockBuffer[NR_BLOCK_OF_LONGSECTOR];                  // Buffer to store block contents
    uint8_t blankAccessBits[3] = {0xff, 0x07, 0x80};
    uint8_t idx = 0;
    uint8_t numOfSector = NR_BLOCK_OF_LONGSECTOR;                 // Assume Mifare Classic 1K for now (16 4-block sectors)

    // Keyb on NDEF and Mifare Classic should be the same
    uint8_t keyuniversal[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    char WriteTONFC[100];//rzh
    int readCounter = 0;
    uint8_t currentblock_R;
    idx = SectorRequesttoWrite; // the block that we want to write to it
    // Step 1: Authenticate the current sector using key B 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    // Authenticate have done in perivious function
    //  success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, BLOCK_NUMBER_OF_SECTOR_TRAILER(idx), 1, (uint8_t *)KEY_DEFAULT_KEYAB);
    //  if (!success){IF_SERIAL_DEBUG(printf_New("Authentication failed for sector "); IF_SERIAL_DEBUG(printf_New(numOfSector); return; }

    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    if (!success) {
        IF_SERIAL_DEBUG(printf_New("<%s>  success = nfc.readPassiveTargetID = null", __PRETTY_FUNCTION__));
        return 0;
    }
    if (uidLength != 4) {
        IF_SERIAL_DEBUG(printf_New("<%s>  uidLength != 4", __PRETTY_FUNCTION__));
        return 0;
    }
    if (idx < 1 || idx > 15) {
        IF_SERIAL_DEBUG(printf_New("<%s>  error in (idx < 1 || idx > 15)", __PRETTY_FUNCTION__));
        return 0;
    }
    // Step 1: Authenticate the current sector using key B 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    IF_SERIAL_DEBUG(printf_New("write :Start writing", 0));

    success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, BLOCK_NUMBER_OF_SECTOR_TRAILER(idx), 1,
                                                  (uint8_t *) KEY_DEFAULT_KEYAB);
    if (!success) {
        IF_SERIAL_DEBUG(printf_New("<%s> Authentication failed for sector %d", __PRETTY_FUNCTION__, numOfSector));
        return 0;
    }
    memset(blockBuffer, 0x1F, sizeof(blockBuffer));
    for (int i = 0; i < NR_BLOCK_OF_LONGSECTOR; i++)
        blockBuffer[i] = data[i];
    if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer))) {
        IF_SERIAL_DEBUG(printf_New("<%s> Unable to write to sector %d-3", __PRETTY_FUNCTION__, idx));
        return 0;
    }
    if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer))) {
        IF_SERIAL_DEBUG(printf_New("<%s> Unable to write to sector %d-2", __PRETTY_FUNCTION__, idx));
        return 0;
    }
    // memset(blockBuffer, 0x2F, sizeof(blockBuffer));  //write on block number 3
    if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 1, blockBuffer))) {
        IF_SERIAL_DEBUG(printf_New("<%s> Unable to write to sector %d-1", __PRETTY_FUNCTION__, idx));
        return 0;
    }
    // Step 3: Reset both keys to 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    memcpy(blockBuffer, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));
    memcpy(blockBuffer + 6, blankAccessBits, sizeof(blankAccessBits));
    blockBuffer[9] = 0x68;
    memcpy(blockBuffer + 10, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));

    // Step 4: Write the trailer block
    if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)), blockBuffer))) {
        IF_SERIAL_DEBUG(
                printf_New("<%s> Unable to write trailer block of sector %d", __PRETTY_FUNCTION__, numOfSector));
        return 0;
    }
    return 1;

}

int ReadNFCRead(uint8_t uid[], uint8_t uidLength, uint8_t SectorRequesttoRead, uint8_t *data) {
    uint8_t success = 1;                         // Flag to check if there was an error with the PN532
    bool authenticated = false;               // Flag to indicate if the sector is authenticated
    uint8_t blankAccessBits[3] = {0xff, 0x07, 0x80};
    uint8_t idx = 0;
    uint8_t numOfSector = NR_BLOCK_OF_LONGSECTOR;                 // Assume Mifare Classic 1K for now (16 4-block sectors)

    // Keyb on NDEF and Mifare Classic should be the same
    uint8_t keyuniversal[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    char WriteTONFC[100];//rzh
    int readCounter = 0;
    uint8_t currentblock_R;
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    if (!success) {
        IF_SERIAL_DEBUG(printf_New("<%s>  success = nfc.readPassiveTargetID = null", __PRETTY_FUNCTION__));
        return 0;
    }
    if (uidLength != 4) {
        IF_SERIAL_DEBUG(printf_New("<%s>  uidLength != 4", __PRETTY_FUNCTION__));
        return 0;
    }
    if (SectorRequesttoRead < 1 || SectorRequesttoRead > 15) {
        IF_SERIAL_DEBUG(printf_New("<%s>  error in (SectorRequesttoRead < 1 || SectorRequesttoRead > 15)",
                                   __PRETTY_FUNCTION__));
        return 0;
    }
    IF_SERIAL_DEBUG(printf_New("Read :Start reading\n", 0));

    //      for (currentblock = 0; currentblock < 64; currentblock++)
    for (currentblock_R = SectorRequesttoRead * 4; currentblock_R < SectorRequesttoRead * 4 + 3; currentblock_R++) {
        // Check if this is a new block so that we can reauthenticate
        if (nfc.mifareclassic_IsFirstBlock(currentblock_R)) authenticated = false;

        // If the sector hasn't been authenticated, do so first
        if (!authenticated) {
            // Starting of a new sector ... try to to authenticate
            //    IF_SERIAL_DEBUG(printf_New("------------------------Sector ")); IF_SERIAL_DEBUG(printf_New(currentblock_R / 4, DEC)); IF_SERIAL_DEBUG(printf_New("-------------------------");
            if (currentblock_R == 0) {
                // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic (non-NDEF!)
                // or 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 for NDEF formatted cards using key a,
                // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF 0xFF)
                success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, currentblock_R, 1, keyuniversal);
            } else {
                // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic (non-NDEF!)
                // or 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7 for NDEF formatted cards using key a,
                // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF 0xFF)
                success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, currentblock_R, 1, keyuniversal);
            }
            if (success) {
                authenticated = true;
            } else {
                IF_SERIAL_DEBUG(printf_New("Read :Authentication error", 0));
            }
        }
        // If we're still not authenticated just skip the block
        if (authenticated) {
            data[0] = 0x00;
            data[1] = 0x00;
            data[2] = 0x00;
            data[3] = 0x00;
            data[4] = 0x00;
            //  if(currentblock==3)
            //    nfc.mifareclassic_WriteDataBlock(3,data);
            // Authenticated ... we should be able to read the block now
            // Dump the data into the 'data' array
            success = nfc.mifareclassic_ReadDataBlock(currentblock_R, data);
            if (success) {
                // Read successful
                IF_SERIAL_DEBUG(printf_New("Read : Block %d", currentblock_R));
                if (currentblock_R < 10) {
                    IF_SERIAL_DEBUG(printf_New("  ", 0));
                } else {
                    IF_SERIAL_DEBUG(printf_New(" ", 0));
                }
                // Dump the raw data
                IF_SERIAL_DEBUG(printf_New("Read Dump: : Block ", 0));
                nfc.PrintHexChar(data, 16);
            }
            //         else{IF_SERIAL_DEBUG(printf_New("Block "); IF_SERIAL_DEBUG(printf_New(currentblock, DEC); IF_SERIAL_DEBUG(printf_New(" unable to read this block");}
        }
        //        else{IF_SERIAL_DEBUG(printf_New("Block "); IF_SERIAL_DEBUG(printf_New(currentblock, DEC); IF_SERIAL_DEBUG(printf_New(" unable to authenticate");}
    }
    return 1;
}

int Read_RFIDCardData(StructTotalValues *TotalValues) {
    if (!NFCInitializeOK) {
        IF_SERIAL_DEBUG(printf_New("%s:NFC not initialize \n", __PRETTY_FUNCTION__));
        return 0;
    }
    _RFIDCardInform RFIDCardInform__1;
    _RFIDCardInform *RFIDCardInform__ = &RFIDCardInform__1;
    RFIDCardInform__->UsedScharzhe = 0;
    RFIDCardInform__->DateTaarefePassed[0] = 0;
    RFIDCardInform__->DateTaarefePassed[1] = 0;
    RFIDCardInform__->DateTaarefePassed[2] = 0;
    RFIDCardInform__->DateTaarefePassed[3] = 0;
    RFIDCardInform__->Taarefe[0] = 0;
    RFIDCardInform__->Taarefe[1] = 0;
    RFIDCardInform__->Taarefe[2] = 0;
    RFIDCardInform__->Taarefe[3] = 0;
    RFIDCardInform__->CharzheDore[0] = 0;
    RFIDCardInform__->CharzheDore[1] = 0;
    RFIDCardInform__->CharzheDore[2] = 0;
    RFIDCardInform__->CharzheDore[3] = 0;
    int SuccessRead = ReadInformToRFidCard(RFIDCardInform__);
    if (SuccessRead == 0) return 0;
    byte CheckSm;
    char *SumChar;
    SumChar = (char *) malloc(sizeof(RFIDCardInform__));
    memcpy(SumChar, RFIDCardInform__, sizeof(RFIDCardInform__));
    CheckSm=Calc_publicCheckSum(SumChar, sizeof(RFIDCardInform__));
    free(SumChar);
    if(CheckSm != RFIDCardInform__->XorSum){
        IF_SERIAL_DEBUG(printf_New("%s:NFC Xor Sum error \n", __PRETTY_FUNCTION__));
        return 0;
    }
    TotalValues->DateTaarefe[0] = RFIDCardInform__->DateTaarefePassed[0];
    TotalValues->DateTaarefe[1] = RFIDCardInform__->DateTaarefePassed[1];
    TotalValues->DateTaarefe[2] = RFIDCardInform__->DateTaarefePassed[2];
    TotalValues->DateTaarefe[3] = RFIDCardInform__->DateTaarefePassed[3];
    TotalValues->DateStartKeshavarzi = RFIDCardInform__->SatartDay;
    TotalValues->Taarefe[0] = RFIDCardInform__->Taarefe[0];
    TotalValues->Taarefe[1] = RFIDCardInform__->Taarefe[1];
    TotalValues->Taarefe[2] = RFIDCardInform__->Taarefe[2];
    TotalValues->Taarefe[3] = RFIDCardInform__->Taarefe[3];

    int y_j, m_j, d_j;
/*    GetDateFrom1299passday(&y_j,&m_j,&d_j,RFIDCardInform__->SatartDay);
    printf_New("Start_day : %d/%d/%d,\n",y_j,m_j,d_j);
    GetDateFrom1299passday(&y_j,&m_j,&d_j,RFIDCardInform__->SatartDay+RFIDCardInform__->DateTaarefePassed[0]);
    printf_New("Date_0 : %d/%d/%d,\n",y_j,m_j,d_j);
    GetDateFrom1299passday(&y_j,&m_j,&d_j,RFIDCardInform__->SatartDay+RFIDCardInform__->DateTaarefePassed[1]);
    printf_New("Date_1 : %d/%d/%d,\n",y_j,m_j,d_j);
    GetDateFrom1299passday(&y_j,&m_j,&d_j,RFIDCardInform__->SatartDay+RFIDCardInform__->DateTaarefePassed[2]);
    printf_New("Date_2 : %d/%d/%d,\n",y_j,m_j,d_j);
    GetDateFrom1299passday(&y_j,&m_j,&d_j,RFIDCardInform__->SatartDay+RFIDCardInform__->DateTaarefePassed[3]);
    printf_New("Date_3 : %d/%d/%d,\n",y_j,m_j,d_j);

*/


    GetDateFrom1299passday(&y_j, &m_j, &d_j, RFIDCardInform__->SatartDay);
    sprintf(TotalValues->DateStart, "%d/%02d/%02d", y_j, m_j, d_j);
    GetDateFrom1299passday(&y_j, &m_j, &d_j, RFIDCardInform__->SatartDay + TotalValues->Taarefe[3]);
    sprintf(TotalValues->DateEnd, "%d/%02d/%02d", y_j, m_j, d_j);
    IncreseCharzh(RFIDCardInform__->UsedScharzhe, TotalValues->DateStart, TotalValues->DateEnd);

    RFIDCardInform__->UsedScharzhe = 0;
    RFIDCardInform__->DateTaarefePassed[0] = 0;
    RFIDCardInform__->DateTaarefePassed[1] = 0;
    RFIDCardInform__->DateTaarefePassed[2] = 0;
    RFIDCardInform__->DateTaarefePassed[3] = 0;
    RFIDCardInform__->Taarefe[0] = 0;
    RFIDCardInform__->Taarefe[1] = 0;
    RFIDCardInform__->Taarefe[2] = 0;
    RFIDCardInform__->Taarefe[3] = 0;
    RFIDCardInform__->CharzheDore[0] = 0;
    RFIDCardInform__->CharzheDore[1] = 0;
    RFIDCardInform__->CharzheDore[2] = 0;
    RFIDCardInform__->CharzheDore[3] = 0;

    SumChar = (char *) malloc(sizeof(RFIDCardInform__));
    memcpy(SumChar, RFIDCardInform__, sizeof(RFIDCardInform__));
    RFIDCardInform__->XorSum=Calc_publicCheckSum(SumChar, sizeof(RFIDCardInform__));
    free(SumChar);
    WriteInformToRFidCard(RFIDCardInform__);

    return 1;

}

int WriteInformToRFidCard(_RFIDCardInform *RFIDCardInform__) {
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    uint8_t success = 1;                         // Flag to check if there was an error with the PN532
    bool authenticated = false;               // Flag to indicate if the sector is authenticated
    uint8_t blankAccessBits[3] = {0xff, 0x07, 0x80};
    uint8_t idx = 0;
    uint8_t numOfSector = NR_BLOCK_OF_LONGSECTOR;                 // Assume Mifare Classic 1K for now (16 4-block sectors)

    // Keyb on NDEF and Mifare Classic should be the same
    uint8_t keyuniversal[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    char WriteTONFC[100];//rzh
    int readCounter = 0;
    uint8_t currentblock_R;
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    if (!success) {
        IF_SERIAL_DEBUG(printf_New("<%s>  success = nfc.readPassiveTargetID = null", __PRETTY_FUNCTION__));
        return 0;
    }
    if (uidLength != 4) {
        IF_SERIAL_DEBUG(printf_New("<%s>  uidLength != 4", __PRETTY_FUNCTION__));
        return 0;
    }
    //   if (SectorRequesttoRead < 1 || SectorRequesttoRead > 15) return;
    uint8_t data[NR_BLOCK_OF_LONGSECTOR], *data_ST;                        // Array to store block data during reads
    IF_SERIAL_DEBUG(printf_New("Write :Start writing", 0));

    /*   printf_New("@@@@111111=  %u ,%d,%d,%d,%d  ", RFIDCardInform__->UsedScharzhe,RFIDCardInform__->DateTaarefe[0],RFIDCardInform__->DateTaarefe[1]
               ,RFIDCardInform__->DateTaarefe[2],RFIDCardInform__->DateTaarefe[3]);

       printf_New( "  %f ,%f ,%f,%f",RFIDCardInform__->Taarefe[0],RFIDCardInform__->Taarefe[1],RFIDCardInform__->Taarefe[2],RFIDCardInform__->Taarefe[3]);
       printf_New( "  %u ,%u ,%u,%u\n",RFIDCardInform__->CharzheDore[0],RFIDCardInform__->CharzheDore[1],RFIDCardInform__->CharzheDore[2],RFIDCardInform__->CharzheDore[3]);
   */
    int SuccessWrite;
    int len = sizeof(_RFIDCardInform);

    char *raw = (char *) malloc(len);
    if (raw == NULL) {
        IF_SERIAL_DEBUG(printf_New("<%s>  can not allocate memory", __PRETTY_FUNCTION__));
        return 0;
    }
    int z = 0;
    memcpy(raw, RFIDCardInform__, len);

    uint8_t data_[10][NR_BLOCK_OF_LONGSECTOR];                        // Array to store block data during reads
    for (int y = 0; y < len;) {
        for (int x = 0; x < 16; x++) {
            data_[z][x] = 0;
            if ((y + x) < len)
                data_[z][x] = raw[y + x];
        }
        SuccessWrite = WriteNFCBlock(uid, uidLength, 10 + z, data_[z]);
        z = z + 1;
        y = y + 16;
    }

    /*  RFIDCardInform__->UsedScharzhe=0;
      RFIDCardInform__->DateTaarefe[0]=0;RFIDCardInform__->DateTaarefe[1]=0;RFIDCardInform__->DateTaarefe[2]=0;RFIDCardInform__->DateTaarefe[3]=0;
      RFIDCardInform__->Taarefe[0]=0;RFIDCardInform__->Taarefe[1]=0;RFIDCardInform__->Taarefe[2]=0;RFIDCardInform__->Taarefe[3]=0;
      RFIDCardInform__->CharzheDore[0]=0;RFIDCardInform__->CharzheDore[1]=0;RFIDCardInform__->CharzheDore[2]=0;RFIDCardInform__->CharzheDore[3]=0;
      printf_New("@@@@2222222=  %u ,%d,%d,%d,%d  ", RFIDCardInform__->UsedScharzhe,RFIDCardInform__->DateTaarefe[0],RFIDCardInform__->DateTaarefe[1]
              ,RFIDCardInform__->DateTaarefe[2],RFIDCardInform__->DateTaarefe[3]);
      printf_New( "  %f ,%f ,%f,%f",RFIDCardInform__->Taarefe[0],RFIDCardInform__->Taarefe[1],RFIDCardInform__->Taarefe[2],RFIDCardInform__->Taarefe[3]);
      printf_New( "  %u ,%u ,%u,%u\n",RFIDCardInform__->CharzheDore[0],RFIDCardInform__->CharzheDore[1],RFIDCardInform__->CharzheDore[2],RFIDCardInform__->CharzheDore[3]);
      z=0;
      for (int y = 0; y < len;) {
          ReadNFCRead(uid, uidLength, 10+z, data_[z]);

          for (int x = 0; x < 16; x++) {
               if ((y + x) < len)
                  raw[y + x]=data_[z][x] ;
          }
          z=z+1;
          y = y + 16;
      }

      memcpy( RFIDCardInform__,raw, len);
      printf_New("@@@@33333=  %u ,%d,%d,%d,%d  ", RFIDCardInform__->UsedScharzhe,RFIDCardInform__->DateTaarefe[0],RFIDCardInform__->DateTaarefe[1]
              ,RFIDCardInform__->DateTaarefe[2],RFIDCardInform__->DateTaarefe[3]);
      printf_New( "  %f ,%f ,%f,%f",RFIDCardInform__->Taarefe[0],RFIDCardInform__->Taarefe[1],RFIDCardInform__->Taarefe[2],RFIDCardInform__->Taarefe[3]);
      printf_New( "  %u ,%u ,%u,%u\n",RFIDCardInform__->CharzheDore[0],RFIDCardInform__->CharzheDore[1],RFIDCardInform__->CharzheDore[2],RFIDCardInform__->CharzheDore[3]);
  */
    free(raw);
    return SuccessWrite;
}

int ReadInformToRFidCard(_RFIDCardInform *RFIDCardInform__) {
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    uint8_t success = 1;                         // Flag to check if there was an error with the PN532
    bool authenticated = false;               // Flag to indicate if the sector is authenticated
    uint8_t blankAccessBits[3] = {0xff, 0x07, 0x80};
    uint8_t idx = 0;
    uint8_t numOfSector = NR_BLOCK_OF_LONGSECTOR;                 // Assume Mifare Classic 1K for now (16 4-block sectors)

    // Keyb on NDEF and Mifare Classic should be the same
    uint8_t keyuniversal[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    char WriteTONFC[100];//rzh
    int readCounter = 0;
    uint8_t currentblock_R;
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    if (!success) {
//        IF_SERIAL_DEBUG(printf_New("<%s>  success = nfc.readPassiveTargetID = null", __PRETTY_FUNCTION__));
        return 0;
    }
    boolean ReadingPrevRFID = true;
    // Display some basic information about the card

    for (uint8_t i = 0; i < uidLength; i++) {
        if (uid[i] != Last_uid[i])
            ReadingPrevRFID = false;
        Last_uid[i] = uid[i];
    }
    if (ReadingPrevRFID) return 0;
    if (uidLength != 4) {
        IF_SERIAL_DEBUG(printf_New("<%s>  uidLength != 4", __PRETTY_FUNCTION__));
        return 0;
    }
    //   if (SectorRequesttoRead < 1 || SectorRequesttoRead > 15) return;
    uint8_t data[NR_BLOCK_OF_LONGSECTOR], *data_ST;                        // Array to store block data during reads
    IF_SERIAL_DEBUG(printf_New("Write :Start writing", 0));

    /*  printf_New("@@@@111111=  %u ,%d,%d,%d,%d  ", RFIDCardInform__->UsedScharzhe,RFIDCardInform__->DateTaarefe[0],RFIDCardInform__->DateTaarefe[1]
              ,RFIDCardInform__->DateTaarefe[2],RFIDCardInform__->DateTaarefe[3]);

      printf_New( "  %f ,%f ,%f,%f",RFIDCardInform__->Taarefe[0],RFIDCardInform__->Taarefe[1],RFIDCardInform__->Taarefe[2],RFIDCardInform__->Taarefe[3]);
      printf_New( "  %u ,%u ,%u,%u\n",RFIDCardInform__->CharzheDore[0],RFIDCardInform__->CharzheDore[1],RFIDCardInform__->CharzheDore[2],RFIDCardInform__->CharzheDore[3]);
  */
    int SuccessRead;
    int len = sizeof(_RFIDCardInform);

    char *raw = (char *) malloc(len);
    if (raw == NULL) {
        IF_SERIAL_DEBUG(printf_New("<%s>  can not allocate memory", __PRETTY_FUNCTION__));
        return 0;
    }
    int z = 0;
    uint8_t data_[10][NR_BLOCK_OF_LONGSECTOR];                        // Array to store block data during reads
/*   memcpy(raw, RFIDCardInform__, len);

    for (int y = 0; y < len;) {
        for (int x = 0; x < 16; x++) {
            data_[z][x] = 0;
            if ((y + x) < len)
                data_[z][x] = raw[y + x];
        }
        WriteNFCBlock(uid, uidLength, 10+z, data_[z]);
        z=z+1;
        y = y + 16;
    }

    RFIDCardInform__->UsedScharzhe=0;
    RFIDCardInform__->DateTaarefe[0]=0;RFIDCardInform__->DateTaarefe[1]=0;RFIDCardInform__->DateTaarefe[2]=0;RFIDCardInform__->DateTaarefe[3]=0;
    RFIDCardInform__->Taarefe[0]=0;RFIDCardInform__->Taarefe[1]=0;RFIDCardInform__->Taarefe[2]=0;RFIDCardInform__->Taarefe[3]=0;
    RFIDCardInform__->CharzheDore[0]=0;RFIDCardInform__->CharzheDore[1]=0;RFIDCardInform__->CharzheDore[2]=0;RFIDCardInform__->CharzheDore[3]=0;
    printf_New("@@@@2222222=  %u ,%d,%d,%d,%d  ", RFIDCardInform__->UsedScharzhe,RFIDCardInform__->DateTaarefe[0],RFIDCardInform__->DateTaarefe[1]
            ,RFIDCardInform__->DateTaarefe[2],RFIDCardInform__->DateTaarefe[3]);
    printf_New( "  %f ,%f ,%f,%f",RFIDCardInform__->Taarefe[0],RFIDCardInform__->Taarefe[1],RFIDCardInform__->Taarefe[2],RFIDCardInform__->Taarefe[3]);
    printf_New( "  %u ,%u ,%u,%u\n",RFIDCardInform__->CharzheDore[0],RFIDCardInform__->CharzheDore[1],RFIDCardInform__->CharzheDore[2],RFIDCardInform__->CharzheDore[3]);
*/
    z = 0;

    for (int y = 0; y < len;) {
        SuccessRead = ReadNFCRead(uid, uidLength, 10 + z, data_[z]);

        for (int x = 0; x < 16; x++) {
            if ((y + x) < len)
                raw[y + x] = data_[z][x];
        }
        z = z + 1;
        y = y + 16;
    }

    memcpy(RFIDCardInform__, raw, len);
/*
    printf_New("@@@@33333=  %u ,%d,%d,%d,%d  ", RFIDCardInform__->UsedScharzhe,RFIDCardInform__->DateTaarefe[0],RFIDCardInform__->DateTaarefe[1]
            ,RFIDCardInform__->DateTaarefe[2],RFIDCardInform__->DateTaarefe[3]);
    printf_New( "  %f ,%f ,%f,%f",RFIDCardInform__->Taarefe[0],RFIDCardInform__->Taarefe[1],RFIDCardInform__->Taarefe[2],RFIDCardInform__->Taarefe[3]);
    printf_New( "  %u ,%u ,%u,%u\n",RFIDCardInform__->CharzheDore[0],RFIDCardInform__->CharzheDore[1],RFIDCardInform__->CharzheDore[2],RFIDCardInform__->CharzheDore[3]);
*/
    free(raw);
    return SuccessRead;
}

void tttttttest() {
    _RFIDCardInform RFIDCardInform__1;
    _RFIDCardInform *RFIDCardInform__ = &RFIDCardInform__1;
    RFIDCardInform__->SatartDay = 7297;//13951222
    RFIDCardInform__->UsedScharzhe = 54321;
    RFIDCardInform__->DateTaarefePassed[0] = 100;
    RFIDCardInform__->DateTaarefePassed[1] = 101;
    RFIDCardInform__->DateTaarefePassed[2] = 102;
    RFIDCardInform__->DateTaarefePassed[3] = 103;
    RFIDCardInform__->Taarefe[0] = 200.2;
    RFIDCardInform__->Taarefe[1] = 201.201;
    RFIDCardInform__->Taarefe[2] = 202.202;
    RFIDCardInform__->Taarefe[3] = 203.203;
    RFIDCardInform__->CharzheDore[0] = 30000;
    RFIDCardInform__->CharzheDore[1] = 30001;
    RFIDCardInform__->CharzheDore[2] = 30002;
    RFIDCardInform__->CharzheDore[3] = 30003;
    char *SumChar;
    SumChar = (char *) malloc(sizeof(RFIDCardInform__));
    memcpy(SumChar, RFIDCardInform__, sizeof(RFIDCardInform__));
    RFIDCardInform__->XorSum=Calc_publicCheckSum(SumChar, sizeof(RFIDCardInform__));
    free(SumChar);
    WriteInformToRFidCard(RFIDCardInform__);
    RFIDCardInform__->UsedScharzhe = 0;
    RFIDCardInform__->DateTaarefePassed[0] = 0;
    RFIDCardInform__->DateTaarefePassed[1] = 0;
    RFIDCardInform__->DateTaarefePassed[2] = 0;
    RFIDCardInform__->DateTaarefePassed[3] = 0;
    RFIDCardInform__->Taarefe[0] = 0;
    RFIDCardInform__->Taarefe[1] = 0;
    RFIDCardInform__->Taarefe[2] = 0;
    RFIDCardInform__->Taarefe[3] = 0;
    RFIDCardInform__->CharzheDore[0] = 0;
    RFIDCardInform__->CharzheDore[1] = 0;
    RFIDCardInform__->CharzheDore[2] = 0;
    RFIDCardInform__->CharzheDore[3] = 0;
    ReadInformToRFidCard(RFIDCardInform__);
    byte CheckSm;
    SumChar = (char *) malloc(sizeof(RFIDCardInform__));
    memcpy(SumChar, RFIDCardInform__, sizeof(RFIDCardInform__));
    CheckSm=Calc_publicCheckSum(SumChar, sizeof(RFIDCardInform__));
    free(SumChar);

    printf_New("@@@@33333Sum=%d,%d  = %u %u ,%d,%d,%d,%d  ",RFIDCardInform__->XorSum,CheckSm, RFIDCardInform__->SatartDay, RFIDCardInform__->UsedScharzhe,
               RFIDCardInform__->DateTaarefePassed[0],
               RFIDCardInform__->DateTaarefePassed[1], RFIDCardInform__->DateTaarefePassed[2],
               RFIDCardInform__->DateTaarefePassed[3]);
    printf_New("  %f ,%f ,%f,%f", RFIDCardInform__->Taarefe[0], RFIDCardInform__->Taarefe[1],
               RFIDCardInform__->Taarefe[2], RFIDCardInform__->Taarefe[3]);
    printf_New("  %u ,%u ,%u,%u\n", RFIDCardInform__->CharzheDore[0], RFIDCardInform__->CharzheDore[1],
               RFIDCardInform__->CharzheDore[2], RFIDCardInform__->CharzheDore[3]);
    int y_j, m_j, d_j;
    GetDateFrom1299passday(&y_j, &m_j, &d_j, RFIDCardInform__->SatartDay);
    printf_New("Start_day : %d/%d/%d,\n", y_j, m_j, d_j);
    GetDateFrom1299passday(&y_j, &m_j, &d_j, RFIDCardInform__->SatartDay + RFIDCardInform__->DateTaarefePassed[0]);
    printf_New("Date_0 : %d/%d/%d,\n", y_j, m_j, d_j);
    GetDateFrom1299passday(&y_j, &m_j, &d_j, RFIDCardInform__->SatartDay + RFIDCardInform__->DateTaarefePassed[1]);
    printf_New("Date_1 : %d/%d/%d,\n", y_j, m_j, d_j);
    GetDateFrom1299passday(&y_j, &m_j, &d_j, RFIDCardInform__->SatartDay + RFIDCardInform__->DateTaarefePassed[2]);
    printf_New("Date_2 : %d/%d/%d,\n", y_j, m_j, d_j);
    GetDateFrom1299passday(&y_j, &m_j, &d_j, RFIDCardInform__->SatartDay + RFIDCardInform__->DateTaarefePassed[3]);
    printf_New("Date_3 : %d/%d/%d,\n", y_j, m_j, d_j);
}
/*void testdate() {
    int gy, gm, gd;

    M2Sh(&gy, &gm, &gd, 2017, 3, 12);
    char msg[100];
    sprintf(msg, "M2sh----> %d:%d:%d,NoDay:=%u\n", gy, gm, gd,  GetPassedDateFrom1299(1395, 12, 22));
    printf(msg);


    Sh2M(&gy, &gm, &gd, 1395, 12, 22);
    sprintf(msg, "SH2M-----> %d:%d:%d,\n", gy, gm, gd);
    sprintf(msg, "SH2M----> %d:%d:%d,NoDay:=%d \n", gy, gm, gd,  Day_Of_Year_Jalali(2012, 03, 12));
    printf(msg);
    unsigned long paseday=GetPassedDateFrom1299(1395, 12, 22);
    GetDateFrom1299passday(&gy, &gm, &gd,paseday);
    sprintf(msg, "AfterPasedDay-----> %d:%d:%d,\n", gy, gm, gd);
    printf(msg);

}*/