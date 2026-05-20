#ifndef USER_GROUP_MANAGER_H
#define USER_GROUP_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include "user.h"
#include "group.h"

class UserGroupManager {
private:
    std::map<int, std::shared_ptr<User>> users_;
    std::map<int, std::shared_ptr<Group>> groups_;

public:
    void createUser(int userId, const std::string& username, const std::string& userInfo = "");
    void deleteUser(int userId);
    void allUsers() const;
    void getUser(int userId) const;
    void createGroup(int groupId, const std::string& groupName, const std::string& groupInfo = "");
    void deleteGroup(int groupId);
    void allGroups() const;
    void getGroup(int groupId) const;
    void addUserToGroup(int userId, int groupId);
    void removeUserFromGroup(int userId, int groupId);
};

#endif