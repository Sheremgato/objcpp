#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Character " << name << " created!\n";
    }

    // Деструктор
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    // Деструктор
    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Weapon {
private:
    std::string name;
    int damage;
    double weight;

public:
    // Конструктор
    Weapon(const std::string& n, int d, double w)
        : name(n), damage(d), weight(w) {
        std::cout << "Weapon " << name << " created!\n";
    }

    // Деструктор
    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    // Метод для вывода информации об оружии
    void displayInfo() const {
        std::cout << "Weapon: " << name << ", Damage: " << damage
                  << ", Weight: " << weight << " kg" << std::endl;
    }
};

int main() {
    // Создание персонажа, монстра и оружия
    Character hero("Arthur", 100, 15, 10);
    Monster goblin("Goblin", 50, 5, 3);
    Weapon sword("Sword", 50, 3.5);
    Weapon axe("Axe", 70, 5.0);

    // Вывод информации
    hero.displayInfo();
    goblin.displayInfo();
    sword.displayInfo();
    axe.displayInfo();

    return 0;
}
