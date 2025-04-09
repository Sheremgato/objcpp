#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // Имя персонажа
    int health;        // Уровень здоровья
    int attack;        // Уровень атаки
    int defense;       // Уровень защиты

public:
    // Конструктор для инициализации данных
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Метод для получения уровня здоровья
    int getHealth() const {
        return health;
    }

    // Метод для вывода информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Метод для атаки другого персонажа
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.takeDamage(damage);  // Используем метод takeDamage для нанесения урона
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        } else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

    // Метод для восстановления здоровья
    void heal(int amount) {
        health += amount;
        if (health > 100) {
            health = 100;
        }
        std::cout << name << " heals for " << amount << " points. Current HP: " << health << std::endl;
    }

    // Новый метод для уменьшения уровня здоровья
    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0;
        }
        std::cout << name << " takes " << amount << " damage. Current HP: " << health << std::endl;
    }
};

int main() {
    // Создаем объекты персонажей
    Character hero("Hero", 80, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // Выводим информацию о персонажах
    hero.displayInfo();
    monster.displayInfo();

    // Герой атакует монстра (вызов метода attackEnemy, который использует takeDamage)
    hero.attackEnemy(monster);
    monster.displayInfo();

    // Пример прямого вызова метода takeDamage для героя
    hero.takeDamage(30);
    hero.displayInfo();

    return 0;
}
