#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <algorithm>
#include <stdexcept>

// ---------- Шаблонный класс Logger ----------
template<typename T>
class Logger {
private:
    std::ofstream logFile;
public:
    Logger(const std::string& filename) {
        logFile.open(filename, std::ios::app);
    }
    ~Logger() {
        if (logFile.is_open()) logFile.close();
    }
    void log(const T& entry) {
        logFile << entry << std::endl;
    }
};

// ---------- Шаблонный класс Inventory ----------
template<typename T>
class Inventory {
private:
    std::vector<T> items;
public:
    void addItem(const T& item) {
        items.push_back(item);
    }
    void removeItem(const T& item) {
        items.erase(std::remove(items.begin(), items.end(), item), items.end());
    }
    void display() const {
        std::cout << "Инвентарь:\n";
        for (const auto& item : items) {
            std::cout << "- " << item << std::endl;
        }
    }
    const std::vector<T>& getItems() const {
        return items;
    }
};

// ---------- Класс Monster (и наследники) ----------
class Character;

class Monster {
protected:
    std::string name;
    int hp;
    int attackPower;
    int defense;
public:
    Monster(std::string n, int h, int a, int d) : name(n), hp(h), attackPower(a), defense(d) {}
    virtual ~Monster() {}

    virtual void attack(Character& c);
    virtual void display() const {
        std::cout << name << " (HP: " << hp << ", ATK: " << attackPower << ", DEF: " << defense << ")\n";
    }

    void takeDamage(int dmg) {
        hp -= dmg;
        if (hp < 0) throw std::runtime_error(name + " умер!");
    }

    bool isDead() const { return hp <= 0; }
    std::string getName() const { return name; }
    int getHP() const { return hp; }
    int getAttack() const { return attackPower; }
    int getDefense() const { return defense; }
};

class Goblin : public Monster {
public:
    Goblin() : Monster("Гоблин", 30, 5, 2) {}
};

class Dragon : public Monster {
public:
    Dragon() : Monster("Дракон", 100, 20, 10) {}
};

class Skeleton : public Monster {
public:
    Skeleton() : Monster("Скелет", 40, 8, 4) {}
};

// ---------- Класс Character ----------
class Character {
private:
    std::string name;
    int hp;
    int attackPower;
    int defense;
    int level;
    int experience;
    Inventory<std::string> inventory;
    Logger<std::string> logger;

public:
    Character(std::string n)
        : name(n), hp(100), attackPower(10), defense(5), level(1), experience(0), logger("game_log.txt") {}

    void attackMonster(Monster& m) {
        int damage = std::max(0, attackPower - m.getDefense());
        logger.log(name + " атакует " + m.getName() + " на " + std::to_string(damage) + " урона.");
        try {
            m.takeDamage(damage);
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
    }

    void takeDamage(int dmg) {
        int realDmg = std::max(0, dmg - defense);
        hp -= realDmg;
        logger.log(name + " получает " + std::to_string(realDmg) + " урона.");
        if (hp <= 0) {
            throw std::runtime_error(name + " погиб!");
        }
    }

    void heal(int amount) {
        hp += amount;
        if (hp > 100) hp = 100;
        logger.log(name + " лечится на " + std::to_string(amount) + " HP.");
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {
            level++;
            experience = 0;
            logger.log(name + " повысил уровень до " + std::to_string(level));
        }
    }

    void display() const {
        std::cout << name << " (HP: " << hp << ", ATK: " << attackPower
                  << ", DEF: " << defense << ", LVL: " << level << ", EXP: " << experience << ")\n";
        inventory.display();
    }

    void addItem(const std::string& item) {
        inventory.addItem(item);
        logger.log(name + " получает предмет: " + item);
    }

    void save(const std::string& filename) {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Ошибка сохранения!");
        out << name << "\n" << hp << "\n" << attackPower << "\n" << defense << "\n"
            << level << "\n" << experience << "\n";
        const auto& items = inventory.getItems();
        out << items.size() << "\n";
        for (const auto& item : items) out << item << "\n";
    }

    void load(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("Ошибка загрузки!");
        size_t invSize;
        std::string item;
        in >> name >> hp >> attackPower >> defense >> level >> experience >> invSize;
        std::getline(in, item); // очистка строки
        for (size_t i = 0; i < invSize; ++i) {
            std::getline(in, item);
            inventory.addItem(item);
        }
    }
};

// Реализация атаки монстра на персонажа
void Monster::attack(Character& c) {
    c.takeDamage(attackPower);
}

// ---------- Класс Game ----------
class Game {
private:
    std::unique_ptr<Character> player;

public:
    void start() {
        std::string choice;
        std::cout << "1. Новая игра\n2. Загрузить игру\nВыбор: ";
        std::cin >> choice;

        if (choice == "2") {
            try {
                player = std::make_unique<Character>("Игрок");
                player->load("save.txt");
                std::cout << "Игра загружена!\n";
            } catch (const std::exception& e) {
                std::cout << "Ошибка: " << e.what() << "\nСоздание новой игры.\n";
                newGame();
            }
        } else {
            newGame();
        }

        gameLoop();
    }

    void newGame() {
        std::string name;
        std::cout << "Введите имя персонажа: ";
        std::cin >> name;
        player = std::make_unique<Character>(name);
        player->addItem("Меч");
        player->addItem("Зелье лечения");
    }

    void gameLoop() {
        bool running = true;
        while (running) {
            player->display();
            std::cout << "1. Сразиться\n2. Лечение\n3. Сохранить\n4. Выйти\nВыбор: ";
            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1:
                    fight();
                    break;
                case 2:
                    player->heal(20);
                    break;
                case 3:
                    try {
                        player->save("save.txt");
                        std::cout << "Игра сохранена!\n";
                    } catch (const std::exception& e) {
                        std::cout << e.what() << std::endl;
                    }
                    break;
                case 4:
                    running = false;
                    break;
                default:
                    std::cout << "Неверный выбор.\n";
            }
        }
    }

    void fight() {
        std::unique_ptr<Monster> monster;
        int randType = rand() % 3;
        if (randType == 0) monster = std::make_unique<Goblin>();
        else if (randType == 1) monster = std::make_unique<Dragon>();
        else monster = std::make_unique<Skeleton>();

        std::cout << "Враг появился: ";
        monster->display();

        try {
            while (!monster->isDead()) {
                player->attackMonster(*monster);
                if (!monster->isDead())
                    monster->attack(*player);
            }
            std::cout << "Монстр повержен!\n";
            player->gainExperience(50);
            player->addItem("Трофей монстра");
        } catch (const std::exception& e) {
            std::cout << "Бой завершен: " << e.what() << std::endl;
        }
    }
};

// ---------- main ----------
int main() {
    srand(static_cast<unsigned>(time(0)));
    Game game;
    game.start();
    return 0;
}
