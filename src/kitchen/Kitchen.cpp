/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Kitchen.cpp
*/

/// @file Kitchen.cpp
/// @brief Implementation of the Kitchen class.

#include "Kitchen.hpp"
#include <sstream>
#include <iomanip>

namespace Plazza {
    static constexpr std::size_t KITCHEN_TIMEOUT = 5;

    Kitchen::Kitchen(std::size_t numCooks, double multiplier, std::size_t regenTime, IPC ipc) :
    _numCooks(numCooks), _timeMultiplier(multiplier), _regenerationTime(regenTime),
    _ipc(std::move(ipc)), _running(true), _activePizzas(0)
    {
        MenuLoader::loadMenu("config/menu.csv");
        if (allKnownIngredients.empty())
            throw PlazzaException("No known ingredients.");

        for (const auto& ingredient : allKnownIngredients)
            _stock[ingredient] = 5;

        _cookStates.resize(numCooks);
        _lastActiveTime = std::chrono::steady_clock::now();
    }

    Kitchen::~Kitchen()
    {
        _pizzaQueue.close();
    }

    void Kitchen::run()
    {
        for (std::size_t i = 0; i < _numCooks; ++i) {
            auto cook = std::make_unique<Plazza::Thread>();
            cook->start(&Kitchen::cookThreadLoop, this, i);
            _cookThreads.push_back(std::move(cook));
        }

        _activityMutex.lock();
        _lastActiveTime = std::chrono::steady_clock::now();
        _activityMutex.unlock();

        while (_running) {
            if (_ipc.hasData(1000)) {
                std::string message;
                _ipc >> message;

                if (message == "STATUS") {
                    std::string report = this->generateStatusReport();
                    _ipcMutex.lock();
                    _ipc << report;
                    _ipcMutex.unlock();
                } else if (!message.empty()) {
                    _activityMutex.lock();
                    _activePizzas++;
                    _lastActiveTime = std::chrono::steady_clock::now();
                    _activityMutex.unlock();

                    try {
                        Plazza::PizzaOrder order = PizzaSerializer::unpack(message);

                        Plazza::Pizza pizza(order.type, order.size);
                        
                        _pizzaQueue.push(pizza);
                    } catch (const std::exception& e) {
                        std::cerr << "[Kitchen] Error creating pizza: " << e.what() << std::endl;
                        
                        _activityMutex.lock();
                        _activePizzas--;
                        _activityMutex.unlock();
                    }
                }
            }

            // Timeout check (5 seconds)
            _activityMutex.lock();
            if (_activePizzas == 0) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - _lastActiveTime).count();
                
                if (static_cast<std::size_t>(elapsed) >= KITCHEN_TIMEOUT)
                    _running = false;
            }
            _activityMutex.unlock();
        }

        _pizzaQueue.close();

        for (auto& cook : _cookThreads)
            if (cook->joinable())
                cook->join();
    }

    void Kitchen::cookThreadLoop(std::size_t cookId)
    {
        while (_running) {
            Pizza pizza("", PizzaSize::S);

            if (!_pizzaQueue.pop(pizza))
                break;

            std::size_t totalCookTimeMs = pizza.getCookTime() * _timeMultiplier;

            _cookStatesMutex.lock();
            _cookStates[cookId] = {true, pizza.getType(),
                                std::chrono::steady_clock::now(), totalCookTimeMs};
            _cookStatesMutex.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(totalCookTimeMs));

            _cookStatesMutex.lock();
            _cookStates[cookId] = {false, "", {}, 0};
            _cookStatesMutex.unlock();

            _ipcMutex.lock();
            _ipc << "DONE";
            _ipcMutex.unlock();

            _activityMutex.lock();
            _activePizzas--;
            _lastActiveTime = std::chrono::steady_clock::now();
            _activityMutex.unlock();
        }
    }

    std::string Kitchen::generateStatusReport()
    {
        std::string report;

        report += "  Cooks:\n";
        _cookStatesMutex.lock();
        for (std::size_t i = 0; i < _cookStates.size(); i++) {
            const auto& state = _cookStates.at(i);

            report += "    [Cook #" + std::to_string(i) + "] ";
            if (state.busy) {
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - state.startTime).count();

                double remaining = (static_cast<double>(state.totalCookTimeMs) - elapsed) / 1000.0;
                if (remaining < 0.0)
                    remaining = 0.0;
                
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(2) << remaining;
                report += "Baking " + state.pizzaName + " (" + oss.str() + "s remaining)\n";
            } else {
                report += "Waiting\n";
            }
        }
        _cookStatesMutex.unlock();

        auto queued = _pizzaQueue.getItems();
        report += "  Queue (" + std::to_string(queued.size()) + " pizza(s)):\n";
        for (const auto& pizza : queued)
            report += "    - " + pizza.getType() + "\n";

        report += "  Ingredients stock:\n";

        return report;
    }
}
