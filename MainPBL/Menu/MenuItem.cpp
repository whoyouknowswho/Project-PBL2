#include "MenuItem.h"

#include <bits/stdc++.h>

using namespace std;

MenuItem::MenuItem() : id(""), name(""), basePrice(0.0), isAvailable(true), description("") {
}

MenuItem::MenuItem(const string& name, double price)
    : id(""), name(name), basePrice(price), isAvailable(true), description("") {
}

double MenuItem::getPrice() const {
    return basePrice;
}

double MenuItem::updatePrice(double newPrice) {
    if (newPrice >= 0) {
        basePrice = newPrice;
    }
    return basePrice;
}

void MenuItem::setAvailable(bool status) {
    isAvailable = status;
}

bool MenuItem::getAvailable() const {
    return isAvailable;
}

void MenuItem::displayInfo() const {
    cout << "Name: " << name << endl;
    cout << "Price: $" << basePrice << endl;
    cout << "Available: " << (isAvailable ? "Yes" : "No") << endl;
    cout << "Description: " << description << endl;
}