#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <stdexcept>

// Класс Character
class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character: " << character.name << ", HP: " << character.health
           << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }
};

// Класс Weapon
class Weapon {
private:
    std::string name;
    int damage;

public:
    Weapon(const std::string& n, int d) : name(n), damage(d) {}

    int getDamage() const { return damage; }
    std::string getName() const { return name; }

    Weapon operator+(const Weapon& other) const {
        return Weapon(name + "-" + other.name, damage + other.damage);
    }

    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
        os << "Weapon: " << weapon.name << ", Damage: " << weapon.damage;
        return os;
    }
};

// Класс Inventory
class Inventory {
private:
    std::unique_ptr<std::string[]> items;
    int capacity;
    int size;

public:
    Inventory(int cap = 10) : capacity(cap), size(0) {
        items = std::make_unique<std::string[]>(capacity);
    }

    void addItem(const std::string& item) {
        if (size < capacity) {
            items[size++] = item;
        } else {
            std::cout << "Inventory is full! Cannot add item: " << item << std::endl;
        }
    }

    void displayInventory() const {
        std::cout << "Inventory (" << size << "/" << capacity << "):" << std::endl;
        for (int i = 0; i < size; ++i) {
            std::cout << "- " << items[i] << std::endl;
        }
    }
};

// Базовый класс Entity
class Entity {
public:
    virtual void displayInfo() const = 0;
    virtual int getHealth() const = 0;
    virtual ~Entity() = default;
};

// Класс Player
class Player : public Entity {
private:
    std::string name;
    int health;
    int level;

public:
    Player(const std::string& n, int h, int l) : name(n), health(h), level(l) {}

    void displayInfo() const override {
        std::cout << "Player: " << name << ", HP: " << health << ", Level: " << level << std::endl;
    }

    int getHealth() const override {
        return health;
    }
};

// Класс Enemy
class Enemy : public Entity {
private:
    std::string name;
    int health;
    std::string type;

public:
    Enemy(const std::string& n, int h, const std::string& t) : name(n), health(h), type(t) {}

    void displayInfo() const override {
        std::cout << "Enemy: " << name << ", HP: " << health << ", Type: " << type << std::endl;
    }

    int getHealth() const override {
        return health;
    }
};

// Шаблонный класс GameManager с обработкой исключений
template <typename T>
class GameManager {
private:
    std::vector<std::unique_ptr<T>> entities;

public:
    void addEntity(std::unique_ptr<T> entity) {
        if (entity->getHealth() <= 0) {
            throw std::invalid_argument("Entity has invalid health");
        }
        entities.push_back(std::move(entity));
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }
};

// Шаблонный класс Queue с обработкой исключений в pop()
template <typename T>
class Queue {
private:
    std::deque<T> data;

public:
    void push(const T& item) {
        data.push_back(item);
    }

    void pop() {
        if (data.empty()) {
            throw std::out_of_range("Queue is empty! Cannot pop.");
        }
        std::cout << "Removed: " << data.front() << std::endl;
        data.pop_front();
    }

    void display() const {
        std::cout << "Queue contents: ";
        for (const auto& item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    // GameManager с обработкой исключений
    GameManager<Entity> manager;
    try {
        manager.addEntity(std::make_unique<Player>("Hero", 100, 1));
        manager.addEntity(std::make_unique<Enemy>("Goblin", 50, "Goblin"));
        manager.addEntity(std::make_unique<Enemy>("Orc", -80, "Brute")); // Ошибка: здоровье < 0
    } catch (const std::invalid_argument& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    std::cout << "\nAll valid game entities:\n";
    manager.displayAll();

    // Queue для чисел с обработкой исключений
    Queue<int> intQueue;
    try {
        intQueue.push(10);
        intQueue.push(20);
        intQueue.push(30);
        intQueue.display();
        intQueue.pop();
        intQueue.display();
        intQueue.pop();
        intQueue.pop();
        intQueue.pop(); // Ошибка: очередь пустая
    } catch (const std::out_of_range& e) {
        std::cerr << "Queue exception: " << e.what() << std::endl;
    }

    // Queue для строк с обработкой исключений
    Queue<std::string> stringQueue;
    try {
        stringQueue.push("Potion");
        stringQueue.push("Scroll");
        stringQueue.display();
        stringQueue.pop();
        stringQueue.display();
        stringQueue.pop();
        stringQueue.pop(); // Ошибка: очередь пустая
    } catch (const std::out_of_range& e) {
        std::cerr << "Queue exception: " << e.what() << std::endl;
    }

    return 0;
}
