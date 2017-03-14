
#ifndef HEADER_HELPERFUNC
#define HEADER_HELPERFUNC

bool IsNumber(char *StrNum);


int m2s(int ym, int mm, int dm, int &ys, int &ms, int &ds);

char *FtoStr(float f, char (&FStr)[20], int DecPoint);

char *Dtostrf(double val, signed char width, unsigned char prec, char *sout);

int printf_New(char *str, ...);

char *GetStrCurrentDay(char *Mem);

char GetCharCurrentProfile();

char *GetCurrentStrHour(char *Mem);


void GetDateFrom1299passday(int *y_j, int *m_j, int *d_j, int PasedDay) ;
unsigned long GetPassedDateFrom1299(int y_j, int m_j, int d_j);

byte Calc_publicCheckSum(char *data,int size_data);

#endif

