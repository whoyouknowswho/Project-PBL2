#ifndef BILL_H
#define BILL_H

#include <bits/stdc++.h>

using namespace std;

struct BillItem {
    string itemId, itemName;
    int quantity = 0;
    double unitPrice = 0;

    double totalPrice() const {
        return quantity * unitPrice;
    }
};

class Bill {
    string billId, issueDate, customerId, employeeId;
    vector<BillItem> items;
    double discountPercentage = 0;

public:
    Bill() = default;
    Bill(string billId, string issueDate, string customerId, string employeeId);

    const string& getId() const;
    const string& getIssueDate() const;
    const vector<BillItem>& getItems() const;
    void addItem(const BillItem& item);
    void setDiscount(double percentage);
    double subTotal() const;
    double discountAmount() const;
    double grandTotal() const;
    void printBill() const;
    string toFile() const;
    static Bill fromFile(const string& data);
};

#endif