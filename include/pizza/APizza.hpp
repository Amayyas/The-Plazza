/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** APizza.hpp
*/

/// @file APizza.hpp
/// @brief APizza class definition

#ifndef APIZZA_HPP
    #define APIZZA_HPP
    #include "IPizza.hpp"

namespace Plazza {
    class APizza : public IPizza {
        public:
            ~APizza() override = default;
    };
} // namespace Plazza

#endif
