/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Pizza.hpp
*/

/// @file Pizza.hpp
/// @brief Pizza class definition

#ifndef PIZZA_HPP
    #define PIZZA_HPP
    #include "APizza.hpp"

namespace Plazza {
    ///
    /// @class Pizza
    /// @brief Concrete class representing a specific pizza instance ordered in the Plazza.
    ///
    /// This class inherits from the abstract APizza class. Upon construction, it queries 
    /// the global or static recipe map (`pizzaRecipes`) using the pizza's name to fetch 
    /// its required ingredients and dynamically calculate its localized baking duration.
    ///
    class Pizza : public APizza {
        public:
            /// @brief Constructor to initialize any type of pizza.
            /// @param name Type of the pizza (e.g regina, margarita)
            /// @param size Size of the pizza (S, M, L, XL, XXL)
            Pizza(const std::string& name, PizzaSize size)
                : APizza(name, size) 
            {
                const Recipe& recipe = pizzaRecipes.at(name);
                
                _cookTime = static_cast<std::size_t>(recipe.baseCookTime * 1000);
                _ingredients = recipe.ingredients;
            }

            /// @brief Get the total cooking time for this specific pizza.
            /// @return std::size_t The baking time in milliseconds.
            inline std::size_t getCookTime() const override { return _cookTime; }

            /// @brief Get the list of ingredients required for this pizza.
            /// @return const std::vector<std::string>& A reference to the list of ingredients.
            inline const std::vector<std::string>& getIngredients() const override { return _ingredients; }

        private:
            std::size_t _cookTime;
            std::vector<std::string> _ingredients;
    };
} // namespace Plazza

#endif
