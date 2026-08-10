#pragma once

#include <sqlite3.h>
#include <vector>
#include "Ride_Record.h"

class RidesDatabase {
private:
    sqlite3 *ridesDb;
    const char *dbName;

public:
    RidesDatabase(const char* filename = "../Energy_Rides_DB");

    void insertNewRides(std::vector<RideRecord> rides);
    void getCurrentRides();
    int openDatabase();
    int closeDatabase();

    ~RidesDatabase();
};