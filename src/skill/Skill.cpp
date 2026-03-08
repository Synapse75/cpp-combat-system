#include "Skill.h"
#include "../entity/Entity.h"
#include "../combat/CombatSystem.h"
#include "../event/EventBus.h"
#include <iostream>

Skill::Skill(const std::string& name, float cooldown, float multiplier)
    : name_(name), cooldown_(cooldown), currentCooldown_(0), multiplier_(multiplier) {}

const std::string& Skill::GetName() const {
    return name_;
}
bool Skill::IsReady() const {
    return currentCooldown_ <= 0;
}
void Skill::Use(Entity& caster, Entity& target) {
    if (!IsReady()) {
        std::cout << "Skill " << name_ << " is on cooldown for " << currentCooldown_ << " seconds.\n";
        return;
    }
    Execute(caster, target);
    SkillCastEvent skillEvt{caster.GetId(), target.GetId(), name_};
    EventBus::Instance().Emit<SkillCastEvent>(skillEvt);
    currentCooldown_ = cooldown_;
}
void Skill::UpdateCooldown(float deltaTime) {
    if (currentCooldown_ > 0) {
        currentCooldown_ -= deltaTime;
        if (currentCooldown_ < 0) currentCooldown_ = 0;
    }
}