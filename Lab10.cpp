#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdexcept>

// ------------------------ User Base Class ------------------------
class User {
protected:
    std::string name;
    int id;
    int accessLevel;

public:
    User(std::string name, int id, int accessLevel) {
        if (name.empty()) throw std::invalid_argument("Имя не может быть пустым.");
        if (accessLevel < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным.");
        this->name = name;
        this->id = id;
        this->accessLevel = accessLevel;
    }

    virtual ~User() = default;

    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    virtual void displayInfo() const {
        std::cout << "Имя: " << name << ", ID: " << id << ", Уровень доступа: " << accessLevel << "\n";
    }

    virtual std::string serialize() const {
        return name + "," + std::to_string(id) + "," + std::to_string(accessLevel);
    }
};

// ------------------------ Derived User Types ------------------------
class Student : public User {
    std::string group;
public:
    Student(std::string name, int id, int accessLevel, std::string group)
        : User(name, id, accessLevel), group(group) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << "Группа: " << group << "\n";
    }

    std::string serialize() const override {
        return "Student," + User::serialize() + "," + group;
    }
};

class Teacher : public User {
    std::string department;
public:
    Teacher(std::string name, int id, int accessLevel, std::string department)
        : User(name, id, accessLevel), department(department) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << "Кафедра: " << department << "\n";
    }

    std::string serialize() const override {
        return "Teacher," + User::serialize() + "," + department;
    }
};

class Administrator : public User {
    std::string role;
public:
    Administrator(std::string name, int id, int accessLevel, std::string role)
        : User(name, id, accessLevel), role(role) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << "Роль: " << role << "\n";
    }

    std::string serialize() const override {
        return "Administrator," + User::serialize() + "," + role;
    }
};

// ------------------------ Resource Class ------------------------
class Resource {
    std::string name;
    int requiredAccessLevel;
public:
    Resource(std::string name, int requiredAccessLevel)
        : name(name), requiredAccessLevel(requiredAccessLevel) {}

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    void display() const {
        std::cout << "Ресурс: " << name << ", Требуемый уровень доступа: " << requiredAccessLevel << "\n";
    }

    std::string serialize() const {
        return name + "," + std::to_string(requiredAccessLevel);
    }

    static Resource deserialize(const std::string& line) {
        size_t delim = line.find(',');
        return Resource(line.substr(0, delim), std::stoi(line.substr(delim + 1)));
    }
};

// ------------------------ AccessControlSystem Template ------------------------
template<typename U, typename R>
class AccessControlSystem {
    std::vector<std::shared_ptr<U>> users;
    std::vector<R> resources;

public:
    void addUser(std::shared_ptr<U> user) {
        users.push_back(user);
    }

    void addResource(const R& resource) {
        resources.push_back(resource);
    }

    void showAccess() {
        for (const auto& user : users) {
            user->displayInfo();
            for (const auto& resource : resources) {
                std::cout << "  -> ";
                resource.display();
                std::cout << (resource.checkAccess(*user) ? "    Доступ разрешён\n" : "    Доступ запрещён\n");
            }
        }
    }

    void saveUsersToFile(const std::string& filename) {
        std::ofstream file(filename);
        for (const auto& user : users)
            file << user->serialize() << "\n";
    }

    void saveResourcesToFile(const std::string& filename) {
        std::ofstream file(filename);
        for (const auto& res : resources)
            file << res.serialize() << "\n";
    }

    void loadResourcesFromFile(const std::string& filename) {
        resources.clear();
        std::ifstream file(filename);
        std::string line;
        while (getline(file, line)) {
            resources.push_back(Resource::deserialize(line));
        }
    }

    void findUserByName(const std::string& searchName) const {
        for (const auto& user : users) {
            if (user->getName() == searchName) {
                user->displayInfo();
                return;
            }
        }
        std::cout << "Пользователь не найден.\n";
    }

    void sortUsersByAccessLevel() {
        std::sort(users.begin(), users.end(), [](const std::shared_ptr<U>& a, const std::shared_ptr<U>& b) {
            return a->getAccessLevel() < b->getAccessLevel();
        });
    }
};

// ------------------------ Main ------------------------
int main() {
    try {
        AccessControlSystem<User, Resource> system;

        system.addUser(std::make_shared<Student>("Иван", 1, 1, "Группа А"));
        system.addUser(std::make_shared<Teacher>("Мария", 2, 3, "Математика"));
        system.addUser(std::make_shared<Administrator>("Алексей", 3, 5, "ИТ отдел"));

        system.addResource(Resource("Библиотека", 1));
        system.addResource(Resource("Лаборатория", 3));
        system.addResource(Resource("Серверная", 5));

        std::cout << "=== Доступ ===\n";
        system.showAccess();

        system.saveUsersToFile("users.txt");
        system.saveResourcesToFile("resources.txt");

        std::cout << "\n=== Поиск пользователя ===\n";
        system.findUserByName("Мария");

        std::cout << "\n=== Сортировка по уровню доступа ===\n";
        system.sortUsersByAccessLevel();
        system.showAccess();

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}
