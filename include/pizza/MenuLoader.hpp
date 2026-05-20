/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** MenuLoader.hpp
*/

/// @file MenuLoader.hpp
/// @brief MenuLoader class definition.

#pragma once

#include "IPizza.hpp"
#include "PlazzaException.hpp"
#include <string>

namespace Plazza {
    ///
    /// @class MenuLoader
    /// @brief Static utility class responsible for loading pizza recipes and configuration.
    ///
    /// This class handles retrieving the available pizza types, their ingredients, and cooking times.
    /// It populates the recipe registry either from an external CSV configuration file or by
    /// falling back to hardcoded default recipes if no file is provided.
    ///
    class MenuLoader {
        public:
            /// @brief Load the CSV file and fill the map pizzaRecipes.
            /// @param filepath Path to the file menu.csv
            static void loadMenu(const std::string& filepath = "");

        private:
            /// @brief Load the default recipes if there is no CSV
            static void loadDefaultMenu();
    };
} // namespace Plazza
