#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>
#include <map>
using namespace std;

enum OrderStatus { PENDING, ASSIGNED, COMPLETED };

class Order {
private:
    string orderId;
    map<string, int> items;
    string assignedEmployeeId;
    OrderStatus status;

public:
    Order(string orderId);

    string           getOrderId();
    string           getAssignedEmployeeId();
    OrderStatus      getStatus();
    map<string, int> getItems();
    string           getStatusString();

    void addItem(string productId, int quantity);
    void removeItem(string productId);
    void setAssignedEmployee(string employeeId);
    void setStatus(OrderStatus s);

    void   display();
    string toCSV();
};

#endif