#include "BILL.h"

#include <bits/stdc++.h>

using namespace std;

namespace {
vector<string> split(const string& text, char delimiter) {
    vector<string> fields;
    stringstream stream(text);
    string field;
    while (getline(stream, field, delimiter))
        fields.push_back(field);
    return fields;
}
}  // namespace

Bill::Bill(string id, string date, string customer, string employee)
    : billId(id), issueDate(date), customerId(customer), employeeId(employee) {
}

const string& Bill::getId() const {
    return billId;
}

const string& Bill::getIssueDate() const {
    return issueDate;
}

const vector<BillItem>& Bill::getItems() const {
    return items;
}

void Bill::addItem(const BillItem& itemToAdd) {
    for (BillItem& item : items) {
        if (item.itemId == itemToAdd.itemId && item.itemName == itemToAdd.itemName) {
            item.quantity += itemToAdd.quantity;
            return;
        }
    }
    items.push_back(itemToAdd);
}

void Bill::setDiscount(double percentage) {
    if (percentage >= 0 && percentage <= 100)
        discountPercentage = percentage;
    else
        discountPercentage = 0;
}

double Bill::subTotal() const {
    double total = 0;
    for (const BillItem& item : items)
        total += item.totalPrice();
    return total;
}

double Bill::discountAmount() const {
    return subTotal() * discountPercentage / 100;
}

double Bill::grandTotal() const {
    return subTotal() - discountAmount();
}

void Bill::printBill() const {
    cout << "BILL " << billId << '\n'
         << "Date: " << issueDate << '\n'
         << "Customer: " << customerId << '\n'
         << "Employee: " << employeeId << '\n';
    for (const BillItem& item : items) {
        cout << item.itemId << " - " << item.itemName << " x " << item.quantity << " @ "
             << item.unitPrice << " = " << item.totalPrice() << '\n';
    }
    cout << "Subtotal: " << subTotal() << '\n'
         << "Discount: " << discountAmount() << '\n'
         << "Total: " << grandTotal() << '\n';
}

string Bill::toFile() const {
    ostringstream data;
    data << billId << '|' << issueDate << '|' << customerId << '|' << employeeId << '|'
         << discountPercentage << '|';
    for (size_t i = 0; i < items.size(); ++i) {
        if (i)
            data << ';';
        data << items[i].itemId << ',' << items[i].itemName << ',' << items[i].quantity << ','
             << items[i].unitPrice;
    }
    return data.str();
}

Bill Bill::fromFile(const string& data) {
    vector<string> fields = split(data, '|');
    if (fields.size() < 6)
        return Bill();

    Bill bill(fields[0], fields[1], fields[2], fields[3]);
    bill.setDiscount(stod(fields[4]));
    for (const string& itemData : split(fields[5], ';')) {
        vector<string> itemFields = split(itemData, ',');
        if (itemFields.size() == 4) {
            bill.addItem({itemFields[0], itemFields[1], stoi(itemFields[2]), stod(itemFields[3])});
        }
    }
    return bill;
}