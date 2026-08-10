#include <iostream>
#include <stdexcept>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "Json_Parser.h"
#include "Rides_Database.h"

int main() {
    try {
    JsonParser Parser;
    std::cout << "httpGet response: " << Parser.httpGet() << std::endl;
    Parser.printRides();

    RidesDatabase db;
    db.insertNewRides(Parser.getRides());
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

    char exit = '\0';
    std::cout << "Press q and enter to exit" << std::endl;
    while (exit != 'q') {
        std::cin >> exit;
    }

    return 0;
}
