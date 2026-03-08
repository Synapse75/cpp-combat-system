#pragma once
#include <vector>
#include <memory>
#include "Skill.h"

class SkillManager {
public:
    void AddSkill(std::unique_ptr<Skill> skill);
    Skill* GetSkill(const std::string& name);
    void UpdateAllCooldowns(float deltaTime);
    void PrintSkillStatus() const;

private:
    std::vector<std::unique_ptr<Skill>> skills_;
};