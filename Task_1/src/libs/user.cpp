#include "user.h"
#include "group.h"
#include <iostream>

User::User(int id, const std::string& name, const std::string& info)
    : Entity(id, name, info) {}

void User::setGroup(std::shared_ptr<Group> group) {
    groupLink_ = group;
}

std::shared_ptr<Group> User::getGroup() const {
    return groupLink_.lock();
}

bool User::isInGroup() const {
    return !groupLink_.expired();
}

void User::printInfo() const {
    std::cout << "User: " << name_ 
              << " (ID: " << id_ 
              << ", Info: " << info_ << ")" << std::endl;
    
    auto group = groupLink_.lock();
    if (group) {
        std::cout << "  Member of group: " << group->getName() 
                  << " (ID: " << group->getId() << ")" << std::endl;
    } else {
        std::cout << "  Not a member of any group" << std::endl;
    }
}
