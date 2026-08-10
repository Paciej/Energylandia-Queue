#include "Rides_Database.h"
#include <iostream>
#include <stdexcept>
#include <string>

RidesDatabase::RidesDatabase(const char* filename) {
    if (sqlite3_open_v2(filename, &ridesDb, SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK) {
        std::cout << "Error opening database: " << sqlite3_errmsg(ridesDb) << std::endl;
        std::cout << "Creating a new database" << std::endl;

        if (sqlite3_open(filename, &ridesDb) != SQLITE_OK || ridesDb == nullptr) {
            throw std::runtime_error(std::string("Cannot create new database: ") + 
                    sqlite3_errmsg(ridesDb));
        }

        const char* sql = "CREATE TABLE wait_times ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "ride_name TEXT NOT NULL,"
            "land_name TEXT,"
            "wait_time INTEGER,"
            "is_open BOOLEAN,"
            "recorded_at TEXT NOT NULL);";

        if (sqlite3_exec(ridesDb, sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
            sqlite3_exec(ridesDb, "ROLLBACK;", nullptr, nullptr, nullptr);
            throw std::runtime_error(std::string("Error creating table: ") + 
                    sqlite3_errmsg(ridesDb));
        }
    }

    this->dbName = filename;
    std::cout << "Connection successfull" << std::endl;
}

RidesDatabase::~RidesDatabase() {
    std::cout << "Closing Database..." << std::endl;

    if (sqlite3_close(ridesDb) != SQLITE_OK) {
        std::cout << "SQL Database cannot close properly" << std::endl;
    }
}

void RidesDatabase::getCurrentRides() {

}

void RidesDatabase::insertNewRides(std::vector<RideRecord> rides) {

    if (rides.empty()) {
        return;
    }

    char *errMsg = nullptr; 

    if (sqlite3_exec(ridesDb, "BEGIN TRANSACTION;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string msg = errMsg ? errMsg : "Unknown error";
        sqlite3_free(errMsg);
        throw std::runtime_error("Error beginning transaction: " + msg);
    }

    const char* sql = "INSERT INTO wait_times (ride_name, land_name, wait_time, is_open, recorded_at) "
        "VALUES (?, ?, ?, ?, ?);";
    
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(ridesDb, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_exec(ridesDb, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error(std::string("Error preparing transaction: ") + 
                sqlite3_errmsg(ridesDb));
    }

    try 
    {
        for (const auto& record : rides)
        {
            sqlite3_bind_text(stmt, 1, record.rideName.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, record.landName.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 3, record.waitTime);
            sqlite3_bind_int(stmt, 4, record.isOpen ? 1 : 0);
            sqlite3_bind_text(stmt, 5, record.recordedAt.c_str(), -1, SQLITE_TRANSIENT);
            
            int rc = sqlite3_step(stmt);

            if (rc != SQLITE_DONE) {
                throw std::runtime_error(std::string("Error making transaction: ") +
                        std::to_string(rc) + 
                        sqlite3_errmsg(ridesDb));
            }
            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
        }
    }
    catch (...)
    {
        sqlite3_finalize(stmt);
        sqlite3_exec(ridesDb, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw;
    }   

    sqlite3_finalize(stmt);
    
    if (sqlite3_exec(ridesDb, "COMMIT;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string msg = errMsg ? errMsg : "Unknown error";
        sqlite3_free(errMsg);
        throw std::runtime_error("Error acceptting transaction: " + msg);
    }
}

int RidesDatabase::openDatabase() {
    return 0;
}

int RidesDatabase::closeDatabase() {
    return 0;
}