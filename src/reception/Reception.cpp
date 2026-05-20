/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Reception.cpp
*/

/// @file Reception.cpp
/// @brief Implementation of the Reception class.

#include "Reception.hpp"
#include "OrderParser.hpp"
#include <functional>
#include <iostream>
#include <iomanip>

namespace Plazza {
    Reception::Reception(float cookingMultiplier, int cooksPerKitchen, int restockDelay, bool ticket):
    _multiplier(cookingMultiplier),
    _cooksPerKitchen(cooksPerKitchen),
    _restockDelay(restockDelay),
    _ticket(ticket) {}

    Reception::~Reception() {}

    void Reception::run()
    {
        _running = true;
        shellLoop();
    }

    void Reception::shellLoop()
    {
        std::string line;

        while (_running) {
            std::cout << PROMPT;

            if (!std::getline(std::cin, line) || line == EXIT) {
                _running = false;
                std::cout << EXIT_MESSAGE << std::endl;
                break;
            }

            std::size_t start = line.find_first_not_of(" \t");
            std::size_t end   = line.find_last_not_of(" \t");
            if (start == std::string::npos)
                continue;
            line = line.substr(start, end - start + 1);

            handleCommand(line);
        }
    }

    void Reception::handleCommand(const std::string &line)
    {
        static const std::map<std::string, std::function<void()>> commands = {
            {"status", [this]() { handleStatus(); }},
            {"menu", [this]() { displayMenu(); }}
        };
        
        auto it = commands.find(Tools::toLower(Tools::trim(line)));
        if (it != commands.end()) {
            it->second();
        } else {
            handleOrder(line);
        }
    }

    void Reception::handleOrder(const std::string &line)
    {
        std::vector<std::unique_ptr<IPizza>> pizzas;
    
        try {
            pizzas = OrderParser::parse(line);
        } catch (const PlazzaException &e) {
            std::cerr << e.what() << "\n";
            return;
        }
    
        if (pizzas.empty()) {
            std::cerr << "Empty order.\n";
            return;
        }
    
        if (_ticket) {
            displayReceipt(pizzas);
            std::cout << "[reception] Send " << pizzas.size() << " pizza(s) to kitchens.\n";
        }
    
        for (auto &pizza : pizzas)
            dispatchPizza(std::move(pizza));
    }

    void Reception::handleStatus()
    {
        // ToDo: Displays the kitchens status: current occupancy of the cooks and stocks of ingredients.
    }

    void Reception::displayReceipt(const std::vector<std::unique_ptr<IPizza>> &pizzas)
    {
        float grandTotal = 0.0f;
        
        std::cout << "\n-----------------------------------------\n";
        std::cout << "               MAMATINA\n";
        std::cout << " 1 Rue des Pertuisanes, 34000 Montpellier\n";
        std::cout << "-----------------------------------------\n";
        
        for (auto &pizza : pizzas) {
            std::string typeStr = pizza->getType();
            std::string sizeStr = "U";
            
            for (auto const& [str, s] : pizzaSizes)
                if (s == pizza->getSize()) 
                    sizeStr = str; 
            
            if (!typeStr.empty())
                typeStr[0] = std::toupper(typeStr[0]);
            
            float price = pizzaRecipes.at(pizza->getType()).price;
            grandTotal += price;

            std::string productLine = typeStr + " (" + sizeStr + ")";

            std::cout << std::left << std::setfill('.') << std::setw(30) << productLine 
                      << " " << std::fixed << std::setprecision(2) << price << " €\n";
        }
        
        std::cout << "-----------------------------------------\n";
        std::cout << std::left << std::setfill('.') << std::setw(30) << " TOTAL" 
                  << " " << std::fixed << std::setprecision(2) << grandTotal << " €\n";
        std::cout << "-----------------------------------------\n";
        std::cout << " Tel: 04 99 66 45 09\n";
        std::cout << " Mail: contact@mamatina-plazza.fr\n";
        std::cout << "-----------------------------------------\n\n";

    }

    void Reception::displayMenu()
    {
        if (pizzaRecipes.empty()) {
            std::cout << "\n[Mamatina] Empty menu.\n\n";
            return;
        }

        std::cout << "\n=========================================\n";
        std::cout << "         Pizzeria de LA MAMATINA\n";
        std::cout << "                  Menu\n";
        std::cout << "=========================================\n\n";

        for (const auto &[name, recipe] : pizzaRecipes) {
            std::string pizzaName = name;

            if (!pizzaName.empty())
                pizzaName[0] = std::toupper(pizzaName[0]);

            std::cout << pizzaName << " - " << recipe.price << " €\n";
            std::cout << "    Baking time: " << recipe.baseCookTime << " ms\n";
            std::cout << "    Ingredients: ";

            for (std::size_t i = 0; i < recipe.ingredients.size(); ++i) {
                std::cout << recipe.ingredients[i];
                if (i < recipe.ingredients.size() - 1)
                    std::cout << ", ";
            }
            std::cout << "\n\n";
        }

        std::cout << "Size available: S, M, L, XL, XXL\n";
        std::cout << "-----------------------------------------\n\n";
        std::cout << std::setfill(' ');
    }

    int Reception::getBestKitchen() 
    {
        return 0;
    }

    void Reception::spawnKitchen()
    {

    }

    void Reception::dispatchPizza([[maybe_unused]] std::unique_ptr<IPizza> pizza)
    {

    }
}
