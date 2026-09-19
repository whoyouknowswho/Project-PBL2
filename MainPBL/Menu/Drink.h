#ifndef Drink_H
#define Drink_H
#include <bits/stdc++.h>
using namespace std;

class Drink {
public:
    Drink();
    Drink(const string& name, const string& cupSize, double price, bool hasIce, bool isCarbonated);
    string getName() const;
    string getCupSize() const;
    double getPrice() const;
    bool hasIce() const;
    bool isCarbonated() const;

private:
    string cupSize;
    double price;
    bool hasice;
    bool iscarbonated;
};

#endif