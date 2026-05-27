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
    static constexpr std::size_t STOCK_CAPACITY = 5;
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
        _regenThread.start(&Kitchen::stockRegenerationLoop, this);

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

                if (!message.empty()) {
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
        
        _stockMutex.lock();
        _cvStock.notifyAll();
        _stockMutex.unlock();

        if (_regenThread.joinable())
            _regenThread.join();

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

            _stockMutex.lock();
            while (!tryConsumeIngredients(pizza.getIngredients()) && _running)
                _cvStock.wait(_stockMutex); 

            if (!_running) {
                _stockMutex.unlock();
                break;
            }
            _stockMutex.unlock();

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

    void Kitchen::stockRegenerationLoop()
    {
        while (_running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(_regenerationTime));

            _stockMutex.lock();
            for (auto& [ingredientName, quantity] : _stock)
                if (quantity < STOCK_CAPACITY)
                    quantity++;

            _cvStock.notifyAll();
            _stockMutex.unlock();
        }
    }

    bool Kitchen::tryConsumeIngredients(const std::vector<std::string>& ingredients)
    {
        for (const auto& reqIngredient : ingredients)
            if (_stock[reqIngredient] == 0)
                return false;

        for (const auto& reqIngredient : ingredients)
            _stock[reqIngredient]--;
        return true;
    }
}
