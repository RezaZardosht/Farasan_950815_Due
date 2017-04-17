
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "Time.h"
#include <LiquidCrystal.h>
#include "loop.h"
//#include "DueFlashStorage\DueFlashStorage.h"
//#include "DueTimer\DueTimer.h"
#define DEBUG true

bool IsNumber(char *StrNum) {
    char str[20];
    int k = 0;
    memcpy(str, StrNum, strlen(StrNum));
    for (int i = 0; i < (strlen(StrNum) - 1); i++) {
        k = (int) str[i];

        if ((k >= 48 && k <= 57) || str[i] == '.' || str[0] == '-') {
            // it is a number, so do some code

        } else {
            return false;
            // it is not a number, do something else
        }
    }

    return true;
}

bool IsNumber(const char *StrNum) {
    char *Tempstr;
    sprintf(Tempstr, "%s", StrNum);
    return (IsNumber(Tempstr));
}

int m2s______(int ym, int mm, int dm, int &ys, int &ms, int &ds) {
    unsigned long int ys1, ym1;
    int ym2, ys2, mm1, ms1, k, ms0;
    char msb_Date[100];

    ym1 = ym + 2000;
    k = ym1 % 4;
    ym1--;
    ym1 = ym1 * 365;
    if (mm == 1) {
        mm1 = 0;
    }
    if (mm == 2) {
        mm1 = 31;
    }
    if (mm == 3) {
        mm1 = 59;
    }
    if (mm == 4) {
        mm1 = 90;
    }
    if (mm == 5) {
        mm1 = 120;
    }
    if (mm == 6) {
        mm1 = 151;
    }
    if (mm == 7) {
        mm1 = 181;
    }
    if (mm == 8) {
        mm1 = 212;
    }
    if (mm == 9) {
        mm1 = 243;
    }
    if (mm == 10) {
        mm1 = 273;
    }
    if (mm == 11) {
        mm1 = 304;
    }
    if (mm == 12) {
        mm1 = 334;
    }
    if (k == 0) {
        mm1++;
    }
    ym1 = ym1 + mm1;
    ym1 = ym1 + dm;
    ym2 = ym + 2000;
    ym2--;
    ym2 = ym2 / 4;
    ym1 = ym1 + ym2;
    ym1 = ym1 - 226899;
    ys2 = ym2 - 155;
    ys1 = ym1 - ys2;
    ys = ys1 / 365;
    ys = ys - 1999;
    ms1 = ys1 % 365;
    ms0 = 1;
    if (ms1 > 31) {
        ms0++;
        ms1 = ms1 - 31;
    }
    if (ms1 > 31) {
        ms0++;
        ms1 = ms1 - 31;
    }
    if (ms1 > 31) {
        ms0++;
        ms1 = ms1 - 31;
    }
    if (ms1 > 31) {
        ms0++;
        ms1 = ms1 - 31;
    }
    if (ms1 > 31) {
        ms0++;
        ms1 = ms1 - 31;
    }
    if (ms1 > 31) {
        ms0++;
        ms1 = ms1 - 31;
    }
    if (ms1 > 30) {
        ms0++;
        ms1 = ms1 - 30;
    }
    if (ms1 > 30) {
        ms0++;
        ms1 = ms1 - 30;
    }
    if (ms1 > 30) {
        ms0++;
        ms1 = ms1 - 30;
    }
    if (ms1 > 30) {
        ms0++;
        ms1 = ms1 - 30;
    }
    if (ms1 > 30) {
        ms0++;
        ms1 = ms1 - 30;
    }
    ds = ms1;
    ms = ms0;
}

/*
  char *TTstr;
    char FFstr[20];
  TTstr = FtoStr(1234.789 , FFstr, 2) ;
  lcd.setCursor(1, 1);
  lcd.print(TTstr);
*/

unsigned long get_MicroSecondDiff(unsigned long PrevValue) {
    if (micros() >= PrevValue)return (micros() - PrevValue);
    else return (GreaUnSLong - PrevValue) + micros();
}

unsigned long get_MilliSecondDiff(unsigned long PrevValue) {
    if (millis() >= PrevValue)return (millis() - PrevValue);
    else return (GreaUnSLong - PrevValue) + millis();
}

