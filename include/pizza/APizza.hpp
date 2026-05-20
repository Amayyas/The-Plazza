/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** APizza.hpp
*/

/// @file APizza.hpp
/// @brief APizza class definition

#pragma once

#include "IPizza.hpp"

namespace Plazza {
    ///
    /// @class APizza
    /// @brief Abstract base class representing a generic Pizza.
    /// 
    /// This class implements the common interface defined by IPizza. It acts as
    /// a bridge between the pure interface and concrete pizza implementations,
    /// storing shared attributes like the pizza's name and size.
    ///
    class APizza : public IPizza {
        public:
            /// @brief Destructor for APizza.
            /// Marked as virtual override to ensure proper cleanup of derived concrete classes.
            ~APizza() override = default;

            /// @brief Get the type/name of the pizza.
            /// @return std::string The name of the pizza.
            std::string getType() const override;

            /// @brief Get the size of the pizza.
            /// @return PizzaSize The size enum (S, M, L, XL, XXL).
            PizzaSize getSize() const override;

            /// @brief Get the total cooking time for this specific pizza.
            /// @return std::size_t The baking time in milliseconds.
            std::size_t getCookTime() const override = 0;

            /// @brief Get the list of ingredients required for this pizza.
            /// @return const std::vector<std::string>& A reference to the list of ingredients.
            const std::vector<std::string>& getIngredients() const override = 0;

        protected:
            /// @brief Protected constructor to initialize core pizza attributes.
            /// @param name The identifier/name string of the pizza.
            /// @param size The target size of the pizza.
            APizza(const std::string& name, PizzaSize size);

            std::string _name;
            PizzaSize _pizzaSize;
    };
} // namespace Plazza
