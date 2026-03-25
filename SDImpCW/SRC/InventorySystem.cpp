#include "InventorySystem.h"
using namespace std;

// ─────────────────────────────────────────
//  Constructor and Destructor
// ─────────────────────────────────────────
InventorySystem::InventorySystem() {
    currentUser = nullptr;
    loadUsers();
    loadProducts();
    loadOrders();
}

InventorySystem::~InventorySystem() {
    for (int i = 0; i < users.size(); i++)
        delete users[i];
}

// ─────────────────────────────────────────
//  Run
// ─────────────────────────────────────────
void InventorySystem::run() {
    cout << "\n========================================" << endl;
    cout << "  Warehouse Inventory Management System  " << endl;
    cout << "========================================" << endl;


    while (true) {
        currentUser = login();
        if (!currentUser) {
            cout << "Goodbye!" << endl;
            break;
        }
        if (currentUser->getRole() == "MANAGER")
            managerLoop();
        else
            employeeLoop();
    }
}

// ─────────────────────────────────────────
//  Login
// ─────────────────────────────────────────
Person* InventorySystem::login() {
    cout << "\n--- LOGIN ---" << endl;
    cout << endl;
    cout << "Enter ID (or 'quit' to exit): ";
    string id;
    getline(cin, id);
    id = trim(id);
    if (id == "quit") return nullptr;

    cout << "Enter Password: ";
    string pw;
    getline(cin, pw);
    pw = trim(pw);

    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getId() == id && users[i]->getPassword() == pw) {
            cout << endl;
            cout << "Welcome, " << users[i]->getName()
                 << "! (" << users[i]->getRole() << ")" << endl;
            return users[i];
        }
    }
    cout << "Invalid credentials. Try again." << endl;
    return login();
}

// ─────────────────────────────────────────
//  Manager Loop
// ─────────────────────────────────────────
void InventorySystem::managerLoop() {
    string input;
    int choice;
    do {
        currentUser->displayMenu();
        getline(cin, input);
        try { choice = stoi(input); }
        catch (...) { cout << "Please enter a number." << endl; choice = -1; continue; }

        if      (choice == 1)  addProduct();
        else if (choice == 2)  viewProducts();
        else if (choice == 3)  editProduct();
        else if (choice == 4)  deleteProduct();
        else if (choice == 5)  addEmployee();
        else if (choice == 6)  viewEmployees();
        else if (choice == 7)  editEmployee();
        else if (choice == 8)  createOrder();
        else if (choice == 9)  viewOrders();
        else if (choice == 10) editOrder();
        else if (choice == 11) assignOrder();
        else if (choice == 12) generateInventoryReport();
        else if (choice == 13) generateOrdersReport();
        else if (choice == 0) {
            saveProducts();
            saveOrders();
            saveUsers();
            cout << "Data saved. Logged out." << endl;
        }
        else cout << "Unknown option." << endl;
    } while (choice != 0);
}

// ─────────────────────────────────────────
//  Employee Loop
// ─────────────────────────────────────────
void InventorySystem::employeeLoop() {
    string input;
    int choice;
    do {
        currentUser->displayMenu();
        getline(cin, input);
        try { choice = stoi(input); }
        catch (...) { cout << "Please enter a number." << endl; choice = -1; continue; }

        if      (choice == 1) viewMyOrders();
        else if (choice == 2) findProduct();
        else if (choice == 3) completeOrder();
        else if (choice == 4) generateMyReport();
        else if (choice == 0) {
            saveProducts();
            saveOrders();
            cout << "Data saved. Logged out." << endl;
        }
        else cout << "Unknown option." << endl;
    } while (choice != 0);
}

