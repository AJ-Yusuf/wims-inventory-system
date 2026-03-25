#include <iostream>
#include <cassert>
#include "../SRC/Product.h"
#include "../SRC/Order.h"
#include "../SRC/Manager.h"
#include "../SRC/Employee.h"
using namespace std;

int passed = 0;
int failed = 0;

void test(string name, bool condition) {
    if (condition) {
        cout << "  [PASS] " << name << endl;
        passed++;
    } else {
        cout << "  [FAIL] " << name << endl;
        failed++;
    }
}

// ─────────────────────────────────────────
//  Product Tests
// ─────────────────────────────────────────
void testProduct() {
    cout << "\n--- Product Tests ---" << endl;

    Product p("W_MOUSE_101", "Wireless Mouse", 500, "Aisle C", 29.99);
    test("Product ID stored correctly",       p.getProductId() == "W_MOUSE_101");
    test("Product name stored correctly",     p.getName() == "Wireless Mouse");
    test("Product quantity stored correctly", p.getQuantity() == 500);
    test("Product location stored correctly", p.getLocation() == "Aisle C");
    test("Product price stored correctly",    p.getPrice() == 29.99);

    p.setName("Wireless Mouse Pro");
    test("setName works", p.getName() == "Wireless Mouse Pro");

    p.setQuantity(450);
    test("setQuantity works", p.getQuantity() == 450);

    p.setPrice(34.99);
    test("setPrice works", p.getPrice() == 34.99);

    p.setLocation("Aisle D");
    test("setLocation works", p.getLocation() == "Aisle D");

    bool reduced = p.reduceQuantity(50);
    test("reduceQuantity succeeds with enough stock", reduced == true);
    test("Quantity correctly reduced",                p.getQuantity() == 400);

    bool overReduce = p.reduceQuantity(9999);
    test("reduceQuantity fails when insufficient stock", overReduce == false);

    try {
        Product bad("X", "Bad", -1, "Loc", 10.0);
        test("Negative quantity throws exception", false);
    } catch (invalid_argument&) {
        test("Negative quantity throws exception", true);
    }

    try {
        Product bad2("X", "Bad", 10, "Loc", -5.0);
        test("Negative price throws exception", false);
    } catch (invalid_argument&) {
        test("Negative price throws exception", true);
    }

    string csv = p.toCSV();
    test("toCSV contains product ID", csv.find("W_MOUSE_101") != string::npos);
    test("toCSV contains name",       csv.find("Wireless Mouse Pro") != string::npos);
}

// ─────────────────────────────────────────
//  Order Tests
// ─────────────────────────────────────────
void testOrder() {
    cout << "\n--- Order Tests ---" << endl;

    Order o("ORD001");
    test("Order ID stored correctly",  o.getOrderId() == "ORD001");
    test("Initial status is PENDING",  o.getStatus() == PENDING);
    test("Initially unassigned",       o.getAssignedEmployeeId() == "");

    o.addItem("W_MOUSE_101", 50);
    o.addItem("KEYBOARD_102", 30);
    test("Two items in order",    o.getItems().size() == 2);
    test("Correct mouse qty",     o.getItems().at("W_MOUSE_101") == 50);
    test("Correct keyboard qty",  o.getItems().at("KEYBOARD_102") == 30);

    o.addItem("W_MOUSE_101", 10);
    test("Adding same item accumulates quantity", o.getItems().at("W_MOUSE_101") == 60);

    o.removeItem("KEYBOARD_102");
    test("Item removed from order", o.getItems().size() == 1);

    o.setAssignedEmployee("EMP001");
    test("Status becomes ASSIGNED", o.getStatus() == ASSIGNED);
    test("Employee ID stored",      o.getAssignedEmployeeId() == "EMP001");

    o.setStatus(COMPLETED);
    test("Status set to COMPLETED",    o.getStatus() == COMPLETED);
    test("Status string is COMPLETED", o.getStatusString() == "COMPLETED");

    string csv = o.toCSV();
    test("toCSV contains order ID", csv.find("ORD001") != string::npos);
    test("toCSV contains item",     csv.find("W_MOUSE_101") != string::npos);
}

// ─────────────────────────────────────────
//  Person / Inheritance Tests
// ─────────────────────────────────────────
void testPerson() {
    cout << "\n--- Person / Inheritance Tests ---" << endl;

    Manager mgr("Sarah", "MGR001", "pass123");
    test("Manager role correct",     mgr.getRole() == "MANAGER");
    test("Manager name correct",     mgr.getName() == "Sarah");
    test("Manager ID correct",       mgr.getId() == "MGR001");
    test("Manager password correct", mgr.getPassword() == "pass123");

    Employee emp("David", "EMP001", "emp123");
    test("Employee role correct",    emp.getRole() == "EMPLOYEE");
    test("Employee name correct",    emp.getName() == "David");
    test("Employee ID correct",      emp.getId() == "EMP001");

    emp.setName("Dave");
    test("setName works", emp.getName() == "Dave");

    emp.setPassword("newpass");
    test("setPassword works", emp.getPassword() == "newpass");

    string mgrCSV = mgr.toCSV();
    test("Manager CSV contains MANAGER", mgrCSV.find("MANAGER") != string::npos);

    string empCSV = emp.toCSV();
    test("Employee CSV contains EMPLOYEE", empCSV.find("EMPLOYEE") != string::npos);
}

// ─────────────────────────────────────────
//  Main
// ─────────────────────────────────────────
int main() {
    cout << "========================================" << endl;
    cout << "           WIMS Unit Tests              " << endl;
    cout << "========================================" << endl;

    testProduct();
    testOrder();
    testPerson();

    cout << "\n========================================" << endl;
    cout << "  Results: " << passed << " passed, " << failed << " failed" << endl;
    cout << "========================================" << endl;

    return (failed == 0) ? 0 : 1;
}
