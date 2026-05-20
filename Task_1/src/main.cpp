#include <iostream>
#include <string>
#include <sstream>
#include "user_group_manager.h"

void printHelp() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  createUser <userId> <username> [info] - Create a new user" << std::endl;
    std::cout << "  deleteUser <userId> - Delete a user" << std::endl;
    std::cout << "  allUsers - Show all users" << std::endl;
    std::cout << "  getUser <userId> - Show user information" << std::endl;
    std::cout << "  createGroup <groupId> <groupName> [info] - Create a new group" << std::endl;
    std::cout << "  deleteGroup <groupId> - Delete a group" << std::endl;
    std::cout << "  allGroups - Show all groups with their members" << std::endl;
    std::cout << "  getGroup <groupId> - Show group information with members" << std::endl;
    std::cout << "  addUserToGroup <userId> <groupId> - Add user to group" << std::endl;
    std::cout << "  removeUserFromGroup <userId> <groupId> - Remove user from group" << std::endl;
    std::cout << "  help - Show this help" << std::endl;
    std::cout << "  exit - Exit the program" << std::endl;
}

void processCommand(const std::string& command, UserGroupManager& manager) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "createUser") {
        int userId;
        std::string username, info;
        if (iss >> userId >> username) {
            std::getline(iss, info);
            if (!info.empty() && info[0] == ' ') info = info.substr(1);
            manager.createUser(userId, username, info);
        } else {
            std::cout << "Error: Invalid arguments. Usage: createUser <userId> <username> [info]" << std::endl;
        }
    }
    else if (cmd == "deleteUser") {
        int userId;
        if (iss >> userId) manager.deleteUser(userId);
        else std::cout << "Error: Invalid arguments. Usage: deleteUser <userId>" << std::endl;
    }
    else if (cmd == "allUsers") manager.allUsers();
    else if (cmd == "getUser") {
        int userId;
        if (iss >> userId) manager.getUser(userId);
        else std::cout << "Error: Invalid arguments. Usage: getUser <userId>" << std::endl;
    }
    else if (cmd == "createGroup") {
        int groupId;
        std::string groupName, info;
        if (iss >> groupId >> groupName) {
            std::getline(iss, info);
            if (!info.empty() && info[0] == ' ') info = info.substr(1);
            manager.createGroup(groupId, groupName, info);
        } else {
            std::cout << "Error: Invalid arguments. Usage: createGroup <groupId> <groupName> [info]" << std::endl;
        }
    }
    else if (cmd == "deleteGroup") {
        int groupId;
        if (iss >> groupId) manager.deleteGroup(groupId);
        else std::cout << "Error: Invalid arguments. Usage: deleteGroup <groupId>" << std::endl;
    }
    else if (cmd == "allGroups") manager.allGroups();
    else if (cmd == "getGroup") {
        int groupId;
        if (iss >> groupId) manager.getGroup(groupId);
        else std::cout << "Error: Invalid arguments. Usage: getGroup <groupId>" << std::endl;
    }
    else if (cmd == "addUserToGroup") {
        int userId, groupId;
        if (iss >> userId >> groupId) {
            manager.addUserToGroup(userId, groupId);
        } else {
            std::cout << "Error: Usage: addUserToGroup <userId> <groupId>" << std::endl;
        }
    }
    else if (cmd == "removeUserFromGroup") {
        int userId, groupId;
        if (iss >> userId >> groupId) {
            manager.removeUserFromGroup(userId, groupId);
        } else {
            std::cout << "Error: Usage: removeUserFromGroup <userId> <groupId>" << std::endl;
        }
    }
    else if (cmd == "help") printHelp();
    else if (cmd == "exit") {
        std::cout << "Finished!" << std::endl;
        exit(0);
    }
    else if (!cmd.empty()) std::cout << "Error: Unknown command. Type 'help' for available commands." << std::endl;
}

int main() {
    UserGroupManager manager;
    std::string command;

    std::cout << "User and Group Management System" << std::endl;
    std::cout << "Type 'help' for available commands" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        processCommand(command, manager);
    }
    return 0;
}
