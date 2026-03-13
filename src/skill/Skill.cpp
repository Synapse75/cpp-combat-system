#include "Skill.h"
#include "../entity/Entity.h"
#include "../combat/CombatSystem.h"
#include "../event/EventBus.h"
#include "../data/DataLoader.h"
#include <iostream>
#include <random>

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
    if (IsReady()) {
        SkillCastEvent skillEvt{caster.GetId(), target.GetId(), name_};
        EventBus::Instance().Emit<SkillCastEvent>(skillEvt);
        Execute(caster, target);
        currentCooldown_ = cooldown_;
    }
}
void Skill::Execute(Entity& caster, Entity& target) {
    if (type_ == "damage") {
        CombatSystem::ApplyDamage(caster, target, multiplier_);
    } else if (type_ == "heal") {
        CombatSystem::ApplyHealing(caster, target, healAmount_);
    }
    if (!buffName_.empty()) {
        static thread_local std::mt19937_64 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        if (dist(rng) < buffPossibility_) {
            auto buff = DataLoader::LoadBuff(buffName_);
            if (buff) target.GetBuffManager().ApplyBuff(std::move(buff), target);
        }
    }
}
void Skill::UpdateCooldown(float deltaTime) {
    if (currentCooldown_ > 0) {
        currentCooldown_ -= deltaTime;
        if (currentCooldown_ < 0) currentCooldown_ = 0;
    }
}