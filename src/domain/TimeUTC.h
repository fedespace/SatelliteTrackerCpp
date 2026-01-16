#pragma once

struct TimeUTC {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;

    // Introduce validation on time components. The function is of type void because it is not returning a value.
    void validate() const {
        if (month < 1 || month > 12) 
            throw std::invalid_argument("Invalid month.");
        if (day < 1 || day > 31)
            throw std::invalid_argument("Invalid day.");
        if (hour < 0 || hour > 24)
            throw std::invalid_argument("Invalid hour.");
        if (minute < 0 || minute > 60)
            throw std::invalid_argument("Invalid minute.");
        if (second < 0.0 || second > 59.0)
            throw std::invalid_argument("Invalid second.");
        
        // Will need to add a check for every single month then especially february (?)
    }
};