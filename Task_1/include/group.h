#ifndef GROUP_H
#define GROUP_H

#include "entity.h"
#include <memory>
#include <vector>

class User;

class Group : public Entity, public std::enable_shared_from_this<Group> {
private:
    std::vector<std::shared_ptr<User>> users_;

public:
    Group(int id, const std::string& name, const std::string& info = "");
    ~Group() override = default;

    void addUser(std::shared_ptr<User> user);
    void removeUser(std::shared_ptr<User> user);
    bool hasUser(int userId) const;
    const std::vector<std::shared_ptr<User>>& getUsers() const;
    size_t getUserCount() const;
    
    void printInfo() const override;
};

#endif