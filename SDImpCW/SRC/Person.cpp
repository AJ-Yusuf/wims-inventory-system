#include "Person.h"
using namespace std;

Person::Person(string name, string id, string password) {
    this->name     = name;
    this->id       = id;
    this->password = password;
}

string Person::getName()     { return name; }
string Person::getId()       { return id; }
string Person::getPassword() { return password; }

void Person::setName(string n)     { name = n; }
void Person::setPassword(string p) { password = p; }