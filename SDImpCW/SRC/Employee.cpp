#include "Employee.h"
using namespace std;

Employee::Employee(string name, string id, string password)
    : Person(name, id, password) {}

string Employee::getRole() { return "EMPLOYEE"; }

void Employee::displayMenu() {
    cout << "\n====================================" << endl;
    cout << "  EMPLOYEE MENU - " << name << endl;
    cout << "====================================" << endl;
    cout << "  1. View My Assigned Orders" << endl;
    cout << "  2. Find a Product" << endl;
    cout << "  3. Complete an Order" << endl;
    cout << "  4. Generate My Report" << endl;
    cout << "  0. Logout" << endl;
    cout << "====================================" << endl;
    cout << "Choice: ";
}

string Employee::toCSV() {
    return id + "," + name + "," + password + ",EMPLOYEE";
}