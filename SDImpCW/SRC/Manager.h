#ifndef MANAGER_H
#define MANAGER_H

#include "Person.h"

class Manager : public Person {
public:
    Manager(string name, string id, string password);

    string getRole();
    void   displayMenu();
    string toCSV();
};

#endif