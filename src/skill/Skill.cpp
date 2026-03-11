#include "Skill.h"
#include "../entity/Entity.h"
#include "../combat/CombatSystem.h"
#include "../event/EventBus.h"
#include "../data/DataLoader.h"
#include <iostream>

Skill::Skill(const nlohmann::json& config) {
    // 如果 key 不存在，就使用第二个参数作为默认值
    name_        = config.value("name", "Unknown Skill");
    type_        = config.value("type", "damage");
    cooldown_    = config.value("cooldown", 0.0f);
    multiplier_  = config.value("multiplier", 1.0f);
    healAmount_  = config.value("healAmount", 0.0f);
    buffName_    = config.value("applyBuff", ""); 
    buffPossibility_ = config.value("buffPossibility", 0.0f);

    currentCooldown_ = 0.0f;
}

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
void Skill::Execute(Entity& caster, Entity& target) {
    std::cout << caster.GetName() << " casts " << name_ << " on " << target.GetName() << "!\n";
    if (type_ == "damage") {
        CombatSystem::ApplyDamage(caster, target, multiplier_);
    } else if (type_ == "heal") {
        CombatSystem::ApplyHealing(caster, target, healAmount_);
    }
    if (!buffName_.empty() && (rand() / float(RAND_MAX)) < buffPossibility_) {
        auto buff = DataLoader::LoadBuff(buffName_);
        if (buff) {
            target.GetBuffManager().ApplyBuff(std::move(buff), target);
        }
    }
}
void Skill::UpdateCooldown(float deltaTime) {
    if (currentCooldown_ > 0) {
        currentCooldown_ -= deltaTime;
        if (currentCooldown_ < 0) currentCooldown_ = 0;
    }
}