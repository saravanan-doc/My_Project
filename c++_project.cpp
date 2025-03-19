#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

// Base class: InventoryItem
class InventoryItem {
protected:
    string name;
    int quantity;

public:
    InventoryItem(const string& name, int quantity) : name(name), quantity(quantity) {}
    virtual void displayItem() const {
        cout << "Item: " << name << ", Quantity: " << quantity << endl;
    }
    virtual ~InventoryItem() {}
};

// Derived class: DepartmentItem (Inheritance and Polymorphism)
class DepartmentItem : public InventoryItem {
    string departmentName;

public:
    DepartmentItem(const string& name, int quantity, const string& dept)
        : InventoryItem(name, quantity), departmentName(dept) {}

    void displayItem() const override {
        cout << "Item: " << name << ", Quantity: " << quantity
             << ", Department: " << departmentName << endl;
    }
};

// Inventory Management System
class InventoryManagementSystem {
    unordered_map<string, unique_ptr<InventoryItem>> inventory;
    mutex mtx;
    condition_variable cv;

public:
    // Add item (Thread-Safe)
    void addItem(const string& name, int quantity, const string& dept) {
        lock_guard<mutex> lock(mtx);
        inventory[name] = make_unique<DepartmentItem>(name, quantity, dept);
        cv.notify_all();
    }

    // Display all items
    void displayInventory() {
        lock_guard<mutex> lock(mtx);
        for (const auto& [key, item] : inventory) {
            item->displayItem();
        }
    }

    // Exception handling example
    void removeItem(const string& name) {
        lock_guard<mutex> lock(mtx);
        if (inventory.find(name) == inventory.end()) {
            throw runtime_error("Item not found!");
        }
        inventory.erase(name);
    }

    // Simple IPC Example using Threads
    void processTransaction(const string& message) {
        thread t([=]() {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&] { return !message.empty(); });
            cout << "Processing IPC Message: " << message << endl;
        });
        t.join();
    }
};

// Main Function
int main() {
    InventoryManagementSystem ims;

    try {
        ims.addItem("Widget", 50, "Assembly");
        ims.addItem("Gadget", 30, "Packaging");

        cout << "Current Inventory: " << endl;
        ims.displayInventory();

        ims.removeItem("Gadget");
        cout << "After Removal: " << endl;
        ims.displayInventory();

        ims.processTransaction("Restock Request");
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
