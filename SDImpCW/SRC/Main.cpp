#include "Product.h"
#include "Order.h"
#include <iostream>
using namespace std;

int main() {
    Product p("W_MOUSE_101", "Wireless Mouse", 500, "Aisle C", 29.99);
    p.display();

    cout << "---" << endl;

    Order o("ORD001");
    o.addItem("W_MOUSE_101", 50);
    o.addItem("KEYBOARD_102", 30);
    o.display();

    return 0;
}