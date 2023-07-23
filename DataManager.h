// DataManager.h

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>

using namespace std;

#include "Time.h"

class DataManager {
private:
    string id;
    string destination;
    Time timing; // Use the Time class for timing

public:
    DataManager(const std::string& id, const std::string& destination, int hours, int minutes);
    virtual ~DataManager();

    const string& getId() const;
    const string& getDestination() const;
    const Time& getTiming() const;

    void setId(const std::string& newId);
    void setDestination(const std::string& newDestination);
    void setTiming(int hours, int minutes);

    virtual void display() const = 0;
};

#endif // DATAMANAGER_H