// Time.cpp
#include "Time.h"

Time::Time(int hours, int minutes) : hours(hours), minutes(minutes) {}

bool Time::isValidTime() const {
    return (hours >= 0 && hours < 24) && (minutes >= 0 && minutes < 60);
}

std::string Time::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes;
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