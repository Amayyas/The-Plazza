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
#include <sstream>
#include <chrono>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>

namespace Plazza {
    Reception::Reception(float cookingMultiplier, int cooksPerKitchen, int restockDelay, bool ticket):
    _multiplier(cookingMultiplier),
    _cooksPerKitchen(cooksPerKitchen),
    _restockDelay(restockDelay),
    _ticket(ticket)
    {
        _logFile.open("plazza.log", std::ios::app);
    }

    Reception::~Reception()
    {
        for (auto &kitchen : _kitchens)
            if (kill(kitchen.pid, 0) == 0)
                kitchen.ipc << "EXIT";

        for (auto &kitchen : _kitchens) {
            int status = 0;
            waitpid(kitchen.pid, &status, 0);
        }

        _kitchens.clear();

        if (_logFile.is_open())
            _logFile.close();
    }

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

            for (auto it = _kitchens.begin(); it != _kitchens.end();) {
                if (it->ipc.hasData(0)) {
                    std::string message;
                    it->ipc >> message;

                    if (message.rfind("DONE:", 0) == 0) {
                        notifyPizzaReady(message.substr(5));
                        if (it->currentLoad > 0)
                            it->currentLoad--;
                    }
                }

                int status;
                pid_t result = waitpid(it->pid, &status, WNOHANG);
                if (result > 0)
                    it = _kitchens.erase(it);
                else
                    it++;
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
        try {
            pizzasOrdered = OrderParser::parse(line);
        } catch (const PlazzaException &e) {
            std::cerr << e.what() << "\n";
            return;
        }
    
        if (pizzasOrdered.empty()) {
            std::cerr << "Empty order.\n";
            return;
        }
    
        if (_ticket) {
            displayReceipt(pizzasOrdered);
            std::cout << "[reception] Send " << pizzasOrdered.size() << " pizza(s) to kitchens.\n";
        }
    
        for (std::size_t i = 0; i < pizzasOrdered.size(); i++)
            if (getBestKitchen() == -1)
                spawnKitchen();

        for (auto &pizza : pizzasOrdered) {
            int index = getBestKitchen();
            std::string serializedPizza = PizzaSerializer::pack(*pizza);

            _kitchens[index].ipc << serializedPizza;
            _kitchens[index].currentLoad++;
        }

        pizzasOrdered.clear();
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
                    if (response.rfind("DONE:", 0) == 0) {
                        notifyPizzaReady(response.substr(5));
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
        auto orderTime = std::chrono::system_clock::now();
        std::time_t orderTimeT = std::chrono::system_clock::to_time_t(orderTime);
        char startBuf[32];
        std::strftime(startBuf, sizeof(startBuf), "%H:%M:%S", std::localtime(&orderTimeT));

        std::size_t maxCookMs = 0;
        for (const auto &pizza : pizzas) {
            std::size_t t = static_cast<std::size_t>(pizza->getCookTime() * _multiplier);
            if (t > maxCookMs)
                maxCookMs = t;
        }
        auto estimatedFinish = orderTime + std::chrono::milliseconds(maxCookMs);
        std::time_t estimatedFinishT = std::chrono::system_clock::to_time_t(estimatedFinish);
        char endBuf[32];
        std::strftime(endBuf, sizeof(endBuf), "%H:%M:%S", std::localtime(&estimatedFinishT));

        float grandTotal = 0.0f;

        std::cout << "\n-----------------------------------------\n";
        std::cout << "               MAMATINA\n";
        std::cout << " 1 Rue des Pertuisanes, 34000 Montpellier\n";
        std::cout << "-----------------------------------------\n";
        std::cout << " Order placed : " << startBuf << "\n";
        std::cout << " Est. ready   : " << endBuf
                  << " (+" << maxCookMs << " ms)\n";
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
        std::cout << std::setfill(' ');
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

            if (_logFile.is_open())
                _logFile.close();

            pizzasOrdered.clear();
            pizzasOrdered.shrink_to_fit();

            _kitchens.clear();
            _kitchens.shrink_to_fit();

            {
                Kitchen kitchen(_cooksPerKitchen, _multiplier, _restockDelay, std::move(kitchenIPC));
                kitchen.run();
            }

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

    void Reception::dispatchPizza(std::unique_ptr<IPizza> pizza)
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

    void Reception::notifyPizzaReady(const std::string &serializedPizza)
    {
        std::string pizzaName = "Unknown";
        std::string sizeStr   = "?";

        try {
            PizzaOrder order = PizzaSerializer::unpack(serializedPizza);
            pizzaName = order.type;
            if (!pizzaName.empty())
                pizzaName[0] = std::toupper(pizzaName[0]);
            for (const auto &[str, s] : pizzaSizes)
                if (s == order.size)
                    sizeStr = str;
        } catch (...) {}

        std::string entry = pizzaName + " (" + sizeStr + ") is ready!";
        std::cout << "\n[Reception] " << entry << "\n" << PROMPT << std::flush;

        if (_logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);
            char buf[32];
            std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
            _logFile << "[" << buf << "] " << entry << "\n";
            _logFile.flush();
        }
    }
}
