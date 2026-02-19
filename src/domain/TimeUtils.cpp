#include "TimeUtils.h"
#include "TimeUTC.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>

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
    
    int totYears = year - 2000;
    int totDays = totYears*365 + leapCount;

    // Considering now the day fraction of the TLE and summing together
    double TLE_Days = totYears*365 + leapCount + dayFrac;

    // Converting to minutes
    double TLE_Minutes = TLE_Days * 60 * 24;

    return TLE_Minutes;
};

// ===============================

double epoch2MJD2000(const TimeUTC& timestart) {

    int year = timestart.year;
    int month = timestart.month;
    int day = timestart.day;
    int hour = timestart.hour;
    int minute = timestart.minute;
    double second = timestart.second;

    // Calculating amount of leap years between target year since 2000
    int leapCount = 0;
    for (int y = 2000; y < year; y++) {
        if (leapYear(y)){
            leapCount++;
        }
    }
 
    // Number of minutes from 1st Jan 2000 to start of the current year (2000-year)
    int mjd2000_startYear = (year - 2000) * 365 * 1440 + leapCount * 1440;

    // Defining the days in a month
    std::vector<int> dayInAMonth = {30, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Jan has 30 days because index starts from 0, hence day 31 is equal to day 30
    // if current year is a leap one, then change the Feb one to 29
    if (leapYear(year)) {
        dayInAMonth[1] = 29;
    }

    // Computing the fraction of the day of the year
    int wholeDays = 0;
    for (int m = 0; m < (month - 1); m++) { // means Jan == 0, Feb == 1, consistent with index
        wholeDays = wholeDays + dayInAMonth[m]; 
    }
    double dayF = wholeDays + day + double(hour)/24.0 + double(minute)/1440.0 + second/86400.0;

    // Converting it to mins
    double minF = dayF * 1440;

    double mjd2000 = minF + mjd2000_startYear;


    return mjd2000;
}

TimeUTC MJD20002epoch(double mjd_date) {

    double mjd_stdYear = 365*1440; // [min]
    double fraction = mjd_date / mjd_stdYear;
    int guess0 = int(fraction) + 2000;

    int leapCount = 0;
    for (int y = 2000; y < guess0; y++) {
        if (leapYear(y)){
            leapCount++;
        }
    }

    double mjd_tillYear = mjd_stdYear*(guess0 - 2000) + leapCount*1440;

    double remainingOfYear = mjd_date - mjd_tillYear;
    double dayF = remainingOfYear/1440;

    // Defining the days in a month
    std::vector<int> dayInAMonth = {30, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Jan has 30 days because index starts from 0, hence day 31 is equal to day 30
    // if current year is a leap one, then change the Feb one to 29
    if (leapYear(guess0)) {
        dayInAMonth[1] = 29;
    }

    // Computing the fraction of the day of the year
    int index = 0;
    while (int(dayF) > dayInAMonth[index]) {
        dayF = dayF - dayInAMonth[index];
        index++;
    }
    int month = index + 1;

    int dd = int(dayF);
    double spareSecs = (dayF - dd) * 86400;
    double hhF = spareSecs / 3600;
    int hh = int(hhF);
    double mmF = (hhF - hh) * 60;
    int mm = int(mmF);
    double ss = (mmF - mm) * 60;

    return {guess0, month, dd, hh, mm, ss};