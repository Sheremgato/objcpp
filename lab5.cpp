#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <deque>

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
};

// Шаблонный класс GameManager
template <typename T>
class GameManager {
private:
    std::vector<std::unique_ptr<T>> entities;

public:
    void addEntity(std::unique_ptr<T> entity) {
        entities.push_back(std::move(entity));
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }
};

// Шаблонный класс Queue
template <typename T>
class Queue {
private:
    std::deque<T> data;

public:
    void push(const T& item) {
        data.push_back(item);
    }

    void pop() {
        if (!data.empty()) {
            std::cout << "Removed: " << data.front() << std::endl;
            data.pop_front();
        } else {
            std::cout << "Queue is empty!" << std::endl;
        }
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
    // Character
    Character hero1("Hero", 100, 20, 10);
    Character hero2("Hero", 100, 20, 10);
    Character hero3("Warrior", 150, 25, 15);

    if (hero1 == hero2) std::cout << "Hero1 and Hero2 are the same!\n";
    if (!(hero1 == hero3)) std::cout << "Hero1 and Hero3 are different!\n";
    std::cout << hero1 << std::endl;

    // Weapon
    Weapon sword("Sword", 50);
    Weapon bow("Bow", 30);
    Weapon combined = sword + bow;

    std::cout << "Combined weapon: " << combined << std::endl;
    if (sword > bow) {
        std::cout << sword.getName() << " is stronger than " << bow.getName() << std::endl;
    }

    // Inventory
    Inventory inventory(5);
    inventory.addItem("Sword");
    inventory.addItem("Health Potion");
    inventory.addItem("Shield");
    inventory.addItem("Bow");
    inventory.addItem("Magic Scroll");
    inventory.addItem("Extra Item"); // лишний предмет
    inventory.displayInventory();

    // GameManager
    GameManager<Entity> manager;
    manager.addEntity(std::make_unique<Player>("Hero", 100, 1));
    manager.addEntity(std::make_unique<Enemy>("Goblin", 50, "Goblin"));
    manager.addEntity(std::make_unique<Enemy>("Orc", 80, "Brute"));

    std::cout << "\nAll game entities:\n";
    manager.displayAll();

    // Queue для чисел
    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);
    intQueue.display();
    intQueue.pop();
    intQueue.display();

    // Queue для строк
    Queue<std::string> stringQueue;
    stringQueue.push("Potion");
    stringQueue.push("Scroll");
    stringQueue.display();
    stringQueue.pop();
    stringQueue.display();

    return 0;
}
