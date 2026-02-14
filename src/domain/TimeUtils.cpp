#include "TimeUtils.h"
#include "TimeUTC.h"
#include <stdexcept>
#include <iostream>

// ===============================

bool leapYear(int year) {
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    return isLeap;
};


void validate(const TimeUTC& time) {
    // month validation
        if (time.month < 1 || time.month > 12) 
            throw std::invalid_argument("Invalid month.");
        // all the cases for the days of the different months
        if (time.day < 1 || (time.day > 31 && (time.month == 1 || time.month == 3 || time.month == 5 || time.month == 7 || time.month == 8 || time.month == 10 || time.month == 12)))
            throw std::invalid_argument("Invalid day.");
        if (time.day < 1 || (time.day > 30 && (time.month == 2 || time.month == 4 || time.month == 6 || time.month == 9 || time.month == 11)))
            throw std::invalid_argument("Invalid day.");
        bool isLeap = leapYear(time.year);
        if (time.day < 1 || (time.month == 2 && (time.day > 28 && !isLeap)) || (time.month == 2 && (time.day > 29 && isLeap)))
            throw std::invalid_argument("Invalid day.");
        // hour validation
        if (time.hour < 0 || time.hour > 24)
            throw std::invalid_argument("Invalid hour.");
        // minute validation
        if (time.minute < 0 || time.minute > 60)
            throw std::invalid_argument("Invalid minute.");
        // second validation
        if (time.second < 0.0 || time.second > 59.0)
            throw std::invalid_argument("Invalid second.");
}

// ===============================

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

// ===============================

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
    case 0: // December previous year, doesn't count
        d = 0;
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
    default:
        throw std::invalid_argument("Invalid month: value should be between 1 and 12.");
        break;
    }

    bool currentYear_leap = leapYear(year);
    if (currentYear_leap && month > 2) {
        d = d+1;
    }

    int totDays = totDays_yearminus1 + day + d;

    // Calculating the minutes:
    double target_Min = totDays*24*60 + hour*60 + minute + second/60;

    return target_Min;
};