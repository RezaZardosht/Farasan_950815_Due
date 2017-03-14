
//www.radshid.com
 // کدهای زیر توسط مجموعه مهندسی رادشید گرداوری, ایجاد , تست و بصورت رایگان انتشار یافته است.
 
 

typedef struct DATE_E
 {
	unsigned long dtDay;
	unsigned long dtMonth;
	unsigned long dtYear;
} date;

typedef struct _time{
   char sec;     /* seconds after the minute - [0,59]    */
   char min;     /* minutes after the hour -   [0,59]    */
   char hour;    /* hours since midnight -     [0,23]    */
   char mday;    /* day of the month -         [1,31]    */
   char mon;     /* months since January -     [0,11]    */
   int  year;    /* years since                          */
}ATime;

void  M2Sh(int *j_y, int *j_m, int *j_d, int  g_y, int  g_m, int  g_d);// Mildai To Shamsi
void  Sh2M(int *g_y,int *g_m,int *g_d,int  j_y,int  j_m,int  j_d);// Shamsi To Mi 
int	DayOfWeek( int nYear, int nMonth, int nDay );// Rooze Hafte
void  M2Gh(int *year,int *month,int *Day,int d,int m,int y); //Miladi to ghamari;
void Binary2Time(unsigned long binary,ATime *datetime);// TimeStamp Unix To Mildai
unsigned long DateToBinary(int tm_year,int tm_mon,int tm_mday,int tm_hour,int tm_min,int tm_sec);	//Mildai To TimeStamp (Unix)


/* Return 0 ~ 365
 * 0~364 For Normal Year
 * 0~365 For Leap Year
 * */
int Day_Of_Year_Jalali(int  g_y, int  g_m, int  g_d); /*Return 0 To 365*/
unsigned int Day_Of_Year_Miladi(int tm_year,int tm_mon,int tm_mday);

// 0 means normal
// 1 means 1 days forward
// 2 means 1 days Back
date Georgian2Hijri(signed long d,signed long m,signed long y,char Type);
