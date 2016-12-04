//
// Created by Zardosht on 25/10/2016.
//

#ifndef FARASN_950803_TIMEFUNCS_H
#define FARASN_950803_TIMEFUNCS_H
typedef struct Date_Time {
    char year[4];
    char comma_1;
    char month[2];
    char comma2;
    char day[2];
} DateTimeCharStruct;
typedef struct tm {
    int tm_sec;            /* Seconds.	[0-60] (1 leap second) */
    int tm_min;            /* Minutes.	[0-59] */
    int tm_hour;            /* Hours.	[0-23] */
    int tm_mday;            /* Day.		[1-31] */
    int tm_mon;            /* Month.	[0-11] */
    int tm_year;            /* Year	- 1900.  */
    int tm_wday;            /* Day of week.	[0-6] */
    int tm_yday;            /* Days in year.[0-365]	*/
    int tm_isdst;            /* DST.		[-1/0/1]*/
} TimeStruct;
#endif //FARASN_950803_TIMEFUNCS_H
