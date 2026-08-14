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
                            "recorded_at TEXT NOT NULL);"
        ;

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

std::vector<RideRecord> RidesDatabase::getRidesLatest() {

    const char* sql = "SELECT * "
        "FROM wait_times w "
        "WHERE recorded_at = ("
        " SELECT MAX(w2.recorded_at) "
        " FROM wait_times w2 "
        " WHERE w2.ride_name = w.ride_name"
        ");"
    ;

    std::vector<RideRecord> rides;

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(ridesDb, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(std::string("Error preparing transaction: ") + sqlite3_errmsg(ridesDb));
    }

    int rc = 0;

    try {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            RideRecord record = {};
            record.rideName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            record.landName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            record.waitTime = sqlite3_column_int(stmt, 3);
            record.isOpen   = static_cast<bool>(sqlite3_column_int(stmt, 4));
            record.recordedAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            rides.push_back(record);
        }

        if (rc != SQLITE_DONE) {
            throw std::runtime_error(std::string("Error reading rows: ") + sqlite3_errmsg(ridesDb));
        }
    }
    catch (...) {
        sqlite3_finalize(stmt);
        throw;
    }

    sqlite3_finalize(stmt);
    return rides;
}

std::vector<RideAverage> RidesDatabase::getRidesAvg(const std::string& date) {

    const char* sql = "SELECT ride_name, land_name, AVG(wait_time) "
        "FROM wait_times "
        "WHERE DATE(recorded_at) = ? AND is_open = 1 "
        "GROUP BY ride_name, land_name "
        "ORDER BY land_name;"
    ;

    std::vector<RideAverage> rides;
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(ridesDb, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(std::string("Error preparing statement: ") + sqlite3_errmsg(ridesDb));
    }

    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_TRANSIENT);

    int rc = 0;
    try {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            RideAverage avg = {};
            avg.rideName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            avg.landName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            avg.avgWait  = sqlite3_column_double(stmt, 2);
            rides.push_back(avg);
        }

        if (rc != SQLITE_DONE) {
            throw std::runtime_error(std::string("Error reading rows: ") + sqlite3_errmsg(ridesDb));
        }
    }
    catch (...) {
        sqlite3_finalize(stmt);
        throw;
    }

    sqlite3_finalize(stmt);
    return rides;
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
            
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw std::runtime_error(std::string("Error making transaction: ") +
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