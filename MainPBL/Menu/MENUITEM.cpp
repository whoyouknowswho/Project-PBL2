#include "MenuItem.h"

#include <bits/stdc++.h>

using namespace std;

MenuItem::MenuItem() : id(""), name(""), basePrice(0), stock(0), isAvailable(false) {
}

MenuItem::MenuItem(const string& id, const string& name, double price, int stock)
    : id(id), name(name), basePrice(price), stock(stock), isAvailable(false) {
}

MenuItem::~MenuItem() {
    delete this;
}

const string& MenuItem::getId() const {
    return id;
}

string MenuItem::getName() const {
    return name;
}

double MenuItem::getPrice() const {
    return basePrice;
}

int MenuItem::getStock() const {
    return stock;
}

void MenuItem::updateBasicInfo(const string& newName, double newPrice, int newStock) {
    name = newName;
    basePrice = newPrice;
    stock = newStock;
}

bool MenuItem::decreaseStock(int quantity) {
    if (quantity <= 0 || quantity > stock)
        return false;
    stock -= quantity;
    return true;
}

void MenuItem::increaseStock(int quantity) {
    if (quantity > 0) {
        stock += quantity;
    }
}
void MenuItem::updatePrice(double newPrice) {
    basePrice = newPrice;
}

void MenuItem::setAvailable(bool status) {
    isAvailable = status;
}

bool MenuItem::getAvailable() const {
    return isAvailable;
}

void MenuItem::displayInfo() const {
    cout << "ID: " << id << ", Name: " << name << ", Price: " << basePrice << ", Stock: " << stock
         << ", Available: " << (isAvailable ? "Yes" : "No") << endl;
}
