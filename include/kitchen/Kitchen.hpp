/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Kitchen.hpp
*/

/// @file Kitchen.hpp
/// @brief Kitchen class definition

#pragma once

#include <iostream>

#include "IPC.hpp"
#include "Pizza.hpp"
#include "Thread.hpp"
#include "SafeQueue.hpp"
#include "MenuLoader.hpp"
#include "PizzaFactory.hpp"
#include "PizzaSerializer.hpp"
#include "ConditionVariable.hpp"

namespace Plazza {
    typedef struct {
        pid_t pid;
        IPC ipc;
        int currentLoad = 0;
    } KitchenProxy_t;

    struct CookState {
        bool busy = false;
        std::string pizzaName;
        std::chrono::steady_clock::time_point startTime;
        std::size_t totalCookTimeMs = 0;
    };

    class Kitchen {
    public:
        Kitchen(std::size_t numCooks, double multiplier, std::size_t regenTime, IPC ipc);
        ~Kitchen();

        void run();

    private:
        void cookThreadLoop(std::size_t cookId);
        void stockRegenerationLoop();
        bool tryConsumeIngredients(const std::vector<std::string>& ingredients);
        std::string generateStatusReport();

        std::size_t _numCooks;
        double _timeMultiplier;
        std::size_t _regenerationTime;
        
        IPC _ipc;
        Mutex _ipcMutex;

        std::vector<std::unique_ptr<Thread>> _cookThreads;
        Thread _regenThread;
        
        Mutex _stockMutex;
        ConditionVariable _cvStock;
        
        Mutex _activityMutex;

        SafeQueue<Pizza> _pizzaQueue;
        std::map<std::string, std::size_t> _stock;

        bool _running;

        std::size_t _activePizzas; 
        std::chrono::steady_clock::time_point _lastActiveTime;
    
        std::vector<CookState> _cookStates;
        Mutex _cookStatesMutex;
    };
} // namespace Plazza
