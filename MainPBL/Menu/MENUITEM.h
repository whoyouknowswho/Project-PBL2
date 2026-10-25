#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <string>

using namespace std;

class MenuItem {
protected:
    string id, name;
    double basePrice = 0;
    int stock = 0;
    bool isAvailable = false;

public:
    MenuItem();
    MenuItem(const string& id, const string& name, double price, int stock);
    virtual ~MenuItem();

    const string& getId() const;
    string getName() const;
    double getPrice() const;
    int getStock() const;

    void updateBasicInfo(const string& name, double price, int stock);
    bool decreaseStock(int quantity);
    void increaseStock(int quantity);

    virtual string getType() const = 0;
    virtual string displayDetails() const = 0;
    virtual void printSpecificDetails() const = 0;
    virtual string createSaleName() const = 0;
    virtual void updateDetails() = 0;
    virtual string toFile() const = 0;

    void displayInfo() const;
    void updatePrice(double newPrice);
    void setAvailable(bool status);
    bool getAvailable() const;
};

#endif
