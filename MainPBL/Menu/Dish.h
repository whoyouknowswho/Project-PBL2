#ifndef Dish_H
#define Dish_H
#include <bits/stdc++.h>
using namespace std;

class Dish {
public:
    Dish();
    Dish(const string& name, double price, int spicyLevel, const string& method,
         const string& ingredients, bool vegetarian);
    int getSpicyLevel() const;
    string getMethod() const;
    string getIngredients() const;
    bool isVegetarian() const;
    void displayInfo() const;

private:
    int spicyLevel;
    string method;
    string ingredients;
    bool vegetarian;
};

#endif