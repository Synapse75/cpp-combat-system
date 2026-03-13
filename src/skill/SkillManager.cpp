#include "SkillManager.h"
#include <iostream>

void SkillManager::AddSkill(std::unique_ptr<Skill> skill) {
    skills_.push_back(std::move(skill));
}

Skill* SkillManager::GetSkill(const std::string& name) {
    for (const auto& skill : skills_) {
        if (skill->GetName() == name) {
            return skill.get();
        }
    }
    return nullptr; // 没有找到
}

void SkillManager::UpdateAllCooldowns(float deltaTime) {
    for (const auto& skill : skills_) {
        skill->UpdateCooldown(deltaTime);
    }
}

void SkillManager::PrintSkillStatus() const {
    std::cout << "SkillStatus:\n";
    for (const auto& skill : skills_) {
        std::cout << "  " << skill->GetName() << ": "
                  << (skill->IsReady() ? "Ready" : "Cooldown: " + std::to_string(skill->GetCurrentCooldown())) << "\n";
    }
}

std::vector<Skill> SkillManager::GetSkills() const {
    std::vector<Skill> skillList;
    for (const auto& skill : skills_) {
        skillList.push_back(*skill);
    }
    return skillList;
}