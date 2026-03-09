#pragma once
#include <vector>
#include <memory>
#include "Buff.h"

class Entity; // 前向声明

class BuffManager {
public:
    void ApplyBuff(std::unique_ptr<Buff> buff, Entity& target);
    void UpdateAll(float deltaTime, Entity& target);
    void RemoveExpired(Entity& target);
    bool HasBuff(const std::string& name) const;
    void PrintBuffs() const;

private:
    std::vector<std::unique_ptr<Buff>> activeBuffs_;
};