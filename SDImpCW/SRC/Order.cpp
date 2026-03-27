#include "Order.h"
using namespace std;

Order::Order(string orderId) {
    this->orderId            = orderId;
    this->assignedEmployeeId = "";
    this->status             = PENDING;
}

string           Order::getOrderId()            { return orderId; }
string           Order::getAssignedEmployeeId() { return assignedEmployeeId; }
OrderStatus      Order::getStatus()             { return status; }
map<string, i
    status = ASSIGNED;
}

void Order::setStatus(OrderStatus s) {
    status = s;
}

void Order::display() {
    cout << "  Order ID : " << orderId << endl;
    cout << "  Status   : " << getStatusString() << endl;
    cout << "  Assigned : " << (assignedEmployeeId.empty() ? "Unassigned" : assignedEmployeeId) << endl;
    cout << "  Items    :" << endl;
    for (auto it = items.begin(); it != items.end(); it++) {
        cout << "    - " << it->first << "  x" << it->second << endl;
    }
}

string Order::toCSV() {
    string result = orderId + "," + getStatusString() + "," + assignedEmployeeId + ",";
    bool first = true;
    for (auto it = items.begin(); it != items.end(); it++) {
        if (!first) result += ";";
        result += it->first + ":" + to_string(it->second);
        first = false;
    }
    return result;
}