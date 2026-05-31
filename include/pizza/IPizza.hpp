/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** IPizza.hpp
*/

/// @file IPizza.hpp
/// @brief IPizza class definition

#pragma once

#include <map>
#include <vector>
#include <string>

namespace Plazza {
    /// @brief Available sizes for a pizza order.
    enum PizzaSize
    {
        S   = 1,  ///< Small
        M   = 2,  ///< Medium
        L   = 4,  ///< Large
        XL  = 8,  ///< Extra-Large
        XXL = 16  ///< Extra-Extra-Large
    };

    /// @brief Bidirectional lookup table from size string to PizzaSize enum.
    inline std::map<std::string, PizzaSize> pizzaSizes = {
        {"S", PizzaSize::S},
        {"M", PizzaSize::M},
        {"L", PizzaSize::L},
        {"XL", PizzaSize::XL},
        {"XXL", PizzaSize::XXL}
    };

    ///
    /// @struct Recipe
    /// @brief Holds the cooking data for a single pizza type loaded from the menu.
    ///
    struct Recipe {
        std::size_t baseCookTime;              ///< Base cooking time in seconds (before multiplier).
        std::vector<std::string> ingredients;  ///< List of required ingredient names.
        double price;                          ///< Price of the pizza in euros.
    };

    /// @brief Global registry mapping pizza type names to their recipes (populated by MenuLoader).
    inline std::map<std::string, Recipe> pizzaRecipes;

    /// @brief Flat list of every ingredient name known across all loaded recipes.
    inline std::vector<std::string> allKnownIngredients;

    ///
    /// @class IPizza
    /// @brief Interface class representing the mandatory methods for a Pizza.
    ///
    class IPizza {
        public:
            /// @brief Destructor for IPizza.
            /// Marked as virtual override to ensure proper cleanup of derived concrete classes.
            virtual ~IPizza() = default;

            /// @brief Get the type/name of the pizza.
            /// @return std::string The name of the pizza.
            virtual std::string getType() const = 0;

            /// @brief Get the size of the pizza.
            /// @return PizzaSize The size enum (S, M, L, XL, XXL).
            virtual PizzaSize getSize() const = 0;

            /// @brief Get the total cooking time for this specific pizza.
            /// @return std::size_t The baking time in milliseconds.
            virtual std::size_t getCookTime() const = 0;

            /// @brief Get the list of ingredients required for this pizza.
            /// @return const std::vector<std::string>& A reference to the list of ingredients.
            virtual const std::vector<std::string>& getIngredients() const = 0;
    };
} // namespace Plazza
