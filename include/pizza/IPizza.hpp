/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** IPizza.hpp
*/

/// @file IPizza.hpp
/// @brief IPizza class definition

#ifndef IPIZZA_HPP
    #define IPIZZA_HPP
    #include <map>
    #include <vector>
    #include <string>

namespace Plazza {
    enum PizzaSize
    {
        S = 1,
        M = 2,
        L = 4,
        XL = 8,
        XXL = 16
    };

    inline std::map<std::string, PizzaSize> pizzaSizes = {
        {"S", PizzaSize::S},
        {"M", PizzaSize::M},
        {"L", PizzaSize::L},
        {"XL", PizzaSize::XL},
        {"XXL", PizzaSize::XXL}
    };

    struct Recipe {
        std::size_t baseCookTime;
        std::vector<std::string> ingredients;
        double price;
    };

    inline std::map<std::string, Recipe> pizzaRecipes;

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

#endif
