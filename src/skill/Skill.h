#pragma once
#include <string>
#include "../third_party/nlohmann/json.hpp"

class Entity; // 前向声明

class Skill {
public:
    Skill(const nlohmann::json& config);
    virtual ~Skill() = default;

    const std::string& GetName() const;
    bool IsReady() const;
    void Use(Entity& caster, Entity& target);
    void Execute(Entity& caster, Entity& target);
    void UpdateCooldown(float deltaTime); // 每 tick 调用
    float GetCurrentCooldown() const { return currentCooldown_; }
    

protected:
    std::string name_;
    std::string type_; // 伤害类型（物理/魔法）
    float cooldown_;        // 冷却时间（秒）
    float currentCooldown_; // 当前剩余冷却
    float multiplier_;      // 伤害倍率
    std::string buffName_;  // 可能附加的buff名称
    float buffPossibility_; // 附加buff的概率
    float healAmount_;     // 治疗量（如果是治疗技能）
};