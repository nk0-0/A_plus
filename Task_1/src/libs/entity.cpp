#include "entity.h"
#include <iostream>

Entity::Entity(int id, const std::string& name, const std::string& info)
    : id_(id), name_(name), info_(info) {}

int Entity::getId() const {
    return id_;
}

std::string Entity::getName() const {
    return name_;
}

std::string Entity::getInfo() const {
    return info_;
}

void Entity::setName(const std::string& name) {
    name_ = name;
}

void Entity::setInfo(const std::string& info) {
    info_ = info;
}