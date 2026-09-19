#include "Drink.h"

#include <bits/stdc++.h>

using namespace std;

Drink::Drink() : cupSize(""), price(0.0), hasice(false), iscarbonated(false) {
}

Drink::Drink(const string& name, const string& cupSize, double price, bool hasIce,
             bool isCarbonated)
    : cupSize(cupSize), price(price), hasice(hasIce), iscarbonated(isCarbonated) {
}

string Drink::getName() const {
    return cupSize;
}

double Drink::getPrice() const {
    return price;
}

string Drink::getCupSize() const {
    return cupSize;
}

bool Drink::hasIce() const {
    return hasice;
}

bool Drink::isCarbonated() const {
    return iscarbonated;
}
