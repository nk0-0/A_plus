#include "group.h"
#include "user.h"
#include <iostream>
#include <algorithm>

Group::Group(int id, const std::string& name, const std::string& info)
    : Entity(id, name, info) {}

void Group::addUser(std::shared_ptr<User> user) {
    if (!user) {
        return;
    }
    auto it = std::find_if(users_.begin(), users_.end(),
        [user](const auto& u) { return u->getId() == user->getId(); });
    
    if (it != users_.end()) {
        std::cout << "User " << user->getName() << " is already in group " << name_ << std::endl;
        return;
    }
    auto currentGroup = user->getGroup();
    if (currentGroup) {
        currentGroup->removeUser(user);
    }
    
    users_.push_back(user);
    user->setGroup(shared_from_this());
}

void Group::removeUser(std::shared_ptr<User> user) {
    auto it = std::find_if(users_.begin(), users_.end(),
        [user](const auto& u) { return u->getId() == user->getId(); });
    
    if (it != users_.end()) {
        (*it)->setGroup(nullptr);
        users_.erase(it);
    }
}

bool Group::hasUser(int userId) const {
    auto it = std::find_if(users_.begin(), users_.end(),
        [userId](const auto& u) { return u->getId() == userId; });
    return it != users_.end();
}

const std::vector<std::shared_ptr<User>>& Group::getUsers() const {
    return users_;
}

size_t Group::getUserCount() const {
    return users_.size();
}

void Group::printInfo() const {
    std::cout << "Group: " << name_ 
              << " (ID: " << id_ 
              << ", Info: " << info_ << ")" << std::endl;
    std::cout << "  Members (" << users_.size() << "):" << std::endl;
    
    if (users_.empty()) {
        std::cout << "    (no members)" << std::endl;
    } else {
        for (const auto& user : users_) {
            std::cout << "    - " << user->getName() << " (ID: " << user->getId() << ")" << std::endl;
        }
    }
}