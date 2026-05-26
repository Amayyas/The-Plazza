/*
** EPITECH PROJECT, 2025
** The-Plazza
** File description:
** MenuLoader.cpp
*/

/// @file MenuLoader.cpp
/// @brief Implementation of the MenuLoader class.

#include "MenuLoader.hpp"
#include "Tools.hpp"
#include <fstream>
#include <algorithm>

namespace Plazza {
    void MenuLoader::loadMenu(const std::string& filepath)
    {
        std::ifstream file(filepath);

        if (!file.is_open()) {
            loadDefaultMenu();
            return;
        }

        std::string line;

        while (std::getline(file, line)) {
            line = Tools::trim(line);
            if (line.empty() || line[0] == '#' || line.rfind("PizzaType", 0) == 0)
                continue;

            std::vector<std::string> cols = Tools::split(line, ";");
            if (cols.size() < 4)
                continue;

            std::string name = Tools::trim(cols[0]); 
            std::string cookTimeStr = cols[1];
            std::string ingredientsStr = cols[2];
            std::string priceStr = cols[3];
            
            Recipe recipe;
            recipe.baseCookTime = std::stoul(cookTimeStr);
            
            std::vector<std::string> ingList = Tools::split(ingredientsStr, ",|");
            for (const auto& ing : ingList) {
                std::string cleanedIng = Tools::trim(ing);
                recipe.ingredients.push_back(cleanedIng);

                if (std::find(allKnownIngredients.begin(), allKnownIngredients.end(),
                    cleanedIng) == allKnownIngredients.end())
                    allKnownIngredients.push_back(cleanedIng);
            }

            try {
                recipe.price = std::stod(priceStr);
            } catch (const std::exception &e) {
                recipe.price = 0.0f;
            }

            pizzaRecipes[name] = recipe;
        }
    }

    void MenuLoader::loadDefaultMenu()
    {
        pizzaRecipes["margarita"] = {1, {"dough", "tomato", "gruyere"}, 6.00};
        pizzaRecipes["regina"]    = {2, {"dough", "tomato", "gruyere", "ham", "mushrooms"}, 8.00};
        pizzaRecipes["americana"] = {2, {"dough", "tomato", "gruyere", "steak"}, 16.00};
        pizzaRecipes["fantasia"]  = {4, {"dough", "tomato", "eggplant", "goat cheese", "chief love"}, 7.00};
    
        for (const auto& [name, recipe] : pizzaRecipes) {
            for (const auto& ingredient : recipe.ingredients) {
                if (std::find(allKnownIngredients.begin(), allKnownIngredients.end(),
                    ingredient) == allKnownIngredients.end()) {
                    allKnownIngredients.push_back(ingredient);
                }
            }
        }
    }
}
