#include "DISK.h"

#include <iostream>

using namespace std;

Dish::Dish() : MenuItem(), spicyLevel(0), vegetarian(false) {
}

Dish::Dish(const string& id, const string& name, double price, int stock, int spicyLevel,
           bool vegetarian)
    : MenuItem(id, name, price, stock), spicyLevel(spicyLevel), vegetarian(vegetarian) {
}

int Dish::getSpicyLevel() const {
    return spicyLevel;
}

bool Dish::isVegetarian() const {
    return vegetarian;
}

string Dish::getType() const {
    return "Dish";
}

string Dish::displayDetails() const {
    return getName();
}

void Dish::printSpecificDetails() const {
    cout << "Spicy level: " << spicyLevel << endl;
    cout << "Vegetarian: " << (vegetarian ? "Yes" : "No") << endl;
}

string Dish::createSaleName() const {
    return getName();
}

void Dish::updateDetails() {
    cout << "Enter new spicy level (0-5): ";
    cin >> spicyLevel;
    cout << "Is vegetarian (1 for yes, 0 for no): ";
    cin >> vegetarian;
}

string Dish::toFile() const {
    return "DISH|" + getId() + '|' + getName() + '|' + to_string(getPrice()) + '|' +
           to_string(getStock()) + '|' + to_string(spicyLevel) + '|' + (vegetarian ? "1" : "0");
}