// ─────────────────────────────────────────
//  Manager — Products
// ─────────────────────────────────────────
void InventorySystem::addProduct() {
    cout << "\n----- Add Product -----" << endl;
    cout << endl;
    cout << "Type in th edetails of the product to add" << endl;
    cout << endl;
    cout << "(Enter 0 to return to menu)" << endl;
    cout << endl;
    string id, name, location, qstr, pstr;
    cout << "Product ID : "; getline(cin, id);       id = trim(id);
    if (id == "0") return;
    cout << "Name       : "; getline(cin, name);     name = trim(name);
    cout << "Location   : "; getline(cin, location); location = trim(location);
    cout << "Quantity   : "; getline(cin, qstr);
    cout << "Price      : "; getline(cin, pstr);

    if (id.empty() || productIdExists(id)) {
        cout << "Error: ID is empty or already exists." << endl; return;
    }
    try {
        products.push_back(Product(id, name, stoi(qstr), location, stod(pstr)));
        cout << "Product added successfully." << endl;
    } catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void InventorySystem::viewProducts() {
    if (products.empty()) { cout << "No products in inventory." << endl; return; }
    cout << "\n-- All Products --" << endl;
    cout << string(60, '-') << endl;

    sort(products.begin(), products.end(), [](Product a, Product b) {
        return a.getName() < b.getName();
    });

    for (int i = 0; i < products.size(); i++) {
        products[i].display();
        cout << string(60, '-') << endl;
    }
    cout << "\n(Enter 0 to return to menu): ";
    string input; getline(cin, input);
}

void InventorySystem::editProduct() {
    cout << "\n-- Edit Product --" << endl;
    cout << "(Enter 0 to return to menu)" << endl;
    cout << "Product ID to edit: ";
    string id; getline(cin, id); id = trim(id);
    if (id == "0") return;
    Product* p = findProductById(id);
    if (!p) { cout << "Product not found." << endl; return; }

    cout << "(Press Enter to keep current value)" << endl;
    string v;
    try {
        cout << "Name     [" << p->getName() << "]: ";
        getline(cin, v); v = trim(v);
        if (!v.empty()) p->setName(v);

        cout << "Quantity [" << p->getQuantity() << "]: ";
        getline(cin, v); v = trim(v);
        if (!v.empty()) p->setQuantity(stoi(v));

        cout << "Location [" << p->getLocation() << "]: ";
        getline(cin, v); v = trim(v);
        if (!v.empty()) p->setLocation(v);

        cout << "Price    [" << p->getPrice() << "]: ";
        getline(cin, v); v = trim(v);
        if (!v.empty()) p->setPrice(stod(v));

        cout << "Product updated." << endl;
    } catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void InventorySystem::deleteProduct() {
    cout << "\n-- Delete Product --" << endl;
    cout << "(Enter 0 to return to menu)" << endl;
    cout << "Product ID to delete: ";
    string id; getline(cin, id); id = trim(id);
    if (id == "0") return;
    for (int i = 0; i < products.size(); i++) {
        if (products[i].getProductId() == id) {
            products.erase(products.begin() + i);
            cout << "Product deleted." << endl;
            return;
        }
    }
    cout << "Product not found." << endl;
}

// ─────────────────────────────────────────
//  Manager — Employees
// ─────────────────────────────────────────
void InventorySystem::addEmployee() {
    cout << "\n----- Add Employee -----" << endl;
    cout << "(Enter 0 to return to menu)" << endl;
    string id, name, pw;
    cout << "Employee ID : "; getline(cin, id);   id = trim(id);
    if (id == "0") return;
    cout << "Name        : "; getline(cin, name); name = trim(name);
    cout << "Password    : "; getline(cin, pw);   pw = trim(pw);

    if (id.empty() || userIdExists(id)) {
        cout << "Error: ID is empty or already exists." << endl; return;
    }
    if (name.empty() || pw.empty()) {
        cout << "Error: Name and password cannot be empty." << endl; return;
    }
    users.push_back(new Employee(name, id, pw));
    cout << "Employee added successfully." << endl;
}

void InventorySystem::viewEmployees() {
    cout << "\n-- All Employees --" << endl;
    bool found = false;
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getRole() == "EMPLOYEE") {
            cout << "  ID: " << users[i]->getId()
                 << "  Name: " << users[i]->getName() << endl;
            found = true;
        }
    }
    if (!found) cout << "No employees found." << endl;
    cout << "\n(Enter 0 to return to menu): ";
    string input; getline(cin, input);
}

void InventorySystem::editEmployee() {
    cout << "\n-- Edit Employee --" << endl;
    cout << "(Enter 0 to return to menu)" << endl;
    cout << "Employee ID to edit: ";
    string id; getline(cin, id); id = trim(id);
    if (id == "0") return;
    Person* p = findUserById(id);
    if (!p || p->getRole() != "EMPLOYEE") {
        cout << "Employee not found." << endl; return;
    }
    cout << "(Press Enter to keep current value)" << endl;
    string v;

    cout << "Name     [" << p->getName() << "]: ";
    getline(cin, v); v = trim(v);
    if (!v.empty()) p->setName(v);

    cout << "Password [" << p->getPassword() << "]: ";
    getline(cin, v); v = trim(v);
    if (!v.empty()) p->setPassword(v);

    cout << "Employee updated." << endl;
}

