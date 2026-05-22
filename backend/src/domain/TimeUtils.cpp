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
    // Month validation
    if (time.month < 1 || time.month > 12) // 0 or 13
        throw std::invalid_argument("Invalid month.");
    // Days validation - general
    if (time.day < 1 || time.day > 31) {
        throw std::invalid_argument("Invalid day.");
    }
    // Days validation - 30 days months or 28/29 days month
    bool isLeap = leapYear(time.year);
    switch (time.month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            if (time.day < 1 || time.day > 31) {
                throw std::invalid_argument("Invalid day.");
            }
            break;
        case 4: case 6: case 9: case 11:
            if (time.day < 1 || time.day > 30) {
                throw std::invalid_argument("Invalid day.");
            }
            break;
        case 2:
            if (isLeap) {
                if (time.day < 1 || time.day > 29) {
                    throw std::invalid_argument("Invalid day.");
                }
            } else {
                if (time.day < 1 || time.day > 28) {
                    throw std::invalid_argument("Invalid day.");
                }
            }
            break;
        default:
            break;
    }
    // Hour validation
    if (time.hour < 0 || time.hour > 23) // -1 or 24 (should be next day in that case)
        throw std::invalid_argument("Invalid hour.");
    // Minute validation 
    if (time.minute < 0 || time.minute > 59) // -1 or 60 (should be next hour in that case)
        throw std::invalid_argument("Invalid minute.");
    // second validation
    if (time.second < 0.0 || time.second > 59.0) // -1 or 60.0 (should be next minute in that case)
        throw std::invalid_argument("Invalid second.");
}

// ===============================

// === VALIDATED =================
double jd(Tle tle){
    // Definition from Vallado's sgp4init: days from 1st Jan 1950 00:00:00

    int year2digits = std::stoi(tle.line1.substr(18,2));
    int year;
    if (year2digits >= 57) {
        year = 1900 + year2digits;
    } else {
        year = 2000 + year2digits;
    }
    double dayOfYear = std::stod(tle.line1.substr(20,12));

    // Computing amount of lead years
    int leapDays = 0;
    for (int i = 1950; i < year; i++) { 
        if (leapYear(i)) {
            leapDays++;
        }
    }

    // Compute days and fraction of days
    double jd; // [days]
    jd = (year - 1950) * 365.0 + dayOfYear + leapDays - 1.0; // start counting from 0 on 1st Jan, not 1

    return jd; // [days]
}

// ===============================

double epoch2MJD2000_TLE(Tle tle) {
    // Fetch year and doy from tle object
    int year = std::stoi(tle.line1.substr(18,2)) + 2000; // after year 2000
    double dayOfYear = std::stod(tle.line1.substr(20,12));

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
    double TLE_Days = totYears*365 + leapCount + dayOfYear - 1.0; // the year start with 1st Jan but the count start from 0.0

    // Converting to minutes
    double TLE_Minutes = double(TLE_Days) * 1440.0;

    return TLE_Minutes;
};

// ===============================

// === VALIDATED =================
double epoch2mins(const TimeUTC& timestart) {

    int year = timestart.year;
    int month = timestart.month;
    int day = timestart.day;
    int hour = timestart.hour;
    int minute = timestart.minute;
    double second = timestart.second;

    // Computing amount of lead years
    int leapDays = 0;
    for (int i = 2000; i < year; i++) { 
        if (leapYear(i)) {
            leapDays++;
        }
    }

    // Number of days from 01/01/2000 till start or year
    int nDays = (year - 2000) * 365 + leapDays;

    // Defining the vector of days per month (Jan has 30 because we start counting from 0)
    std::vector<int> dayInAMonth = {30, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (leapYear(year)) {dayInAMonth[1] = 29;}

    // Number of days till (month-1)
    int previousMonth = month - 1;
    int daysTillMonth = 0;
    for (int i = 0; i < previousMonth; i++) {
        daysTillMonth += dayInAMonth[i];
    }

    // Total days
    double D = nDays + daysTillMonth + day + hour/24.0 + minute/1440.0 + second/86400.0;

    // Total minutes
    double M = D * 1440.0;
    return M;
}

TimeUTC MJD20002epoch(double mjd_date) {

    int min_nonLeap = 365 * 1440; // [min]
    double fraction = mjd_date / min_nonLeap;
    int guess0 = int(fraction) + 2000;

    int leapCount = 0;
    for (int y = 2000; y < guess0; y++) {
        if (leapYear(y)){
            leapCount++;
        }
    }

    double min_g0 =((guess0 - 2000) * 365.0 + leapCount - 1.0) * 1440.0;

    int year;
    if (min_g0 <= mjd_date) {
        year = guess0;
    } else {
        year = guess0 - 1;
        min_g0 =((year - 2000) * 365.0 + leapCount - 1.0) * 1440.0;
    }

    double rem = mjd_date - min_g0;

    double dayFrac = rem / 1440.0;
    int day = int(dayFrac);

    // Defining the vector of days per month
    std::vector<int> dayInAMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (leapYear(year)) {dayInAMonth[1] = 29;}

    // Computing the fraction of the day of the year
    int index = 0;
    while (day > dayInAMonth[index]) {
        day = day - dayInAMonth[index];
        index++;
    }
    int month = index + 1;
    
    // Days till month
    int totDays = 0;
    for (int i = 0; i < index; i++) {
        totDays += dayInAMonth[i];
    }

    float remS = (dayFrac - int(dayFrac)) * 86400.0; // [sec]
    float hourFrac = remS / 3600.0;
    int hour = hourFrac;
    
    remS -= hour * 3600.0;
    int minute = remS / 60.0;
    double seconds = remS - minute * 60.0;

    return {year, month, day, hour, minute, seconds};
}

// Conversion from TimeUTC to String
std::string to_iso8601(TimeUTC time) {
    char buf[32];
    std::snprintf(buf, sizeof(buf),
        "%04d-%02d-%02dT%02d:%02d:%06.3fZ",
        time.year, time.month, time.day, time.hour, time.minute, time.second);
    return buf;
}