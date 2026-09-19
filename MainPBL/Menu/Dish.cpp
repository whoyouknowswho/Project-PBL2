#include "Dish.h"

#include <bits/stdc++.h>

using namespace std;

Dish::Dish() : spicyLevel(0), method(""), ingredients(""), vegetarian(false) {
}

Dish::Dish(const string& name, double price, int spicyLevel, const string& method,
           const string& ingredients, bool vegetarian)
    : spicyLevel(spicyLevel), method(method), ingredients(ingredients), vegetarian(vegetarian) {
}

int Dish::getSpicyLevel() const {
    return spicyLevel;
}

string Dish::getMethod() const {
    return method;
}

string Dish::getIngredients() const {
    return ingredients;
}

bool Dish::isVegetarian() const {
    return vegetarian;
}
