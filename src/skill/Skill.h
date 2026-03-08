#pragma once
#include <string>

class Entity; // 前向声明

class Skill {
public:
    Skill(const std::string& name, float cooldown, float multiplier);
    virtual ~Skill() = default;

    const std::string& GetName() const;
    bool IsReady() const;
    void Use(Entity& caster, Entity& target);
    void UpdateCooldown(float deltaTime); // 每 tick 调用
    float GetCurrentCooldown() const { return currentCooldown_; }

    virtual void Execute(Entity& caster, Entity& target) = 0; // 子类实现具体效果

protected:
    std::string name_;
    float cooldown_;        // 冷却时间（秒）
    float currentCooldown_; // 当前剩余冷却
    float multiplier_;      // 伤害倍率
};