#include "Rides_Database.h"
#include <iostream>

RidesDatabase::RidesDatabase(const char* filename) {
    sqlCode = sqlite3_open_v2(filename, &ridesDb, SQLITE_OPEN_READWRITE, nullptr);

    if (sqlCode != SQLITE_OK) {
        std::cout << "Error opening database: " << sqlite3_errmsg(ridesDb) << std::endl;
        std::cout << "Creating a new database" << std::endl;
        sqlCode = sqlite3_open(filename, &ridesDb);

        if (sqlCode != SQLITE_OK || ridesDb == nullptr) {
            std::cout << "ERROR: Cannot create new database" << std::endl;
        }

        const char* sql = "CREATE TABLE wait_times ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "ride_name TEXT NOT NULL,"
            "land_name TEXT,"
            "wait_time INTEGER,"
            "is_open BOOLEAN,"
            "recorded_at TEXT NOT NULL);";

        sqlCode = sqlite3_exec(ridesDb, sql, nullptr, nullptr, nullptr);
    }

    if (sqlCode == SQLITE_OK) {
        std::cout << "Connection successfull" << std::endl;
    }
}

RidesDatabase::~RidesDatabase() {
    std::cout << "Closing Database..." << std::endl;
    sqlCode = sqlite3_close(ridesDb);

    if (sqlCode != SQLITE_OK) {
        std::cout << "SQL Database can't close properly" << std::endl;
    }
}

void RidesDatabase::getCurrentRides() {

}

int RidesDatabase::insertNewRides(/*container*/) {
    return 0;
}

int RidesDatabase::openDatabase() {
    return 0;
}

int RidesDatabase::closeDatabase() {
    return 0;
}