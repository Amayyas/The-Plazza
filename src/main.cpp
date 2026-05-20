/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** main.cpp
*/

/// @file main.cpp
/// @brief Implementation of the main.

#include "PlazzaException.hpp"
#include "MenuLoader.hpp"
#include "Reception.hpp"
#include <exception>
#include <iostream>

#define USAGE "USAGE:\n" \
              "    ./plazza multiplier cooks time [--ticket]\n\n" \
              "DESCRIPTION:\n" \
              "    multiplier    Multiplier factor for the pizza cooking time (positive float).\n" \
              "    cooks         Number of cooks per kitchen (positive integer).\n" \
              "    time          Time in milliseconds for stock replenishment (positive integer).\n" \
              "    --ticket      (Optional) Enables explicit receipt printing at order completion."

int main(int argc, char *const *argv)
{
    bool ticket = false;

    if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        std::cout << USAGE << std::endl;
        return 0;
    }

    if (argc < 4 || argc > 5) {
        std::cerr << "Error: Invalid number of arguments.\n\n" << USAGE << std::endl;
        return 84;
    }

    if (argc == 5) {
        if (std::string(argv[4]) == "--ticket") {
            ticket = true;
        } else {
            std::cerr << "Error: Unknown argument '" << argv[4] << "'.\n\n" << USAGE << std::endl;
            return 84;
        }
    }

    try {
        float multiplier = std::stof(argv[1]);
        int cooksPerKitchen = std::stoi(argv[2]);
        int restockTime = std::stoi(argv[3]);

        if (multiplier <= 0 || cooksPerKitchen <= 0 || restockTime < 0)
            throw Plazza::PlazzaException("Error: Arguments must be positive values.\n");

        Plazza::MenuLoader::loadMenu("config/menu.csv");
        Plazza::Reception reception(multiplier, cooksPerKitchen, restockTime, ticket);

        reception.run();

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
