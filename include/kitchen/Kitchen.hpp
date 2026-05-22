/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Kitchen.hpp
*/

/// @file Kitchen.hpp
/// @brief Kitchen class definition

#pragma once

#include "IPC.hpp"

namespace Plazza {
    typedef struct {
        pid_t pid;
        IPC ipc;
        std::size_t currentLoad = 0;
    } KitchenProxy_t;

    class Kitchen {
        public:
            Kitchen();
            ~Kitchen() = default;
    };
} // namespace Plazza