// ─────────────────────────────────────────
//  Manager — Orders
// ─────────────────────────────────────────
void InventorySystem::createOrder() {
    cout << "\n-- Create Order --" << endl;
    cout << "(Enter 0 to return to menu)" << endl;
    cout << "Order ID: ";
    string oid; getline(cin, oid); oid = trim(oid);
    if (oid == "0") return;
    if (oid.empty() || orderIdExists(oid)) {
        cout << "Error: ID is empty or already exists." << endl; return;
    }

    Order newOrder(oid);
    while (true) {
        cout << "  Product ID: ";
        string pid; getline(cin, pid); pid = trim(pid);
        if (pid == "0") return;
        if (pid == "done") break;
        if (!findProductById(pid)) {
            cout << "  Product not found. Try again." << endl; continue;
        }
        cout << "  Quantity: ";
        string qstr; getline(cin, qstr);
        try {
            newOrder.addItem(pid, stoi(qstr));
            cout << "  Item added." << endl;
            cout << "Add items (type 'done' to finish, or '0' to cancel):" << endl;

        } catch (exception& e) {
            cout << "  Error: " << e.what() << endl;
        }
    }
    if (newOrder.getItems().empty()) {
        cout << "No items added. Order cancelled." << endl; return;
    }
    orders.push_back(newOrder);
    cout << "Order " << oid << " created." << endl;
}

void InventorySystem::viewOrders() {
    if (orders.empty()) { cout << "No orders." << endl; return; }
    cout << "\n-- All Orders --" << endl;
    for (int i = 0; i < orders.size(); i++) {
        cout << string(50, '-') << endl;
        orders[i].display();
    }
    cout << string(50, '-') << endl;
    cout << "\n(Enter 0 to return to menu): ";
    string input; getline(cin, input);
}

void InventorySystem::editOrder() {
    cout << "\n-- Edit Order --" << endl;
    cout << "(Enter 0 to return to menu)" << endl;
    cout << "Order ID to edit: ";
    string oid; getline(cin, oid); oid = trim(oid);
    if (oid == "0") return;
    Order* o = findOrderById(oid);
    if (!o) { cout << "Order not found." << endl; return; }
    if (o->getStatus() == COMPLETED) {
        cout << "Cannot edit a completed order." << endl; return;
    }

    cout << "Edit (1=Add item, 2=Remove item): ";
    string c; getline(cin, c);
    if (c == "1") {
        cout << "Product ID: "; string pid; getline(cin, pid); pid = trim(pid);
        if (!findProductById(pid)) { cout << "Product not found." << endl; return; }
        cout << "Quantity: "; string qstr; getline(cin, qstr);
        try {
            o->addItem(pid, stoi(qstr));
            cout << "Item added to order." << endl;
        } catch (exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    } else if (c == "2") {
        cout << "Product ID to remove: ";
        string pid; getline(cin, pid);
        o->removeItem(trim(pid));
        cout << "Item removed." << endl;
    } else {
        cout << "Invalid choice." << endl;
    }
}

void InventorySystem::assignOrder() {
    cout << "\n-- Assign Order --" << endl;
    bool found = false;
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].getAssignedEmployeeId().empty() && orders[i].getStatus() != COMPLETED) {
            cout << "  Order ID: " << orders[i].getOrderId() << "  Items: " << orders[i].getItems().size() << endl;
            found = true;
        }
    }
    if (!found) { cout << "No unassigned orders." << endl; return; }
    cout << endl;
    cout << "(Enter 0 to return to menu)" << endl;
    cout << "Order ID    : "; string oid; getline(cin, oid); oid = trim(oid);
    if (oid == "0") return;
    Order* o = findOrderById(oid);
    if (!o) { cout << "Order not found." << endl; return; }
    if (o->getStatus() == COMPLETED) {
        cout << "Order already completed." << endl; return;
    }

    cout << "Employee ID : "; string eid; getline(cin, eid); eid = trim(eid);
    Person* emp = findUserById(eid);
    if (!emp || emp->getRole() != "EMPLOYEE") {
        cout << "Employee not found." << endl; return;
    }
    o->setAssignedEmployee(eid);
    cout << "Order assigned to " << emp->getName() << "." << endl;
}

