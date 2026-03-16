#include "Manager.h"
using namespace std;

Manager::Manager(string name, string id, string password)
    : Person(name, id, password) {}

string Manager::getRole() { return "MANAGER"; }

void Manager::displayMenu() {
    cout << "\n====================================" << endl;
    cout << "  MANAGER MENU - " << name << endl;
    cout << "====================================" << endl;
    cout << endl;
    cout << "  ----- PRODUCTS -----" << endl;
    cout << "  1.  Add Product" << endl;
    cout << "  2.  View All Products" << endl;
    cout << "  3.  Edit Product" << endl;
    cout << "  4.  Delete Product" << endl;
    cout << endl;
    cout << "  ----- EMPLOYEES -----" << endl;
    cout << "  5.  Add Employee" << endl;
    cout << "  6.  View All Employees" << endl;
    cout << "  7.  Edit Employee" << endl;
    cout << endl;
    cout << "  ----- ORDERS -----" << endl;
    cout << "  8.  Create Order" << endl;
    cout << "  9.  View All Orders" << endl;
    cout << "  10. Edit Order" << endl;
    cout << "  11. Assign Order" << endl;
    cout << endl; 
    cout << "  ----- REPORTS -----" << endl;
    cout << "  12. Generate Inventory Report" << endl;
    cout << "  13. Generate Orders Report" << endl;
    cout << "  0.  Logout" << endl;
    cout << endl;
    cout << "====================================" << endl;
    cout << "Choice: ";
}

string Manager::toCSV() {
    return id + "," + name + "," + password + ",MANAGER";
}