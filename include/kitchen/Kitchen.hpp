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
#include <atomic>

#include "IPC.hpp"
#include "Pizza.hpp"
#include "Thread.hpp"
#include "SafeQueue.hpp"
#include "MenuLoader.hpp"
#include "PizzaFactory.hpp"
#include "PizzaSerializer.hpp"
#include "ConditionVariable.hpp"

namespace Plazza {
    ///
    /// @struct KitchenProxy_t
    /// @brief Reception-side handle for a running Kitchen child process.
    ///
    struct KitchenProxy_t {
        pid_t pid;           ///< PID of the Kitchen child process.
        IPC   ipc;           ///< IPC channel to communicate with the kitchen.
        int   currentLoad = 0; ///< Number of pizzas currently queued or cooking.
    };

    ///
    /// @struct CookState
    /// @brief Snapshot of a single cook thread's activity, used for status reporting.
    ///
    struct CookState {
        bool   busy           = false; ///< True while the cook is baking a pizza.
        std::string pizzaName;         ///< Name of the pizza being baked (empty if idle).
        std::chrono::steady_clock::time_point startTime; ///< When baking started.
        std::size_t totalCookTimeMs   = 0; ///< Total expected baking duration in ms.
    };

    ///
    /// @class Kitchen
    /// @brief Child-process entity that manages a pool of cook threads and an ingredient stock.
    ///
    /// Each Kitchen runs in its own process (forked by Reception). It maintains N cook
    /// threads (thread pool), a thread-safe pizza queue, and a stock of ingredients that
    /// replenishes at a configurable rate. It communicates with Reception via IPC.
    ///
    class Kitchen {
    public:
        /// @brief Construct and initialise a Kitchen.
        /// @param numCooks    Number of cook threads to spawn.
        /// @param multiplier  Cooking time multiplier (applied to each pizza's base cook time).
        /// @param regenTime   Stock replenishment interval in milliseconds.
        /// @param ipc         IPC channel (already configured for child mode).
        Kitchen(std::size_t numCooks, double multiplier, std::size_t regenTime, IPC ipc);

        /// @brief Destroy the Kitchen, joining all threads.
        ~Kitchen();

        /// @brief Start the kitchen event loop (blocks until the kitchen shuts down).
        void run();

    private:
        void cookThreadLoop(std::size_t cookId);
        void stockRegenerationLoop();
        std::string generateStatusReport();
        void handleMessage(const std::string &message);
        void sendStatus();
        bool tryConsumeIngredients(const std::vector<std::string>& ingredients);

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

        std::atomic<bool> _running;

        std::size_t _activePizzas; 
        std::chrono::steady_clock::time_point _lastActiveTime;
    
        std::vector<CookState> _cookStates;
        Mutex _cookStatesMutex;
    };
} // namespace Plazza