// ─────────────────────────────────────────
//  Manager — Reports
// ─────────────────────────────────────────
void InventorySystem::generateInventoryReport() {
    string filename = "DATA/inventory_report.csv";
    ofstream f(filename);
    if (!f.is_open()) { cout << "Could not create file." << endl; return; }
    f << "ProductID,Name,Quantity,Location,Price" << endl;
    for (int i = 0; i < products.size(); i++)
        f << products[i].toCSV() << endl;
    f.close();
    cout << "Inventory report saved to: " << filename << endl;
}

void InventorySystem::generateOrdersReport() {
    string filename = "DATA/orders_report.csv";
    ofstream f(filename);
    if (!f.is_open()) { cout << "Could not create file." << endl; return; }
    f << "OrderID,Status,AssignedEmployee,Items" << endl;
    for (int i = 0; i < orders.size(); i++)
        f << orders[i].toCSV() << endl;
    f.close();
    cout << "Orders report saved to: " << filename << endl;
}

// ─────────────────────────────────────────
//  Employee — Actions
// ─────────────────────────────────────────
void InventorySystem::viewMyOrders() {
    cout << "\n-- My Assigned Orders --" << endl;
    bool found = false;
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].getAssignedEmployeeId() == currentUser->getId() &&
            orders[i].getStatus() != COMPLETED) {
            cout << string(50, '-') << endl;
            orders[i].display();
            found = true;
        }
    }
    if (!found) cout << "No orders assigned to you." << endl;
    cout << string(50, '-') << endl;
}

void InventorySystem::findProduct() {
    cout << "\n-- Find Product --" << endl;
    cout << "(Enter 0 to return to menu)" << endl;
    cout << "Search by (1=ID, 2=Name): ";
    string mode; getline(cin, mode);
    if (trim(mode) == "0") return;
    cout << "Search term: ";
    string term; getline(cin, term); term = trim(term);

    auto it = find_if(products.begin(), products.end(), [&](Product p) {
        if (mode == "1") return p.getProductId() == term;
        string pName  = p.getName();
        string tLower = term;
        transform(pName.begin(),  pName.end(),  pName.begin(),  ::tolower);
        transform(tLower.begin(), tLower.end(), tLower.begin(), ::tolower);
        return pName.find(tLower) != string::npos;
    });

    if (it == products.end()) {
        cout << "No product found matching '" << term << "'." << endl;
    } else {
        cout << string(50, '-') << endl;
        it->display();
        cout << string(50, '-') << endl;
    }
}

void InventorySystem::completeOrder() {
    cout << "\n-- Complete Order --" << endl;
    bool hasOrders = false;
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].getAssignedEmployeeId() == currentUser->getId() &&
            orders[i].getStatus() != COMPLETED) {
            hasOrders = true;
            break;
        }
    }
    if (!hasOrders) { cout << "No orders for you :)" << endl; return; }
    cout << "(Enter 0 to return to menu)" << endl;
    cout << "Order ID: ";
    string oid; getline(cin, oid); oid = trim(oid);
    if (oid == "0") return;
    Order* o = findOrderById(oid);

    if (!o) { cout << "Order not found." << endl; return; }
    if (o->getAssignedEmployeeId() != currentUser->getId()) {
        cout << "This order is not assigned to you." << endl; return;
    }
    if (o->getStatus() == COMPLETED) {
        cout << "Order already completed." << endl; return;
    }

    map<string, int> items = o->getItems();
    for (auto it = items.begin(); it != items.end(); it++) {
        Product* p = findProductById(it->first);
        if (!p) {
            cout << "Product " << it->first << " not found." << endl; return;
        }
        if (p->getQuantity() < it->second) {
            cout << "Not enough stock for " << it->first << "." << endl; return;
        }
    }

    for (auto it = items.begin(); it != items.end(); it++) {
        findProductById(it->first)->reduceQuantity(it->second);
    }
    o->setStatus(COMPLETED);
    cout << "Order completed. Stock updated." << endl;
}

void InventorySystem::generateMyReport() {
    string filename = "DATA/employee_orders_report.csv";
    ofstream f(filename);
    if (!f.is_open()) { cout << "Could not create file." << endl; return; }
    f << "EmployeeID,EmployeeName,OrderID,Status,Items" << endl;
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getRole() != "EMPLOYEE") continue;
        for (int j = 0; j < orders.size(); j++) {
            if (orders[j].getAssignedEmployeeId() == users[i]->getId())
                f << users[i]->getId() << "," << users[i]->getName() << "," << orders[j].toCSV() << endl;
        }
    }
    f.close();
    cout << "Report saved to: " << filename << endl;
}

