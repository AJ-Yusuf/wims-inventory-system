#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
using namespace std;

class Product {
private:
    string productId;
    string name;
    int    quantity;
    string location;
    double price;

public:
    // Constructor
    Product(string productId, string name, int quantity, 
            string location, double price);

    // Getters
    string getProductId();
    string getName();
    int    getQuantity();
    string getLocation();
    double getPrice();

    // Setters
    void setName(string n);
    void setQuantity(int q);
    void setLocation(string l);
    void setPrice(double p);

    // Other methods
    bool   reduceQuantity(int amount);
    void   display();
    string toCSV();
};

#endif