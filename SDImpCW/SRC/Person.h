#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    string name;
    string id;
    string password;

public:
    Person(string name, string id, string password);
    virtual ~Person() {}

    string getName();
    string getId();
    string getPassword();

    void setName(string n);
    void setPassword(string p);

    virtual string getRole()   = 0;
    virtual void displayMenu() = 0;
};

#endif