/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** PlazzaException.cpp
*/

/// @file PlazzaException.cpp
/// @brief Implementation of the PlazzaException class.

#include "PlazzaException.hpp"

namespace Plazza {

    PlazzaException::PlazzaException(const std::string& message) : _message(message) {}

    const char* PlazzaException::what() const noexcept {
        return _message.c_str();
    }
}
