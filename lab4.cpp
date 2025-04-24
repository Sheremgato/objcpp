#include <iostream>
#include <string>
#include <memory>

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
        std::string newName = name + "-" + other.name;
        int newDamage = damage + other.damage;
        return Weapon(newName, newDamage);
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

// Производный класс Player
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

// Производный класс Enemy
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

int main() {
    // Character сравнение и вывод
    Character hero1("Hero", 100, 20, 10);
    Character hero2("Hero", 100, 20, 10);
    Character hero3("Warrior", 150, 25, 15);

    if (hero1 == hero2) {
        std::cout << "Hero1 and Hero2 are the same!\n";
    }
    if (!(hero1 == hero3)) {
        std::cout << "Hero1 and Hero3 are different!\n";
    }

    std::cout << hero1 << std::endl;

    // Weapon перегрузки
    Weapon sword("Sword", 50);
    Weapon bow("Bow", 30);
    Weapon combined = sword + bow;

    std::cout << "Combined weapon: " << combined << std::endl;

    if (sword > bow) {
        std::cout << sword.getName() << " is stronger than " << bow.getName() << std::endl;
    }

    // Inventory
    Inventory playerInventory(5);
    playerInventory.addItem("Sword");
    playerInventory.addItem("Health Potion");
    playerInventory.addItem("Shield");
    playerInventory.addItem("Bow");
    playerInventory.addItem("Magic Scroll");
    playerInventory.addItem("Extra Item"); // будет проигнорирован

    playerInventory.displayInventory();

    // Entity (полиморфизм и умные указатели)
    std::unique_ptr<Entity> entities[] = {
        std::make_unique<Player>("Hero", 100, 1),
        std::make_unique<Enemy>("Goblin", 50, "Beast"),
        std::make_unique<Enemy>("Orc", 80, "Brute")
    };

    std::cout << "\nEntities:\n";
    for (const auto& entity : entities) {
        entity->displayInfo();
    }

    return 0;
}
