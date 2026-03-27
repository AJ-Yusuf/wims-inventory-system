#include "Product.h"
#include <stdexcept>
using namespace std;

// Constructor
Product::Product(string productId, string name, int quantity,
                 string location, double price) {
    if (quantity < 0)
        throw invalid_argument("Quantity cannot be negative");
    if (price < 0)
        throw invalid_argument("Price cannot be negative");

    this->productId = productId;
    this->name      = name;
    this->quantity  = quantity;
    this->location  = location;
    this->price     = price;
}

// Getters
string Product::getProductId() { return productId; }
string Product::getName()      { return name; }
int    Product::getQuantity()  { return quantity; }
string Product::getLocation()  { return location; }
double Product::getPrice()     { return price; }

// Setters
void Product::setName(string n) { name = n; }
void Product::setLocation(string l) { location = l; }

void Product::setQuantity(int q) {
    if (q < 0)
        throw invalid_argument("Quantity cannot be negative");
    quantity = q;
}

void Product::setPrice(double p) {
    if (p < 0)
        throw invalid_argument("Price cannot be negative");
    price = p;
}

// Returns false if not enough stock
bool Product::reduceQuantity(int amount) {
    if (amount > quantity) return false;
    quantity -= amount;
    return true;
}

// Print to screen
void Product::display() {
    cout << "  ID       : " << productId << endl;
    cout << "  Name     : " << name      << endl;
    cout << "  Quantity : " << quantity  << endl;
    cout << "  Location : " << location  << endl;
    cout << "  Price    : £" << price    << endl;
}

// For saving to CSV file
string Product::toCSV() {
    return productId + "," + name + "," +
           to_string(quantity) + "," +
           location + "," +
           to_string(price);
}