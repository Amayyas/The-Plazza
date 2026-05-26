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
#include <chrono>
#include <unistd.h>

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

            for (auto it = _kitchens.begin(); it != _kitchens.end(); it++) {
                if (it->ipc.hasData(0)) {
                    std::string message;
                    it->ipc >> message;

                    if (message == "DONE" && it->currentLoad > 0)
                        it->currentLoad--;
                }
            }

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
        if (_kitchens.empty()) {
            std::cout << "No active kitchens at the moment." << std::endl;
            return;
        }

        std::cout << "\n=========================================\n";
        std::cout << "                  MAMATINA\n";
        std::cout << "               Kitchen Status\n";
        std::cout << "=========================================\n\n";

        for (std::size_t i = 0; i < _kitchens.size(); i++) {
            std::cout << "[Kitchen #" << _kitchens.at(i).pid << "]" << std::endl;
        
            _kitchens.at(i).ipc << "STATUS";

            std::string response;
            auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);

            while (std::chrono::steady_clock::now() < deadline) {
                if (_kitchens[i].ipc.hasData(50)) {
                    _kitchens[i].ipc >> response;
                    if (response == "DONE") {
                        if (_kitchens[i].currentLoad > 0)
                            _kitchens[i].currentLoad--;
                    } else {
                        std::cout << response;
                        break;
                    }
                }
            }
        }

        std::cout << "=========================================\n" << std::endl;
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
        int bestKitchenIdx = -1;
        int minLoad = -1;
        std::size_t maxCapacity = 2 * _cooksPerKitchen;

        if (_kitchens.empty())
            return -1;

        for (std::size_t i = 0; i < _kitchens.size(); i++) {
            std::size_t currentLoad = _kitchens.at(i).currentLoad;

            if (currentLoad < maxCapacity) {
                if (minLoad == -1 || static_cast<int>(currentLoad) < minLoad) {
                    minLoad = currentLoad;
                    bestKitchenIdx = i;
                }
            }
        }
        return bestKitchenIdx;
    }

    void Reception::spawnKitchen()
    {
        IPC kitchenIPC;
        pid_t pid = fork();

        if (pid == -1)
            throw PlazzaException("Failed to fork new kitchen");

        if (pid == 0) { // Child
            kitchenIPC.setChildMode();
            Kitchen kitchen(_cooksPerKitchen, _multiplier, _restockDelay, std::move(kitchenIPC));

            kitchen.run();
            std::exit(0);
        } else { // Parent
            kitchenIPC.setParentMode();

            KitchenProxy_t newKitchen;
            newKitchen.pid = pid;
            newKitchen.ipc = std::move(kitchenIPC);
            newKitchen.currentLoad = 0;

            _kitchens.push_back(std::move(newKitchen));
        }
    }

    void Reception::dispatchPizza([[maybe_unused]] std::unique_ptr<IPizza> pizza)
    {
        int index = getBestKitchen();

        if (index == -1) {
            spawnKitchen();
            index = _kitchens.size() - 1;
        }

        std::string serializedPizza = PizzaSerializer::pack(*pizza);

        _kitchens[index].ipc << serializedPizza;
        _kitchens[index].currentLoad++;
    }
}
