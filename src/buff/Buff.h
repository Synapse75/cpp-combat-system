#pragma once
#include <string>
#include "../third_party/nlohmann/json.hpp"
#include "../event/Events.h"
#include "../event/EventBus.h"

class Entity; // 前向声明

class Buff {
public:
    Buff(const nlohmann::json& config);
    virtual ~Buff() = default;

    const std::string& GetName() const;
    bool IsExpired() const;
    int GetStacks() const;

    virtual void OnApply(Entity& target) = 0;   // 首次施加
    virtual void OnTick(Entity& target) = 0;    // 每次 tick
    virtual void OnRemove(Entity& target) = 0;  // 到期移除

    void Update(float deltaTime, Entity& target); // 推进时间，触发 tick
    void AddStack();
    void RefreshDuration();
    StackPolicy GetStackPolicy() const { return policy_; }
    float GetRemainingTime() const { return remainingTime_; }

protected:
    std::string name_;
    std::string type_;
    float duration_;
    float remainingTime_;
    float tickInterval_;
    float tickTimer_;
    int stacks_;
    StackPolicy policy_;
};