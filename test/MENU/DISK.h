#ifndef DISK_H
#define DISK_H

#include "MenuItem.h"

using namespace std;

class Dish : public MenuItem {
private:
    int spicyLevel = 0;
    bool vegetarian = false;

public:
    Dish();
    Dish(const string& id, const string& name, double price, int stock, int spicyLevel,
         bool vegetarian);

    int getSpicyLevel() const;
    bool isVegetarian() const;

    string getType() const override;
    string displayDetails() const override;
    void printSpecificDetails() const override;
    string createSaleName() const override;
    void updateDetails() override;
    string toFile() const override;
};

#endif