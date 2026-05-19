/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** APizza.hpp
*/

#ifndef APIZZA_HPP
    #define APIZZA_HPP
    #include "IPizza.hpp"

namespace Plazza {
    class APizza : public IPizza {
        public:
            ~APizza() override = default;
    };
}

#endif
