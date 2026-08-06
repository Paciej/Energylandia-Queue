#include <iostream>
#include <sqlite3.h>

class RidesDatabase {
private:
    sqlite3 *ridesDb;
    int sqlCode;

public:
    RidesDatabase(const char* filename = "Energy_Rides_DB");

    int insertNewRides(/*container*/);
    void getCurrentRides();
    int openDatabase();
    int closeDatabase();

    ~RidesDatabase() {
        sqlCode = sqlite3_close(ridesDb);

        if (sqlCode != SQLITE_OK) {
            std::cout << "SQL Database can't close properly" << std::endl;
        }
    }
};