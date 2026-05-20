/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Exception.hpp
*/

/// @file PlazzaException.hpp
/// @brief PlazzaException class definition

#pragma once

#include <exception>
#include <string>

/// @brief Namespace for the Plazza project
namespace Plazza {
    /// @brief Custom exception class for Plazza-related errors
    class PlazzaException : public std::exception {
        public:
            /// @brief Constructs a PlazzaException object
            /// @param message The error message describing the exception
            PlazzaException(const std::string& message);

            /// @brief Retrieves the error message associated with the exception
            /// @return A C-style string representing the error message
            [[nodiscard]] const char* what() const noexcept override;

        private:
            /// @brief The error message describing the exception
            std::string _message;
    };
} // namespace Plazza
