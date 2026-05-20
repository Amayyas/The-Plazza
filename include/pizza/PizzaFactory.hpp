/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** PizzaFactory.hpp
*/

/// @file PizzaFactory.hpp
/// @brief PizzaFactory class definition

#ifndef PIZZAFACTORY_HPP
    #define PIZZAFACTORY_HPP
    #include "IPizza.hpp"
    #include "PlazzaException.hpp"
    #include <memory>

namespace Plazza {
    ///
    /// @class PizzaFactory
    /// @brief Factory class responsible for the creation of concrete IPizza objects.
    ///
    /// This design pattern decouples the raw command parsing interface from the explicit 
    /// instantiation logic of individual pizza instances. It acts as a secure, centralized 
    /// creation hub that guarantees only valid, known pizza variants enter the simulation pipeline.
    ///
    class PizzaFactory {
        public:
            /// @brief Instantiates a concrete pizza object based on its name and scale configuration.
            ///
            /// @param name The type of the pizza (e.g., "regina", "margarita").
            /// @param size The structural sizing of the target pizza (S, M, L, XL, XXL).
            /// @return std::unique_ptr<IPizza> A unique pointer managing the newly allocated concrete pizza object.
            /// @throw Plazza::PlazzaException If the requested pizza name is invalid or unrecognized by the system registry.
            static std::unique_ptr<IPizza> createPizza(const std::string& name, PizzaSize size);
    };
} // namespace Plazza

#endif
