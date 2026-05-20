#ifndef USER_H
#define USER_H

#include "entity.h"
#include <memory>

class Group;

class User : public Entity {
private:
    std::weak_ptr<Group> groupLink_;

public:
    User(int id, const std::string& name, const std::string& info = "");
    ~User() override = default;

    void setGroup(std::shared_ptr<Group> group);
    std::shared_ptr<Group> getGroup() const;
    bool isInGroup() const;
    void printInfo() const override;
};

#endif