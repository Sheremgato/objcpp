#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>

// Класс для монстра
class Monster {
public:
    std::string name;
    int health;
    int attack;
    int defense;

    Monster(const std::string& name, int health, int attack, int defense)
        : name(name), health(health), attack(attack), defense(defense) {}

    void displayInfo() const {
        std::cout << "Monster: " << name
                  << ", Health: " << health
                  << ", Attack: " << attack
                  << ", Defense: " << defense << std::endl;
    }

    bool isAlive() const {
        return health > 0;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }
};

// Класс для персонажа
class Character {
public:
    std::string name;
    int health;
    int attack;
    int defense;

    Character(const std::string& name, int health, int attack, int defense)
        : name(name), health(health), attack(attack), defense(defense) {}

    void displayInfo() const {
        std::cout << "Character: " << name
                  << ", Health: " << health
                  << ", Attack: " << attack
                  << ", Defense: " << defense << std::endl;
    }

    bool isAlive() const {
        return health > 0;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }
};

// Глобальные объекты для персонажа и монстра
Character hero("Hero", 100, 20, 10);
Monster monster("Goblin", 80, 15, 5);

// Мьютекс для синхронизации доступа к общим данным
std::mutex battleMutex;

// Функция, которая имитирует бой между персонажем и монстром
void battle() {
    while (hero.isAlive() && monster.isAlive()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Бой (персонаж атакует монстра, затем монстр атакует персонажа)
        {
            std::lock_guard<std::mutex> lock(battleMutex);
            // Персонаж атакует монстра
            int damageToMonster = hero.attack - monster.defense;
            if (damageToMonster > 0) {
                monster.takeDamage(damageToMonster);
                std::cout << hero.name << " attacks " << monster.name << " for " << damageToMonster << " damage!\n";
            } else {
                std::cout << hero.name << " attacks " << monster.name << " but it's ineffective!\n";
            }

            // Монстр атакует персонажа
            int damageToHero = monster.attack - hero.defense;
            if (damageToHero > 0) {
                hero.takeDamage(damageToHero);
                std::cout << monster.name << " attacks " << hero.name << " for " << damageToHero << " damage!\n";
            } else {
                std::cout << monster.name << " attacks " << hero.name << " but it's ineffective!\n";
            }
        }
        
        // Вывод текущих состояний
        {
            std::lock_guard<std::mutex> lock(battleMutex);
            hero.displayInfo();
            monster.displayInfo();
        }
    }

    // Завершение боя
    if (hero.isAlive()) {
        std::cout << hero.name << " wins the battle!\n";
    } else {
        std::cout << monster.name << " wins the battle!\n";
    }
}

int main() {
    // Запуск потока для боя
    std::thread battleThread(battle);

    // Ожидаем завершения потока боя
    battleThread.join();

    return 0;
}
