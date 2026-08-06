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

int RidesDatabase::insertNewRides(std::vector<RideRecord> rides) {

    const char* sql = "INSERT INTO wait_times (ride_name, land_name, wait_time, is_open, recorded_at) "
        "VALUES (?, ?, ?, ?, ?);";
    
    sqlite3_stmt *stmt = nullptr;

    sqlCode = sqlite3_prepare_v2(ridesDb, sql, -1, &stmt, nullptr);

        for (const auto& record : rides)
        {
            sqlite3_bind_text(stmt, 1, record.rideName.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, record.landName.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 3, record.waitTime);
            sqlite3_bind_int(stmt, 4, record.isOpen ? 1 : 0);
            sqlite3_bind_text(stmt, 5, record.recordedAt.c_str(), -1, SQLITE_TRANSIENT);
            
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
        }
    
        sqlite3_finalize(stmt);
    return 0;
}

int RidesDatabase::openDatabase() {
    return 0;
}

int RidesDatabase::closeDatabase() {
    return 0;
}