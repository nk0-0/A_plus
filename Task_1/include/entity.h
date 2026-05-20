#ifndef ENTITY_H
#define ENTITY_H

#include <string>

// Базовый абстрактный класс
class Entity {
protected:
    std::string name_;
    int id_;
    std::string info_;

public:
    Entity(int id, const std::string& name, const std::string& info = "");
    virtual ~Entity() = default;

    int getId() const;
    std::string getName() const;
    std::string getInfo() const;

    void setName(const std::string& name);
    void setInfo(const std::string& info);
    
    virtual void printInfo() const = 0;
};

#endif