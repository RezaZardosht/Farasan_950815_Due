
#ifndef HEADER_HELPERFUNC
#define HEADER_HELPERFUNC

bool IsNumber(char *StrNum);

bool IsNumber(const char *StrNum);

void Serial__print(char *Message);

void Serial__println(char *Message);

void Serial__print(String Message);

void Serial__println(String Message);

void Serial__printlnDebug(String Message);

void Serial__println(uint32_t Message, int Type);

void Serial__print(uint32_t Message, int Type);

int m2s(int ym, int mm, int dm, int &ys, int &ms, int &ds);

char *FtoStr(float f, char (&FStr)[20], int DecPoint);

char *Dtostrf(double val, signed char width, unsigned char prec, char *sout);

int printf_New(char *str, ...);

char *GetStrCurrentDay(char *Mem);

char GetCharCurrentProfile();

char *GetCurrentStrHour(char *Mem);

int ardprintf(char *str, ...);

#endif

