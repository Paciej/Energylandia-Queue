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

    ~RidesDatabase();
};