// Time.cpp

using namespace std;

#include "Time.h"

Time::Time(int hours, int minutes) : hours(hours), minutes(minutes) {}

bool Time::isValidTime() const {
    return (hours >= 0 && hours < 24) && (minutes >= 0 && minutes < 60);
}

string Time::toString() const {
    ostringstream oss;
    oss << setfill('0') << setw(2) << hours << ":" << setw(2) << minutes;
    return oss.str();
}

int Time::getHours() const {
    return hours;
}

int Time::getMinutes() const {
    return minutes;
}

int Time::getTotalMinutes() const {
    return hours * 60 + minutes;
}