#pragma once
#include <vector>
#include <memory>
#include "Buff.h"

class Entity; // 前向声明

class BuffManager {
public:
    BuffManager();
    void ApplyBuff(std::unique_ptr<Buff> buff, Entity& target);
    void UpdateAll(float deltaTime, Entity& target);
    void RemoveExpired();
    bool HasBuff(const std::string& name) const;
    void PrintBuffs() const;

private:
    std::vector<std::unique_ptr<Buff>> activeBuffs_;
    std::vector<std::string> pendingRemovals_;
    size_t removeSubscriptionId_ = SIZE_MAX;
    int ownerId_ = -1;
    bool inUpdate_ = false;
    void ProcessPendingRemovals();
};