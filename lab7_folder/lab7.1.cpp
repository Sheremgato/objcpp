#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

// Базовый класс для всех сущностей
class Entity {
public:
    virtual ~Entity() = default;
    virtual void display() const = 0;
    virtual std::string getType() const = 0;
    virtual void save(std::ofstream& file) const = 0;
    virtual void load(std::ifstream& file) = 0;
};

// Класс игрока
class Player : public Entity {
private:
    std::string name;
    int health;
    int level;

public:
    Player(const std::string& name, int health, int level)
        : name(name), health(health), level(level) {}

    void display() const override {
        std::cout << "Player: " << name << ", Health: " << health << ", Level: " << level << std::endl;
    }

    std::string getType() const override {
        return "Player";
    }

    void save(std::ofstream& file) const override {
        file << getType() << '\n' << name << '\n' << health << '\n' << level << '\n';
    }

    void load(std::ifstream& file) override {
        file >> name >> health >> level;
        file.ignore();  // Чистим остатки строки после числа
    }
};

// Менеджер для управления сущностями
template <typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    void addEntity(T entity) {
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->display();
        }
    }

    std::vector<T>& getEntities() {
        return entities;
    }
};

// Функция для сохранения данных в файл
void saveToFile(const GameManager<Entity*>& manager, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing.");
    }

    for (const auto& entity : manager.getEntities()) {
        entity->save(file);
    }
}

// Функция для загрузки данных из файла
void loadFromFile(GameManager<Entity*>& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading.");
    }

    while (file) {
        std::string type;
        std::getline(file, type);
        if (type == "Player") {
            Player* player = new Player("", 0, 0);
            player->load(file);
            manager.addEntity(player);
        }
        // Можно добавить сюда другие типы сущностей, например, врагов и т.д.
    }
}

int main() {
    try {
        GameManager<Entity*> manager;
        
        // Добавление нескольких персонажей
        manager.addEntity(new Player("Hero", 100, 1));
        manager.addEntity(new Player("Mage", 80, 2));
        manager.addEntity(new Player("Warrior", 120, 3));

        // Сохранение в файл
        saveToFile(manager, "game_save.txt");

        // Загрузка из файла
        GameManager<Entity*> loadedManager;
        loadFromFile(loadedManager, "game_save.txt");

        // Вывод загруженных сущностей
        loadedManager.displayAll();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