int BatteryRemainlife() {
    return 100;
}

void ClearEventLog() {
    setEvent(EventLogCleared, true);
    setEvent(EventLogCleared, false);

}

boolean BatteryCharzheError() {
    return false;
}

boolean get_StrongDCMagneticFieldDetected() {
    return digitalRead(StrongDCMagneticFieldDetected_Pin);
};

boolean get_MeterCoverRemoved() {
    return digitalRead(MeterCoverRemoved_Pin);
}

long getDecimal(float val) {
    int intPart = int(val);
    long decPart = 1000 * (val -
                           intPart); //I am multiplying by 1000 assuming that the foat values will have a maximum of 3 decimal places.
    //Change to match the number of decimal places you need
    if (decPart > 0)return (decPart);           //return the decimal part of float number if it is available
    else if (decPart < 0)return ((-1) * decPart); //if negative, multiply by -1
    else if (decPart = 0)return (00);           //return 0 if decimal part of float number is not available
}

// return the day of month for sum of days
int GetDayOfMonthForSumofDays(int days) {
    if (days <= 186)
        return ((int) days % 31 == 0) ? 31 : (int) days % 31;
    else
        return ((int) (days - 186) % 30 == 0) ? 30 : (int) (days - 186) % 30;
}

int GetMonthOfForSumofDays(int days) {
    if (days <= 186)
        return ((int) ((days - 1) / 31) + 1);
    else
        return ((int) ((days - 1 - 186) / 30) + 1) + 6;
}

void GetDateFrom1299passday(int *y_j, int *m_j, int *d_j, int PasedDay) {
    int no_yearPassed = (int) (PasedDay / 365) / 4;
    *y_j = (int) ((PasedDay - no_yearPassed) / 365) + 1375 + 1;
    int md_j = PasedDay - ((*y_j - 1 - 1375) * 365 + no_yearPassed);
    *m_j = GetMonthOfForSumofDays(md_j);
    *d_j = GetDayOfMonthForSumofDays(md_j);
}

unsigned long GetPassedDateFrom1299(int y_j, int m_j, int d_j) {
    int no_yearPassed = (y_j - 1 - 1375);
    int no_kabise = (int) (no_yearPassed) / 4;
    int passedday_j = no_yearPassed * 365 + no_kabise;
    int PassedinMonth = (m_j <= 7) ? (m_j - 1) * 31 : ((m_j - 7) * 30) + 186;
    int passeDay = passedday_j + PassedinMonth + d_j;


    return passeDay;
}


int strtoint(char a[]) {
    int c, sign, offset, n;
    offset = 0;
    sign = 1;
    if (a[0] == '-') {  // Handle negative integers
        sign = -1;
        offset = 1;
    }
    n = 0;
    for (c = offset; a[c] != '\0'; c++) {
        n = n * 10 + a[c] - '0';
    }
    n = n * sign;
    return n;
}

/*const int ElectricalCurrentDisconnected = 12;
  const int ElectricalCurrentConnected = 13;
  const int TamperedWaterFlowDetected = 14;
  const int SuccessfulAuthentication = 15;
  const int AuthenticationFailed = 16;
  const int OperationalKeyChanged = 17;
  const int Secret1ForSecureAlgorithmHasChanged = 18;
  const int Secret2ForSecureAlgorithmHasChanged = 19;
  const int MasterKeyChanged = 21;
  const int MBusDisconnected = 22;*/
char *Dtostrf(double val, signed char width, unsigned char prec, char *sout) {

    char fmt[20];
    sprintf(fmt, "%%%d.%df", width, prec);
    sprintf(sout, fmt, val);
    return sout;
}

#define ARDBUFFER 160

char *FtoStr(float f, char (&FStr)[20], int DecPoint) {
    int D_DecPoint = (DecPoint >= 0 || DecPoint < 5) ? DecPoint : 0;
    for (int i = 0; i < 20; i++)FStr[i] = 0x0;
    char ftostr[50];
    float N_DECIMAL_POINTS_PRECISION = (float) pow(10, D_DecPoint);
    float integerPart = (float) (long) f;
    float TempDecimal = abs((f - integerPart) * N_DECIMAL_POINTS_PRECISION);
    int decimalPart = (int) (TempDecimal + 0.5);
    // int decimalPart = abs(((int)(f * N_DECIMAL_POINTS_PRECISION) % N_DECIMAL_POINTS_PRECISION));
    //  int decimalPart =(int)((f-(float)integerPart)* N_DECIMAL_POINTS_PRECISION);
    sprintf(ftostr, "%d.%d", (long) integerPart, decimalPart);
    strncpy(FStr, ftostr, strlen(ftostr));
    return (char *) FStr;
}

