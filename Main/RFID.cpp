// IF_SERIAL_DEBUG(printf_New("%s: Entered \n", __PRETTY_FUNCTION__));

#include "loop.h"

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
boolean NFCInitializeOK=false;
void InitializeNFC(){

        nfc.begin();
        uint32_t versiondata = nfc.getFirmwareVersion();
        if (!versiondata) {
            IF_SERIAL_DEBUG(printf_New("Didn't find PN53x board"));
            setEvent(ErrorInternal_RFID,true);
            setEvent(ApplicationError,true);
            return;
        }
        // Got ok data, print it out!
        IF_SERIAL_DEBUG(printf_New("Found chip PN5 %X", (versiondata >> 24) & 0xFF));
        IF_SERIAL_DEBUG(printf_New("Firmware ver. %d", (versiondata >> 16) & 0xFF));
        IF_SERIAL_DEBUG(printf_New(". %d", (versiondata >> 8) & 0xFF, DEC));

        // configure board to read RFID tags
        nfc.SAMConfig();

        NFCInitializeOK=true;
        return;


}
void NFC_Loop(void) {

   if(!NFCInitializeOK)return;

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
        ShowDisplayStartREadingRFID();

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

            IF_SERIAL_DEBUG(printf_New("%d\n", TempLongCharzhe.longn));
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
            setEvent( CreditAssignment,true);
            setEvent( CreditAssignment,false);
            uint8_t dateStart[NR_BLOCK_OF_LONGSECTOR];
            uint8_t dateEnd[NR_BLOCK_OF_LONGSECTOR];
            delay(10);
            ReadNFCRead(uid, uidLength, NFC_Sector_DATEStart, dateStart);
            delay(10);
            ReadNFCRead(uid, uidLength, NFC_Sector_DATEEnd, dateEnd);
            delay(10);
            for (int i = 0; i < 10; i++)data[i] = (int) '\x0';
            ReadNFCRead(uid, uidLength, NFC_Sector_FINISH, data);

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
        for (int i = 0; i <n;i++)
            Last_uid[i] = 0;  // Buffer to store the returned UID
    }

    ShowDisplayEndREadingRFID();

}

void WriteNFCBlock(uint8_t uid[], uint8_t uidLength, uint8_t SectorRequesttoWrite, uint8_t *data) {
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
    if (!success) return;
    if (uidLength != 4) return;
    if (idx < 1 || idx > 15) return;
    // Step 1: Authenticate the current sector using key B 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    IF_SERIAL_DEBUG(printf_New("write :Start writing", 0));

    success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, BLOCK_NUMBER_OF_SECTOR_TRAILER(idx), 1,
                                                  (uint8_t *) KEY_DEFAULT_KEYAB);
    if (!success) {
        IF_SERIAL_DEBUG(printf_New("Authentication failed for sector %d", numOfSector));
        return;
    }
    memset(blockBuffer, 0x1F, sizeof(blockBuffer));
    for (int i = 0; i < NR_BLOCK_OF_LONGSECTOR; i++)
        blockBuffer[i] = data[i];
    if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer))) {
        IF_SERIAL_DEBUG(printf_New("Unable to write to sector %d", idx));
        return;
    }
    if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer))) {
        IF_SERIAL_DEBUG(printf_New("Unable to write to sector %d", idx));
        return;
    }
    // memset(blockBuffer, 0x2F, sizeof(blockBuffer));  //write on block number 3
    if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 1, blockBuffer))) {
        IF_SERIAL_DEBUG(printf_New("Unable to write to sector %d", idx));
        return;
    }
    // Step 3: Reset both keys to 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    memcpy(blockBuffer, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));
    memcpy(blockBuffer + 6, blankAccessBits, sizeof(blankAccessBits));
    blockBuffer[9] = 0x68;
    memcpy(blockBuffer + 10, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));

    // Step 4: Write the trailer block
    if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)), blockBuffer))) {
        IF_SERIAL_DEBUG(printf_New("Unable to write trailer block of sector %d", numOfSector));
        return;
    }


}

void ReadNFCRead(uint8_t uid[], uint8_t uidLength, uint8_t SectorRequesttoRead, uint8_t *data) {
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
    if (!success) return;
    if (uidLength != 4) return;
    if (SectorRequesttoRead < 1 || SectorRequesttoRead > 15) return;
    IF_SERIAL_DEBUG(printf_New("Read :Start reading", 0));

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
}
