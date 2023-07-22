// Time.h
#ifndef TIME_H
#define TIME_H

#include <string>
#include <sstream>
#include <iomanip>

class Time {
private:
    int hours;
    int minutes;

public:
    Time(int hours, int minutes);

    bool isValidTime() const;
    std::string toString() const;

    int getHours() const;
    int getMinutes() const;
    int getTotalMinutes() const;

};

#endif // TIME_H