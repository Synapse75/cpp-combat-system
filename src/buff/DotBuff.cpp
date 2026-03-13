#include "DotBuff.h"
#include "../entity/Entity.h"
#include <iostream>

DotBuff::DotBuff(const nlohmann::json& config) : Buff(config) {
    // 读取 JSON 里的 damage
    damage = config.value("damage", 0.0f);
}

void DotBuff::OnApply(Entity& target) {
    BuffApplyEvent applyEvt{
        target.GetId(),
        name_,
        BuffEffectType::Damage,
        duration_,
        policy_,
        stacks_
    };
    EventBus::Instance().Emit<BuffApplyEvent>(applyEvt);
}
void DotBuff::OnTick(Entity& target) {
    BuffTickEvent tickEvt{
        target.GetId(),
        name_,
        BuffEffectType::Damage,
        damage * stacks_,
        remainingTime_
    };
    EventBus::Instance().Emit<BuffTickEvent>(tickEvt);
    target.TakeDamage(damage * stacks_);
}
void DotBuff::OnRemove(Entity& target) {
    BuffRemoveEvent removeEvt{
        target.GetId(),
        name_
    };
    EventBus::Instance().Emit<BuffRemoveEvent>(removeEvt);
}
