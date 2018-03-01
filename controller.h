#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "segmentation.h"
#include <iostream>
#include <opencv2/core.hpp>

class Controller
{
public:
    // Get access to Singleton instance
    static Controller *getInstance() {
        // Creates the instance at first call
        if (singleton == 0)
            singleton = new Controller;
        return singleton;
    }

    // Release singleton instance of this controller
    static void destroy() {
        if (singleton != 0) {
            delete singleton;
            singleton = 0;
        }
    }

    // Delete processor objects created by controller
    ~Controller() {
        delete segmentation;
    }

private:
    //// ATTRIBUTES ////
    // Pointer to singleton
    static Controller *singleton;
    // Segmentation class instance
    Segmentation *segmentation;

    //// METHODS ////
    Controller() {
        // Setting up the application
        segmentation = new Segmentation();
    }
};

#endif // CONTROLLER_H
