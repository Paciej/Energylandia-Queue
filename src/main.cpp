#include <iostream>
#include <stdexcept>
#include "Json_Parser.h"
#include "Rides_Database.h"

int main(int argc, char** argv) {

    JsonParser Parser;
    RidesDatabase db;

    if (argc < 2) {    
        try {
            u_int counter = 0;
            while (true) {
                std::cout << "httpGet response: " << Parser.httpGet() << std::endl;
                //Parser.printRides();

                db.insertNewRides(Parser.getRides());
                std::cout << "Inserted rides: " << ++counter << std::endl;
                Sleep(600000);
            }
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << "RUNTIME ERROR: " << e.what() << "\n";
            return 1;
        }
        catch (const std::exception& e)
        {
            std::cerr << "UNEXPECTED ERROR: " << e.what() << "\n";
            return 1;
        }
    } else {
        try {
        Parser.printRides(db.getRidesLatest());
        Parser.printRides(db.getRidesAvg("2026-08-10"));
        }
        catch (const std::runtime_error& e) {
            std::cerr << "RUNTIME ERROR: " << e.what() << std::endl;
        }
        catch (...) {
            std::cout << "error occured" << std::endl;
            return 1;
        }
    }

    char exit = '\0';
    std::cout << "Press q and enter to exit" << std::endl;
    while (exit != 'q') {
        std::cin >> exit;
    }

    return 0;
}
