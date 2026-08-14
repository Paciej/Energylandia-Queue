#pragma once

#include <sqlite3.h>
#include <vector>
#include "Ride_Record.h"

class RidesDatabase {
private:
    sqlite3 *ridesDb;
    const char *dbName;

public:
    RidesDatabase(const char* filename = "../Energy_Rides.db");

    void insertNewRides(std::vector<RideRecord> rides);
    std::vector<RideRecord> getRidesLatest();
    std::vector<RideAverage> getRidesAvg(const std::string& date);
    int openDatabase();
    int closeDatabase();

    ~RidesDatabase();
};