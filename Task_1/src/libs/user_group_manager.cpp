#include "user_group_manager.h"
#include <iostream>
#include <algorithm>

void UserGroupManager::createUser(int userId, const std::string& username, const std::string& userInfo) {
    if (users_.find(userId) != users_.end()) {
        std::cout << "Error: User with ID " << userId << " already exists" << std::endl;
        return;
    }
    
    users_[userId] = std::make_shared<User>(userId, username, userInfo);
    std::cout << "User created: " << username << " (ID: " << userId << ")" << std::endl;
}

void UserGroupManager::deleteUser(int userId) {
    auto it = users_.find(userId);
    if (it == users_.end()) {
        std::cout << "Error: User with ID " << userId << " not found" << std::endl;
        return;
    }
    
    auto group = it->second->getGroup();
    if (group) {
        group->removeUser(it->second);
    }
    
    users_.erase(it);
    std::cout << "User with ID " << userId << " deleted successfully" << std::endl;
}

void UserGroupManager::allUsers() const {
    if (users_.empty()) {
        std::cout << "No users found" << std::endl;
        return;
    }
    
    std::cout << "All users:" << std::endl;
    for (const auto& [id, user] : users_) {
        user->printInfo();
        std::cout << "-------------------" << std::endl;
    }
}

void UserGroupManager::getUser(int userId) const {
    auto it = users_.find(userId);
    if (it == users_.end()) {
        std::cout << "Error: User with ID " << userId << " not found" << std::endl;
        return;
    }
    
    it->second->printInfo();
}

void UserGroupManager::createGroup(int groupId, const std::string& groupName, const std::string& groupInfo) {
    if (groups_.find(groupId) != groups_.end()) {
        std::cout << "Error: Group with ID " << groupId << " already exists" << std::endl;
        return;
    }
    
    groups_[groupId] = std::make_shared<Group>(groupId, groupName, groupInfo);
    std::cout << "Group created: " << groupName << " (ID: " << groupId << ")" << std::endl;
}

void UserGroupManager::deleteGroup(int groupId) {
    auto it = groups_.find(groupId);
    if (it == groups_.end()) {
        std::cout << "Error: Group with ID " << groupId << " not found" << std::endl;
        return;
    }
    
    for (auto& user : it->second->getUsers()) {
        user->setGroup(nullptr);
    }
    
    groups_.erase(it);
    std::cout << "Group with ID " << groupId << " deleted successfully" << std::endl;
}

void UserGroupManager::allGroups() const {
    if (groups_.empty()) {
        std::cout << "No groups found" << std::endl;
        return;
    }
    
    std::cout << "All groups:" << std::endl;
    for (const auto& [id, group] : groups_) {
        group->printInfo();
        std::cout << "-------------------" << std::endl;
    }
}

void UserGroupManager::getGroup(int groupId) const {
    auto it = groups_.find(groupId);
    if (it == groups_.end()) {
        std::cout << "Error: Group with ID " << groupId << " not found" << std::endl;
        return;
    }
    
    it->second->printInfo();
}

void UserGroupManager::addUserToGroup(int userId, int groupId) {
    auto userIt = users_.find(userId);
    auto groupIt = groups_.find(groupId);
    
    if (userIt == users_.end()) {
        std::cout << "Error: User with ID " << userId << " not found" << std::endl;
        return;
    }
    if (groupIt == groups_.end()) {
        std::cout << "Error: Group with ID " << groupId << " not found" << std::endl;
        return;
    }
    
    groupIt->second->addUser(userIt->second);
    std::cout << "User " << userIt->second->getName() 
              << " added to group " << groupIt->second->getName() << std::endl;
}

void UserGroupManager::removeUserFromGroup(int userId, int groupId) {
    auto userIt = users_.find(userId);
    auto groupIt = groups_.find(groupId);
    
    if (userIt == users_.end()) {
        std::cout << "Error: User with ID " << userId << " not found" << std::endl;
        return;
    }
    if (groupIt == groups_.end()) {
        std::cout << "Error: Group with ID " << groupId << " not found" << std::endl;
        return;
    }
    
    groupIt->second->removeUser(userIt->second);
    std::cout << "User " << userIt->second->getName() 
              << " removed from group " << groupIt->second->getName() << std::endl;
}