/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** PizzaFactory.cpp
*/

/// @file PizzaFactory.cpp
/// @brief Implementation of the PizzaFactory class.

#include "Pizza.hpp"
#include "PizzaFactory.hpp"

namespace Plazza {
    std::unique_ptr<IPizza> PizzaFactory::createPizza(const std::string& name, PizzaSize size)
    {
        if (pizzaRecipes.find(name) == pizzaRecipes.end()) {
            throw PlazzaException("Pizza type not loaded in menu.");
        }

        return std::make_unique<Pizza>(name, size);
    }
}