int printf_New(char *str, ...) {
    int i, count = 0, j = 0, flag = 0, k;
    float ff;
    char Temp20C[20];
    char temp[ARDBUFFER + 1];

    char msg[100], msg2[100];
    boolean Find_Argument;
    for (i = 0; str[i] != '\0'; i++) if (str[i] == '%') count++;

    va_list argv;
    va_start(argv, count);
    i = 0;
    j = 0;
    while (str[i] != '\0') {
        k = 0;
        if (str[i] == '%') {
            msg[0] = str[i];
            temp[j] = '\0';
            Serial.print(temp);
            j = 0;
            temp[0] = '\0';
            Find_Argument = false;
            while (!Find_Argument && str[i] != '\0') {
                msg[++k] = str[++i];
                msg[k + 1] = '\0';
                switch (str[i]) {
                    case 'd':
                        sprintf(msg2, msg, va_arg(argv, int));
                        Find_Argument = true;
                        break;
                    case 'l':
                        sprintf(msg2, msg, va_arg(argv, long));
                        Find_Argument = true;
                        break;
                    case 'u':
                        sprintf(msg2, msg, va_arg(argv, unsigned
                                long));
                        Find_Argument = true;
                        break;
                    case 'f':
                        ff = (float) va_arg(argv, double);
                        //   sprintf(msg2, msg, (float)va_arg(argv, double));
                        sprintf(msg2, "%s", FtoStr(ff, Temp20C, 2));

                        Find_Argument = true;
                        break;
                    case 'c':
                        sprintf(msg2, msg, (char) va_arg(argv, int));
                        Find_Argument = true;
                        break;
                    case 's':
                        sprintf(msg2, msg, va_arg(argv, char *));
                        Find_Argument = true;
                        break;
                    case 'x':
                    case 'X':
                        sprintf(msg2, msg, va_arg(argv, int));
                        Find_Argument = true;

                        break;
                    default:;
                };
            }
            Serial.print(msg2);
            j = 0;
            temp[0] = '\0';
        } else {
            temp[j] = str[i];
            temp[j + 1] = '\0';
            j = (j + 1) % ARDBUFFER;
            if (j == 0) {
                temp[ARDBUFFER] = '\0';
                Serial.print(temp);
                temp[0] = '\0';
            }
        }
        i++;
    };
    //sprintf(msg2,"%d,%s", va_arg(argv, int));
    //          Find_Argument = true;

    Serial.print(temp);

    //  Serial.println();
    return count + 1;
}

char *GetStrCurrentDay(char *Mem) {
    int ys, ms, ds;
    ys = 2016;
    ms = 3;
    ds = 12;
    M2Sh(&ys, &ms, &ds, year(), month(), day());
    sprintf(Mem, "%04d%02d%02d", ys, ms, ds);
    return Mem;
}

char GetCharCurrentProfile() {

    return (char) 0xff;
}

char *GetCurrentStrHour(char *Mem) {
    sprintf(Mem, "%02d%02d%02d", hour(), minute(), second());
    return Mem;
}

byte Calc_publicCheckSum(char *data, int size_data) {
    byte cksum = 0;
    for (int i = 0; i < size_data; i++) {
        cksum += data[i];
    }
    return cksum;
}

void IECuseSerial_print_2Port(char *Data) {
    IECuseSerial.print(Data);
    IECuseSerialWithPC.print(Data);
}

void IECuseSerial_write_2Port(char dataC) {
    IECuseSerial.write(dataC);
    IECuseSerialWithPC.write(dataC);
}


void IECuseSerial_flush_2Port() {
    IECuseSerial.flush();
    IECuseSerialWithPC.flush();
}

void IECuseSerial_end_2Port() {
    IECuseSerial.end();
    IECuseSerialWithPC.end();
}
