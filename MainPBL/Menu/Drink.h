#ifndef DRINK_H
#define DRINK_H

#include "MENUITEM.h"

using namespace std;

class Drink : public MenuItem {
private:
    string cupSize;
    bool hasIceValue = false;

public:
    Drink();
    Drink(const string& id, const string& name, double price, int stock,
          const string& cupSize, bool hasIce);

    string getCupSize() const;
    bool hasIce() const;

    string getType() const override;
    string displayDetails() const override;
    void printSpecificDetails() const override;
    string createSaleName() const override;
    void updateDetails() override;
    string toFile() const override;
};

#endif