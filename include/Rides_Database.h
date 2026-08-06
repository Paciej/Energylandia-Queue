#pragma once

#include <sqlite3.h>
#include <vector>
#include "Ride_Record.h"

class RidesDatabase {
private:
    sqlite3 *ridesDb;
    int sqlCode;

public:
    RidesDatabase(const char* filename = "Energy_Rides_DB");

    int insertNewRides(std::vector<RideRecord> rides);
    void getCurrentRides();
    int openDatabase();
    int closeDatabase();

    ~RidesDatabase();
};