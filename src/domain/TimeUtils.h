#pragma once
#include "TimeUTC.h"

// ===============================

// will build an INLINE FUNCTION so that we eliminate the (small) overhead by calculating everything in a single step.

// Assuming we are dealing with times in the same day
inline double secondsBetween(const TimeUTC& t0, const TimeUTC& t1) {
    return (t1.hour - t0.hour) * 3600.0 +
            (t1.minute - t0.minute) * 60.0 +
            (t1.second - t0.second);
}

// Minutes from epoch to MJD2000 (decimal point)
// TLE will come as (year, dayOfYear)
// targetEpoch will come as (year, month, day, hour, minute, second)
inline double epoch2MJD2000(const TimeUTC& targetTime) {

    int year = targetTime.year;
    int month = targetTime.month;
    int day = targetTime.day;
    int hour = targetTime.hour;
    int minute = targetTime.minute;
    int second = targetTime.second;

    // Calculating amount of leap years between target year since 2000
    int leapCount = 0;
    for (int y = 2000; y < year; y++) {
        if (leapYear(y)) {
            leapCount++;
        }
    }
 
    int febD;
    if (leapYear(year)) {
        int febD = 29;
    } else {
        int febD = 28;
    }

    int d;
    switch (month-1) {
    case 1:
        int d = 31;
        break;
    case 2:
        int d = 31 + febD;
        break;
    case 3:
        int d = 31 + febD + 31;
        break;
    case 4:
        int d = 31 + febD + 31 + 30;
        break;
    case 5:
        int d = 31 + febD + 31 + 30 + 31;
        break;
    case 6:
        int d = 31 + febD + 31 + 30 + 31 + 30;
        break;
    case 7:
        int d = 31 + febD + 31 + 30 + 31 + 30 + 31;
        break;
    case 8:
        int d = 31 + febD + 31 + 30 + 31 + 30 + 31 + 31;
        break;
    case 9:
        int d = 31 + febD + 31 + 30 + 31 + 30 + 31 + 31 + 30;
        break;
    case 10:
        int d = 31 + febD + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31;
        break;
    case 11:
        int d = 31 + febD + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30;
        break;
    case 12:
        int d = 31 + febD + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31;
        break;
    default:
        break;
    }

    int yearsSince = (365 * (year - 1 - 2000))*24*60; // [min]
    int daysSince = (d + day - 1)*24*60;
    int hoursSince = hour*60;
    double secondsSince = second/60;
    
    double minuteSince = yearsSince + daysSince + hoursSince + minute + secondsSince;

    return minuteSince;
}

inline double epoch2MJD2000_TLE(int year, double dayFrac) {
    // Calculating amount of leap years between target year since 2000
    int leapCount = 0;
    for (int y = 2000; y < year; y++) {
        if (leapYear(y)) {
            leapCount++;
        }
    }

    int yearsSince = (365 * (year - 1 - 2000))*24*60; // [min]
    double dayMinSince = dayFrac*24*60;

    double minuteSince = yearsSince + dayMinSince;

    return minuteSince;
}

inline bool leapYear(int year) {
    bool isLeap;
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        isLeap = 1; 
    }
    return isLeap;
}

