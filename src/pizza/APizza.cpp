/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** APizza.cpp
*/

/// @file APizza.cpp
/// @brief Implementation of the APizza class.

#include "APizza.hpp"

namespace Plazza {
    APizza::APizza(const std::string& name, PizzaSize size) :
    _name(name), _pizzaSize(size) {}

    std::string APizza::getType() const
    {
        return _name;
    }

    PizzaSize APizza::getSize() const
    {
        return _pizzaSize;
    }
}
