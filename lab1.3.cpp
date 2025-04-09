#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

// === Базовый класс ===
class Entity {
protected:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    virtual void attack(Entity& target) {
        int damage = attack - target.defense;
        if (damage > 0) {
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Виртуальный метод лечения
    virtual void heal(int amount) {
        std::cout << name << " tries to heal, but nothing happens.\n";
    }

    virtual ~Entity() {}
};

// === Персонаж ===
class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            if (rand() % 100 < 20) {
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Character: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Переопределение метода лечения
    void heal(int amount) override {
        health += amount;
        std::cout << name << " heals for " << amount << " HP!\n";
    }
};

// === Монстр ===
class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            if (rand() % 100 < 30) {
                damage += 5;
                std::cout << "Poisonous attack! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

// === Босс ===
class Boss : public Monster {
public:
    Boss(const std::string& n, int h, int a, int d)
        : Monster(n, h, a, d) {}

    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (rand() % 100 < 40) {
            damage += 15;  // Огненный урон
            std::cout << "Flaming strike! ";
        }
        if (damage > 0) {
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Boss: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

// === Основная функция ===
int main() {
    srand(static_cast<unsigned>(time(0)));

    Character hero("Knight", 120, 25, 15);
    Monster orc("Orc", 60, 18, 6);
    Boss demonLord("Demon Lord", 200, 35, 25);

    Entity* entities[] = { &hero, &orc, &demonLord };

    std::cout << "=== Entity Info ===" << std::endl;
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    std::cout << "\n=== Battle Phase ===" << std::endl;
    hero.attack(orc);
    orc.attack(hero);
    demonLord.attack(hero);

    std::cout << "\n=== Healing Phase ===" << std::endl;
    hero.heal(30);           // Лечение работает
    orc.heal(20);            // Лечение не работает (будет сообщение по умолчанию)

    std::cout << "\n=== Updated Info ===" << std::endl;
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    return 0;
}
