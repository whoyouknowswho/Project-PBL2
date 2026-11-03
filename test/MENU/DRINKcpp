#include "DRINK.h"

#include <iostream>

using namespace std;

Drink::Drink() : MenuItem(), cupSize(""), hasIceValue(false) {
}

Drink::Drink(const string& id, const string& name, double price, int stock, const string& cupSize,
             bool hasIce)
    : MenuItem(id, name, price, stock), cupSize(cupSize), hasIceValue(hasIce) {
}

string Drink::getCupSize() const {
    return cupSize;
}

bool Drink::hasIce() const {
    return hasIceValue;
}

string Drink::getType() const {
    return "Drink";
}

string Drink::displayDetails() const {
    return "Cup size: " + cupSize + ", Has ice: " + (hasIceValue ? "Yes" : "No");
}

void Drink::printSpecificDetails() const {
    cout << "Cup size: " << cupSize << endl;
    cout << "Has ice: " << (hasIceValue ? "Yes" : "No") << endl;
}

string Drink::createSaleName() const {
    string sizeChoice;
    while (true) {
        cout << "Choose cup size (S/M/L): ";
        cin >> sizeChoice;
        if (sizeChoice == "S" || sizeChoice == "M" || sizeChoice == "L") {
            break;
        }
        cout << "Invalid size. Please choose S, M, or L!" << endl;
    }

    string iceChoice;
    while (true) {
        cout << "Choose ice option (1 for Yes, 0 for No): ";
        cin >> iceChoice;
        if (iceChoice == "1" || iceChoice == "0") {
            break;
        }
        cout << "Invalid choice. Please enter 1 or 0!" << endl;
    }

    return getName() + " (Cup size: " + sizeChoice + ", " +
           (iceChoice == "1" ? "Has ice" : "No ice") + ")";
}

void Drink::updateDetails() {
    cout << "Enter new cup size: ";
    cin >> cupSize;
    cout << "Does it have ice? (1 for Yes, 0 for No): ";
    int iceInput;
    cin >> iceInput;
    hasIceValue = (iceInput == 1);
}

string Drink::toFile() const {
    return "DRINK|" + getId() + "|" + getName() + "|" + to_string(getPrice()) + "|" +
           to_string(getStock()) + "|" + cupSize + "|" + (hasIceValue ? "1" : "0");
}
