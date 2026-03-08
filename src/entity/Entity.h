#pragma once
#include <string>
#include "../core/Types.h"
#include "../skill/SkillManager.h"
#include <unordered_map>

class Entity {
public:
    Entity(int id, const std::string& name) : id_(id), name_(name) {};
    int GetId() const;
    const std::string& GetName() const;
    bool IsAlive() const;

    float GetStat(StatType type) const;
    void SetStat(StatType type, float value);
    void ModifyStat(StatType type, float delta);

    void TakeDamage(float amount);
    void Heal(float amount);

    void PrintStatus() const;

    SkillManager& GetSkillManager();
    const SkillManager& GetSkillManager() const;

private:
    int id_;
    std::string name_;
    std::unordered_map<StatType, float> stats_;
    SkillManager skillManager_;
};