#pragma once
#include <memory>
#include "Entity.h"

class EntityFactory {
public:
    static std::unique_ptr<Entity> CreatePlayer(const std::string& name);
    static std::unique_ptr<Entity> CreateEnemy(const std::string& name);
};