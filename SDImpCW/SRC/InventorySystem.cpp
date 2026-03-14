#include "InventorySystem.h"
using namespace std;

//  Constructor and Destructor
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

//  Run
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


//  Login
Person* InventorySystem::login() {
    cout << "\n--- LOGIN ---" << endl;
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
            cout << "Welcome, " << users[i]->getName()
                 << "! (" << users[i]->getRole() << ")" << endl;
            return users[i];
        }
    }
    cout << "Invalid credentials. Try again." << endl;
    return login();
}



//  Manager Loop
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


//  Employee Loop
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
//  Manager — Orders
// ─────────────────────────────────────────
void InventorySystem::createOrder() {
    cout << "\n-- Create Order --" << endl;
    cout << "Order ID: ";
    string oid; getline(cin, oid); oid = trim(oid);
    if (oid.empty() || orderIdExists(oid)) {
        cout << "Error: ID is empty or already exists." << endl; return;
    }

    Order newOrder(oid);
    cout << "Add items (type 'done' to finish):" << endl;
    while (true) {
        cout << "  Product ID: ";
        string pid; getline(cin, pid); pid = trim(pid);
        if (pid == "done") break;
        if (!findProductById(pid)) {
            cout << "  Product not found. Try again." << endl; continue;
        }
        cout << "  Quantity: ";
        string qstr; getline(cin, qstr);
        try {
            newOrder.addItem(pid, stoi(qstr));
            cout << "  Item added." << endl;
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
}

void InventorySystem::editOrder() {
    cout << "\n-- Edit Order --" << endl;
    cout << "Order ID to edit: ";
    string oid; getline(cin, oid); oid = trim(oid);
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
    cout << "Order ID    : "; string oid; getline(cin, oid); oid = trim(oid);
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
    cout << "Search by (1=ID, 2=Name): ";
    string mode; getline(cin, mode);
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
    cout << "Order ID: ";
    string oid; getline(cin, oid); oid = trim(oid);
    Order* o = findOrderById(oid);

    if (!o) { cout << "Order not found." << endl; return; }
    if (o->getAssignedEmployeeId() != currentUser->getId()) {
        cout << "This order is not assigned to you." << endl; return;
    }
    if (o->getStatus() == COMPLETED) {
        cout << "Order already completed." << endl; return;
    }

    // Check stock first
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

    // All good — deduct stock
    for (auto it = items.begin(); it != items.end(); it++) {
        findProductById(it->first)->reduceQuantity(it->second);
    }
    o->setStatus(COMPLETED);
    cout << "Order completed. Stock updated." << endl;
}

void InventorySystem::generateMyReport() {
    string filename = "DATA/report_" + currentUser->getId() + ".csv";
    ofstream f(filename);
    if (!f.is_open()) { cout << "Could not create file." << endl; return; }
    f << "OrderID,Status,Items" << endl;
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].getAssignedEmployeeId() == currentUser->getId())
            f << orders[i].toCSV() << endl;
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
    getline(f, line); // skip header
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
    getline(f, line); // skip header
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
    getline(f, line); // skip header
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

        // parse items: prodId:qty;prodId:qty
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

        // restore correct status
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