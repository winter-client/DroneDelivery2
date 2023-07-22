// Package.h
#ifndef PACKAGE_H
#define PACKAGE_H

#include "DataManager.h"

class Package : public DataManager {
public:
    Package(const std::string& id, const std::string& destination, int hours, int minutes);
    void display() const override;

    // Declare DataManager as a friend class to access private members
    friend class DataManager;
};

#endif // PACKAGE_H