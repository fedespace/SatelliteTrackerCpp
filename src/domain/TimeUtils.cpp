#include "TimeUtils.h"
#include "TimeUTC.h"
#include <stdexcept>
#include <iostream>

// ===============================

bool leapYear(int year) {
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    return isLeap;
};


double epoch2MJD2000_TLE(int year, double dayFrac) {
    // Calculating amount of leap years between target year since 2000
    int leapCount = 0;
    for (int y = 2000; y < year; y++) {
        if (leapYear(y)){
            leapCount++;
        }
    }
    
    
    int totYears = year - 2000 - 1;
    int totDays = totYears*365 + leapCount;

    // Considering now the day fraction of the TLE and summing together
    double TLE_Days = totYears*365 + leapCount + dayFrac;

    // Converting to minutes
    double TLE_Minutes = TLE_Days * 60 * 24;

    return TLE_Minutes;
};


double epoch2MJD2000(const TimeUTC& targetTime) {

    int year = targetTime.year;
    int month = targetTime.month;
    int day = targetTime.day;
    int hour = targetTime.hour;
    int minute = targetTime.minute;
    int second = targetTime.second;

    // Calculating amount of leap years between target year since 2000
    int leapCount = 0;
    for (int y = 2000; y < year; y++) {
        if (leapYear(y)){
            leapCount++;
        }
    }
 
    int totDays_yearminus1 = (year - 1 - 2000) * 365 + leapCount;

    int d;
    int validMonth = month - 1;
    switch (validMonth) {
    case 1:
        d = 31;
        break;
    case 2:
        d = 31 + 28;
        break;
    case 3:
        d = 31 + 28 + 31;
        break;
    case 4:
        d = 31 + 28 + 31 + 30;
        break;
    case 5:
        d = 31 + 28 + 31 + 30 + 31;
        break;
    case 6:
        d = 31 + 28 + 31 + 30 + 31 + 30;
        break;
    case 7:
        d = 31 + 28 + 31 + 30 + 31 + 30 + 31;
        break;
    case 8:
        d = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31;
        break;
    case 9:
        d = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30;
        break;
    case 10:
        d = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31;
        break;
    case 11:
        d = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30;
        break;
    case 12:
        d = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31;
        break;
    }

    bool currentYear_leap = leapYear(year);
    if (currentYear_leap) {
        d = d+1;
    }

    int totDays = totDays_yearminus1 + d + day;

    // Calculating the minutes:
    double target_Min = totDays*24*60 + hour*60 + minute + second*60;

    return target_Min;
};