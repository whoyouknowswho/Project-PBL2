#ifndef MenuItem_H
#define MenuItem_H
#include <bits/stdc++.h>
using namespace std;

class MenuItem {
public:
    MenuItem();
    MenuItem(const string& name, double price);
    string getName() const;
    double getPrice() const;
    void displayInfo() const;
    double updatePrice(double newPrice);
    void setAvailable(bool status);
    bool getAvailable() const;

private:
    string id;
    string name;
    double basePrice;
    double isAvailable;
    string description;
};

#endif