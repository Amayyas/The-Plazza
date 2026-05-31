/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** Tools.hpp
*/

/// @file Tools.hpp
/// @brief Tools class definition

#pragma once

#include <string>
#include <vector>

///
/// @namespace Tools
/// @brief Contains header-only inline string parsing and formatting helpers.
///
/// This namespace isolates common string sanitation routines, shielding core parsing 
/// modules like OrderParser from boilerplate algorithm logic.
///
namespace Tools {

    /// @brief Default whitespace delimiters (space, tab, newline).
    inline const std::string DEFAULT_CHARS = " \t\n";

    /// @brief Remove leading characters from the string.
    /// @param str String to modify
    /// @param del (optional) Delimiters to remove (default: space, tabs, newlines).
    /// @return std::string Formatted string
    inline std::string ltrim(std::string str, const std::string& del=DEFAULT_CHARS)
    {
        std::size_t start = str.find_first_not_of(del);
        return (start == std::string::npos) ? "" : str.substr(start);
    }

    /// @brief Remove trailing characters from the string.
    /// @param str String to modify
    /// @param del (optional) Delimiters to remove (default: space, tabs, newlines).
    /// @return std::string Formatted string
    inline std::string rtrim(std::string str, const std::string& del=DEFAULT_CHARS)
    {
        std::size_t end = str.find_last_not_of(del);
        return (end == std::string::npos) ? "" : str.substr(0, end + 1);
    }

    /// @brief Remove leading and trailing characters from the string.
    /// @param str String to modify
    /// @param del (optional) Delimiters to remove (default: space, tabs, newlines).
    /// @return std::string Formatted string
    inline std::string trim(std::string str, const std::string& del=DEFAULT_CHARS)
    {
        return ltrim(rtrim(str, del), del);
    }

    /// @brief Divide a string into a list of substrings based on separators.
    /// @param str String to split.
    /// @param del (optional) Delimiters/Separators used to split the string
    /// (default: spaces, tabs, newlines).
    /// @param maxSplit (optional) Maximum numlber of splits to perform
    /// (default: -1, which means there is no limit).
    /// @return std::vector<std::string> List of substrings.
    inline std::vector<std::string> split(
        const std::string& str,
        const std::string& del=DEFAULT_CHARS,
        const int maxSplit=-1
    ) {
        std::vector<std::string> substrings;
        size_t start = 0;
        size_t end = str.find_first_of(del);
        int splitCount = 0;

        while (end != std::string::npos && (maxSplit == -1 || splitCount < maxSplit)) {
            if (end > start) {
                substrings.push_back(str.substr(start, end - start));
                splitCount++;
            }
            start = end + 1;
            end = str.find_first_of(del, start);
        }

        if (start < str.length()) {
            substrings.push_back(str.substr(start));
        }
        return substrings;
    }

    /// @brief Convert a string into lowercase.
    /// @param str String to convert.
    /// @return std::string Converted string
    inline std::string toLower(std::string str)
    {
        for (char &c : str)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return str;
    }

    /// @brief Convert a string into uppercase.
    /// @param str String to convert.
    /// @return std::string Converted string
    inline std::string toUpper(std::string str)
    {
        for (char &c : str)
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return str;
    }
} // namespace Tools
