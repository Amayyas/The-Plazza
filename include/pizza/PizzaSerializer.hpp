/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** PizzaSerializer.hpp
*/

/// @file PizzaSerializer.hpp
/// @brief PizzaSerializer class definition

#pragma once

#include "IPizza.hpp"
#include <string>

namespace Plazza {
    /// @brief Lightweight struct holding the data needed to reconstruct a pizza order.
    struct PizzaOrder {
        std::string type; ///< Pizza type name (e.g. "margarita").
        PizzaSize   size; ///< Requested pizza size.
    };

    ///
    /// @class PizzaSerializer
    /// @brief Serializes and deserializes pizza orders for IPC transmission.
    ///
    /// pack() converts an IPizza into a compact string suitable for sending
    /// through the IPC channel.  unpack() reconstructs a PizzaOrder from that
    /// string so the Kitchen side can call PizzaFactory::createPizza().
    ///
    class PizzaSerializer {
    public:
        /// @brief Serialize a pizza into a transmittable string.
        /// @param pizza The pizza instance to serialize.
        /// @return A string of the form "type;sizeValue".
        static std::string pack(const IPizza& pizza);

        /// @brief Deserialize a string back into a PizzaOrder.
        /// @param data A string previously produced by pack().
        /// @return PizzaOrder containing the type name and size.
        /// @throw Plazza::PlazzaException If the data is malformed.
        static PizzaOrder unpack(const std::string& data);
    };
} // namespace Plazza
