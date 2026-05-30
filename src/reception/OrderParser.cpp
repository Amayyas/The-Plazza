/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** OrderParser.cpp
*/

/// @file Reception.cpp
/// @brief Implementation of the Reception class.

#include "OrderParser.hpp"

namespace Plazza {
    std::vector<std::unique_ptr<IPizza>> OrderParser::parse(const std::string &input)
    {
        std::vector<std::unique_ptr<IPizza>> result;

        std::vector<std::string> segments = Tools::split(input, ";");

        for (std::string segment : segments) {
            std::vector<std::unique_ptr<IPizza>> pizzas = parseItem(Tools::trim(segment));

            for (std::unique_ptr<IPizza> &p : pizzas)
                if (p)
                    result.push_back(std::move(p));
        }

        return result;
    }

    std::vector<std::unique_ptr<IPizza>> OrderParser::parseItem(const std::string &item)
    {
        if (item.empty())
            return {};
    
        std::vector<std::string> segment = Tools::split(item);

        if (segment.empty())
            return {};

        if (segment.size() != 3)
            throw PlazzaException("Invalid syntax: \"" + item + "\"\n" + SYNTHAX_USAGE);

        std::string typeName = Tools::toLower(segment[0]);
        std::string sizeName = Tools::toUpper(segment[1]);
        int quantity = 0;

        if (!segment[2].empty() && segment[2][0] == 'x')
            segment[2] = segment[2].substr(1);
        try {
            quantity = std::stoi(segment[2]);

            if (quantity <= 0)
                return {};
        } catch (const std::exception &e) {
            throw PlazzaException("Invalid syntax: \"" + item + "\"\n" + SYNTHAX_USAGE);
        }

        PizzaSize size;
        try {
            size = parseSize(sizeName);
        } catch (const PlazzaException &) {
            throw PlazzaException("Invalid syntax: \"" + item + "\"\n" + SYNTHAX_USAGE);
        }

        std::vector<std::unique_ptr<IPizza>> pizzas;
        try {
            for (int i = 0; i < quantity; ++i)
                pizzas.push_back(PizzaFactory::createPizza(typeName, size));
        } catch (const PlazzaException &) {
            throw PlazzaException("Unknown pizza type: \"" + typeName + "\"\n" + SYNTHAX_USAGE);
        }

        return pizzas;
    }

    PizzaSize OrderParser::parseSize(const std::string &str)
    {
        if (pizzaSizes.find(str) == pizzaSizes.end())
            throw PlazzaException("Unknown pizza size: " + str);

        return pizzaSizes.at(str);
    }
}
