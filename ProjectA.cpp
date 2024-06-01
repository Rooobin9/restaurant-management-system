#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>

class RawMaterial {
public:
    RawMaterial(int id, const std::string& name, double price)
        : id(id), name(name), price(price) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }

    void setName(const std::string& newName) { name = newName; }
    void setPrice(double newPrice) { price = newPrice; }

private:
    int id;
    std::string name;
    double price;
};

class Dish {
public:
    Dish(int id, const std::string& name, double price)
        : id(id), name(name), price(price) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }

    void setName(const std::string& newName) { name = newName; }
    void setPrice(double newPrice) { price = newPrice; }

private:
    int id;
    std::string name;
    double price;
};

class Customer {
public:
    Customer(int id, const std::string& name)
        : id(id), name(name) {}

    int getId() const { return id; }
    std::string getName() const { return name; }

    void setName(const std::string& newName) { name = newName; }

private:
    int id;
    std::string name;
};

class RestaurantManagementSystem {
public:
    void addRawMaterial(int id, const std::string& name, double price) {
        if (rawMaterials.count(id)) {
            throw std::invalid_argument("Raw material ID already exists.");
        }
        rawMaterials[id] = std::make_unique<RawMaterial>(id, name, price);
    }

    void addDish(int id, const std::string& name, double price) {
        if (dishes.count(id) || dishNames.count(name)) {
            throw std::invalid_argument("Dish ID or name already exists.");
        }
        dishes[id] = std::make_unique<Dish>(id, name, price);
        dishNames[name] = id;
    }

    void addCustomer(int id, const std::string& name) {
        if (customers.count(id)) {
            throw std::invalid_argument("Customer ID already exists.");
        }
        customers[id] = std::make_unique<Customer>(id, name);
    }

    void removeRawMaterial(int id) {
        rawMaterials.erase(id);
    }

    void removeDish(int id) {
        if (dishes.count(id)) {
            dishNames.erase(dishes[id]->getName());
            dishes.erase(id);
        }
    }

    void removeCustomer(int id) {
        customers.erase(id);
    }

    void modifyRawMaterial(int id, const std::string& name, double price) {
        if (!rawMaterials.count(id)) {
            throw std::invalid_argument("Raw material ID does not exist.");
        }
        rawMaterials[id]->setName(name);
        rawMaterials[id]->setPrice(price);
    }

    void modifyDish(int id, const std::string& name, double price) {
        if (!dishes.count(id)) {
            throw std::invalid_argument("Dish ID does not exist.");
        }
        if (dishNames.count(name) && dishNames[name] != id) {
            throw std::invalid_argument("Dish name already exists.");
        }
        dishNames.erase(dishes[id]->getName());
        dishes[id]->setName(name);
        dishes[id]->setPrice(price);
        dishNames[name] = id;
    }

    void modifyCustomer(int id, const std::string& name) {
        if (!customers.count(id)) {
            throw std::invalid_argument("Customer ID does not exist.");
        }
        customers[id]->setName(name);
    }

    double calculateTotalFee(const std::vector<int>& orderIds) const {
        double total = 0.0;
        for (int id : orderIds) {
            if (!dishes.count(id)) {
                throw std::invalid_argument("Dish ID does not exist.");
            }
            total += dishes.at(id)->getPrice();
        }
        return total;
    }

    double calculateGrossProfit() const {
        double grossProfit = 0.0;
        for (const auto& pair : dishes) {
            grossProfit += pair.second->getPrice();
        }
        for (const auto& pair : rawMaterials) {
            grossProfit -= pair.second->getPrice();
        }
        return grossProfit;
    }

private:
    std::unordered_map<int, std::unique_ptr<RawMaterial>> rawMaterials;
    std::unordered_map<int, std::unique_ptr<Dish>> dishes;
    std::unordered_map<std::string, int> dishNames;
    std::unordered_map<int, std::unique_ptr<Customer>> customers;
};

int main() {
    RestaurantManagementSystem rms;

    rms.addRawMaterial(1, "Tomato", 0.5);
    rms.addRawMaterial(2, "Cheese", 1.5);

    rms.addDish(1, "Pizza", 10);
    rms.addDish(2, "Salad", 5);

    rms.addCustomer(1, "Alice");
    rms.addCustomer(2, "Bob");

    std::vector<int> orderIds = { 1, 2 };
    double totalFee = rms.calculateTotalFee(orderIds);
    std::cout << "Total Fee: $" << totalFee << std::endl;

    double grossProfit = rms.calculateGrossProfit();
    std::cout << "Gross Profit: $" << grossProfit << std::endl;

    return 0;
}