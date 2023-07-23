// UserInterface.h

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "AutoDeliver.h"

class UserInterface {
private:
    void start();

public:
    bool fileExists(const std::string& filePath);
    bool isPathValid(const std::string& path);

    void addDrone(AutoDeliver& autoDeliver);
    void editDrone(AutoDeliver& autoDeliver);
    void deleteDrone(AutoDeliver& autoDeliver);

    void addPackage(AutoDeliver& autoDeliver);
    void editPackage(AutoDeliver& autoDeliver);
    void deletePackage(AutoDeliver& autoDeliver);

    void generateAndDisplayPlan(AutoDeliver& autoDeliver);
    void savePlan(AutoDeliver& autoDeliver);

    void run() {
        start();
    };

};

#endif // USERINTERFACE_H
