# Warehouse Inventory Management System (WIMS)

A role-based warehouse stock and task management system written from scratch in C++17. Managers maintain the product catalogue, the employee register and the order book; employees pick up assigned orders, search stock and complete orders, which deducts stock automatically. All state persists between runs in flat CSV files, with no database engine required.

Built as individual coursework for **SOFT20091 Software Design and Implementation**, BSc Computer Science, Nottingham Trent University.

---

## Features

**Manager**
- Add, view, edit and delete products
- Register, view and edit employees
- Create orders, assign them to employees, view and edit the order book
- Export inventory and order reports to CSV

**Employee**
- View orders assigned to them
- Search products by ID or name (case-insensitive)
- Complete an order, which validates and then deducts stock
- Export a personal order report

**Both**
- Role-based login, with each role routed to its own menu
- Input validation and error handling on every menu path

---

## Architecture

Three layers, with data, user identity and business logic kept separate.

| Layer | Classes | Responsibility |
|-------|---------|----------------|
| Data | `Product`, `Order` | Entity state and validation |
| Users | `Person` (abstract), `Manager`, `Employee` | Identity, role, menu behaviour |
| Controller | `InventorySystem` | Persistence, authentication, business logic |

`Person` is an abstract base class with pure virtual `getRole()` and `displayMenu()`, so a single `Person*` drives either menu through dynamic dispatch and any new role has to implement both at compile time.

**Things worth pointing at in the code:**
- `completeOrder()` validates stock for every line of the order before deducting anything, so a short-stocked line can't leave the inventory half-updated.
- The CSV parser handles a two-level nested format: order items are stored as `productId:quantity;productId:quantity` inside a single field.
- Custom exceptions are thrown from constructors and setters for invalid state such as negative quantity or price, and caught at the menu boundary.
- Heap-allocated users are cleaned up in the `InventorySystem` destructor.

STL used throughout: `vector` for products, orders and users; `map` for order lines; `sort` and `find_if` with lambdas; `transform` for case-insensitive matching.

---

## Layout

```
SDImpCW/
├── SRC/        Source and headers (13 files)
├── TESTS/      Unit and acceptance tests
└── DATA/       CSV persistence and exported reports
```

---

## Build and run

Compile and run from inside `SDImpCW`, since the data paths are relative to it.

```bash
cd SDImpCW
g++ -std=c++17 SRC/*.cpp -o wims
./wims
```

Sample logins are in `DATA/Users.csv` (for example `MGR001` for the manager menu, `EMP002` for the employee menu).

### Tests

```bash
cd SDImpCW
g++ -std=c++17 TESTS/tests.cpp SRC/Product.cpp SRC/Order.cpp \
    SRC/Person.cpp SRC/Manager.cpp SRC/Employee.cpp -o tests
./tests
```

The suite covers each class's getters, setters and validation rules, and reports a pass/fail count. It sits alongside a written test plan and user acceptance testing against the coursework scenario.

---

## Design documentation

The submitted work included full UML: use case, sequence, state and class diagrams. The implementation follows Single Responsibility, Open/Closed, DRY and Separation of Concerns, and targets low coupling with high cohesion.

---

## Known limitations

These are deliberate consequences of the brief rather than oversights, and are the first things I would change:

- **Passwords are stored in plaintext** in `DATA/Users.csv`. The spec called for flat-file storage; production would hash and salt them.
- **Flat-file persistence** has no concurrency control, so the system assumes a single user at a time.
- **No transactional rollback** across files. Stock validation runs up front to compensate.

---

## Stack

C++17 · STL · CSV file I/O · Git
