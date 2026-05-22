/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** PizzaSerializer.cpp
*/

/// @file PizzaSerializer.cpp
/// @brief Implementation of the PizzaSerializer class.

#include "PizzaSerializer.hpp"
#include "PlazzaException.hpp"
#include "Tools.hpp"

namespace Plazza {
    std::string PizzaSerializer::pack(const IPizza& pizza)
    {
        return pizza.getType() + ";" + std::to_string(static_cast<int>(pizza.getSize()));
    }

    PizzaOrder PizzaSerializer::unpack(const std::string& data)
    {
        std::vector<std::string> parts = Tools::split(data, ";");

        if (parts.size() != 2)
            throw PlazzaException("Malformed serialized pizza order: \"" + data + "\"");

        std::string type = Tools::trim(parts[0]);
        int sizeVal = 0;
        try {
            sizeVal = std::stoi(Tools::trim(parts[1]));
        } catch (const std::exception&) {
            throw PlazzaException("Invalid pizza size value in: \"" + data + "\"");
        }

        return {type, static_cast<PizzaSize>(sizeVal)};
    }
} // namespace Plazza
