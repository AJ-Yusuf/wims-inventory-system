#ifndef INVENTORYSYSTEM_H
#define INVENTORYSYSTEM_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Product.h"
#include "Order.h"
#include "Manager.h"
#include "Employee.h"

using namespace std;

class InventorySystem {
private:
    vector<Product> products;
    vector<Order>   orders;
    vector<Person*> users;
    Person*         currentUser;

    string PRODUCTS_FILE = "DATA/products.csv";
    string ORDERS_FILE   = "DATA/orders.csv";
    string USERS_FILE    = "DATA/users.csv";

    // Helpers
    string   trim(string s);
    Product* findProductById(string id);
    Order*   findOrderById(string id);
    Person*  findUserById(string id);
    bool     productIdExists(string id);
    bool     orderIdExists(string id);
    bool     userIdExists(string id);

    // File I/O
    void loadUsers();
    void loadProducts();
    void loadOrders();
    void saveUsers();
    void saveProducts();
    void saveOrders();

    // Auth
    Person* login();

    // Manager actions
    void addProduct();
    void viewProducts();
    void editProduct();
    void deleteProduct();
    void addEmployee();
    void viewEmployees();
    void editEmployee();
    void createOrder();
    void viewOrders();
    void editOrder();
    void assignOrder();
    void generateInventoryReport();
    void generateOrdersReport();

    // Employee actions
    void viewMyOrders();
    void findProduct();
    void completeOrder();
    void generateMyReport();

    // Menu loops
    void managerLoop();
    void employeeLoop();

public:
    InventorySystem();
    ~InventorySystem();
    void run();
};

#endif       