// ─────────────────────────────────────────
//  File Loading
// ─────────────────────────────────────────
void InventorySystem::loadUsers() {
    ifstream f(USERS_FILE);
    if (!f.is_open()) return;
    string line;
    getline(f, line);
    while (getline(f, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string id, name, pw, role;
        getline(ss, id,   ',');
        getline(ss, name, ',');
        getline(ss, pw,   ',');
        getline(ss, role, ',');
        role = trim(role);
        if (role == "MANAGER")
            users.push_back(new Manager(trim(name), trim(id), trim(pw)));
        else
            users.push_back(new Employee(trim(name), trim(id), trim(pw)));
    }
}

void InventorySystem::loadProducts() {
    ifstream f(PRODUCTS_FILE);
    if (!f.is_open()) return;
    string line;
    getline(f, line);
    while (getline(f, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string id, name, qstr, loc, pstr;
        getline(ss, id,   ',');
        getline(ss, name, ',');
        getline(ss, qstr, ',');
        getline(ss, loc,  ',');
        getline(ss, pstr, ',');
        try {
            products.push_back(Product(trim(id), trim(name),
                               stoi(trim(qstr)), trim(loc), stod(trim(pstr))));
        } catch (...) {}
    }
}

void InventorySystem::loadOrders() {
    ifstream f(ORDERS_FILE);
    if (!f.is_open()) return;
    string line;
    getline(f, line);
    while (getline(f, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string oid, statusStr, empId, itemsStr;
        getline(ss, oid,       ',');
        getline(ss, statusStr, ',');
        getline(ss, empId,     ',');
        getline(ss, itemsStr,  ',');

        Order o(trim(oid));

        if (!trim(empId).empty())
            o.setAssignedEmployee(trim(empId));

        istringstream itemStream(itemsStr);
        string token;
        while (getline(itemStream, token, ';')) {
            int colon = token.find(':');
            if (colon != string::npos) {
                string pid = trim(token.substr(0, colon));
                int qty    = stoi(trim(token.substr(colon + 1)));
                o.addItem(pid, qty);
            }
        }

        statusStr = trim(statusStr);
        if (statusStr == "ASSIGNED")       o.setStatus(ASSIGNED);
        else if (statusStr == "COMPLETED") o.setStatus(COMPLETED);
        else                               o.setStatus(PENDING);

        orders.push_back(o);
    }
}

// ─────────────────────────────────────────
//  File Saving
// ─────────────────────────────────────────
void InventorySystem::saveUsers() {
    ofstream f(USERS_FILE);
    f << "ID,Name,Password,Role" << endl;
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getRole() == "MANAGER")
            f << dynamic_cast<Manager*>(users[i])->toCSV() << endl;
        else
            f << dynamic_cast<Employee*>(users[i])->toCSV() << endl;
    }
}

void InventorySystem::saveProducts() {
    ofstream f(PRODUCTS_FILE);
    f << "ProductID,Name,Quantity,Location,Price" << endl;
    for (int i = 0; i < products.size(); i++)
        f << products[i].toCSV() << endl;
}

void InventorySystem::saveOrders() {
    ofstream f(ORDERS_FILE);
    f << "OrderID,Status,AssignedEmployee,Items" << endl;
    for (int i = 0; i < orders.size(); i++)
        f << orders[i].toCSV() << endl;
}

// ─────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────
string InventorySystem::trim(string s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

Product* InventorySystem::findProductById(string id) {
    for (int i = 0; i < products.size(); i++) {
        if (products[i].getProductId() == id) return &products[i];
    }
    return nullptr;
}

Order* InventorySystem::findOrderById(string id) {
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].getOrderId() == id) return &orders[i];
    }
    return nullptr;
}

Person* InventorySystem::findUserById(string id) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getId() == id) return users[i];
    }
    return nullptr;
}

bool InventorySystem::productIdExists(string id) {
    return findProductById(id) != nullptr;
}

bool InventorySystem::orderIdExists(string id) {
    return findOrderById(id) != nullptr;
}

bool InventorySystem::userIdExists(string id) {
    return findUserById(id) != nullptr;
}