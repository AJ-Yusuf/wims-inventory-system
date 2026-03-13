#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Person.h"

class Employee : public Person {
public:
    Employee(string name, string id, string password);

    string getRole();
    void   displayMenu();
    string toCSV();
};

#endif