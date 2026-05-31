/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** OrderParser.hpp
*/

/// @file OrderParser.hpp
/// @brief OrderParser class definition

#pragma once

/// @brief Human-readable syntax reminder shown alongside parse error messages.
#define SYNTHAX_USAGE "Correct order synthax: [PizzaType] [PizzaSize] x[Quantity]"

#include "Tools.hpp"
#include "IPizza.hpp"
#include "PizzaFactory.hpp"
#include "PlazzaException.hpp"

#include <vector>
#include <memory>

namespace Plazza {
    ///
    /// @class OrderParser
    /// @brief Static parsing class responsible for validating and tokenizing input order strings.
    ///
    /// This class functions as the primary shell input gatekeeper.
    /// It reads raw multi-order commands from the standard input (separated by semicolons),
    /// sanitizes the string segments, extracts the fields (Type, Size, Quantity),
    /// and interacts with the PizzaFactory to construct executable orders.
    ///
    class OrderParser {
        public:
            /// @brief Parses a full raw string line containing one or more pizza orders.
            /// 
            /// Splices the input by a semicolon context divider (`;`) to extract standalone order blocks, 
            /// trims extraneous whitespaces, and aggregates the resulting smart pointers into a flat array vector.
            ///
            /// @param input The raw line of text straight from the CLI prompt.
            /// @return std::vector<std::unique_ptr<IPizza>> A collection of instantiated pizza objects ready for scheduling.
            /// @throw Plazza::PlazzaException If any order block fails regex validation or basic formatting checks.
            static std::vector<std::unique_ptr<IPizza>> parse(const std::string &input);

        private:
            /// @brief Parses a single isolated order match block.
            /// 
            /// Analyzes a unique substring segment formatted strictly as `[PizzaType] [PizzaSize] x[Quantity]`. 
            /// It reads the numeric quantity parameter and calls the PizzaFactory loop-wise to spawn that 
            /// explicit amount of objects.
            ///
            /// @param item A trimmed single order segment substring (e.g., "regina XL x2").
            /// @return std::vector<std::unique_ptr<IPizza>> The specific sub-vector of cloned pizza instances.
            /// @throw Plazza::PlazzaException If structural elements, type strings, or quantities are malformed.
            static std::vector<std::unique_ptr<IPizza>> parseItem(const std::string &item);

            /// @brief Converts a raw string token into its matching PizzaSize strongly-typed enum.
            ///
            /// @param str The sub-token string matching the size indicator (e.g., "S", "M", "L", "XL", "XXL").
            /// @return PizzaSize The matching enum state definition mapped from the string.
            /// @throw Plazza::PlazzaException If the string token does not map to any valid PizzaSize option.
            static PizzaSize parseSize(const std::string &str);
    };
} // namespace Plazza
