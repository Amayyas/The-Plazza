/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Reception.hpp
*/

/// @file Reception.hpp
/// @brief Reception class definition

#pragma once

#define PROMPT "$> "
#define EXIT "exit"
#define EXIT_MESSAGE "End of service, see you tomorrow !"

#include <vector>
#include <string>
#include <memory>

#include "IPC.hpp"
#include "IPizza.hpp"
#include "Kitchen.hpp"
#include "PizzaSerializer.hpp"
#include "PlazzaException.hpp"

namespace Plazza {
    ///
    /// @class Reception
    /// @brief The main orchestrator and user interface manager of the Plazza application.
    ///
    /// Acting as the Master Process (or Core Router), the Reception handles user interactions, 
    /// inputs shell loops, updates kitchen statuses, and dynamically allocates or schedules orders 
    /// across active child Kitchen instances based on optimized workloads.
    ///
    class Reception {
        public:
            /// @brief Constructor initializing the master system core parameters.
            /// @param cookingMultiplier Time multiplier setting impacting overall baking speeds.
            /// @param cooksPerKitchen Maximum number of worker threads allowed per kitchen capsule.
            /// @param restockDelay Temporal gap (in milliseconds) between ingredient replenishment cycles.
            Reception(float cookingMultiplier, int cooksPerKitchen, int restockDelay, bool ticket=false);

            /// @brief Destructor handling safe termination and cleanup of sub-processes and resources.
            ~Reception();

            /// @brief Boots up the master loop execution environment.
            void run();
        
        private:
            float _multiplier;
            int _cooksPerKitchen;
            int _restockDelay;

            std::vector<KitchenProxy_t> _kitchens;

            bool _running;
            bool _ticket;

            /// @brief Infinite interface reading loop that monitors standard input stream states.
            void shellLoop();

            /// @brief High-level multiplexer identifying whether a line is a macro instruction
            /// (exit, status) or a new order.
            /// @param line Raw input string retrieved from standard input.
            void handleCommand(const std::string &line);

            /// @brief Processes a verified raw pizza line through the OrderParser and initiates
            /// factory generation.
            /// @param line Specific string matching sequence items (e.g. "regina M x2").
            void handleOrder(const std::string &line);

            /// @brief Gathers, formats, and displays real-time execution statistics
            /// for all active child Kitchen structures.
            void handleStatus();

            /// @brief Prints a clean terminal summary voucher outlining all successfully processed item segments.
            /// @param pizzas Collection of successfully verified abstract unique pizza pointer objects.
            void displayReceipt(const std::vector<std::unique_ptr<IPizza>> &pizzas);

            /// @brief Utility view rendering a listing summary of existing operational configurations.
            void displayMenu();

            /// @brief Algorithmic selector checking load indexes across alive kitchens
            /// to locate the most optimal queue spot.
            /// @return int Index corresponding to the target entry inside the `_kitchens` vector array.
            /// Returns -1 if none fit.
            int getBestKitchen();

            /// @brief Dynamic allocator handling fork overheads or interface initialization hooks
            /// for a brand new Kitchen node.
            void spawnKitchen();

            /// @brief Delivers a polymorphic unique instance of a pizza block down onto an assigned
            /// kitchen node's channel.
            /// @param pizza Rvalue context reference transporting tracking ownership rights over the allocated asset.
            void dispatchPizza(std::unique_ptr<IPizza> pizza);
    };
} // namespace Plazza
