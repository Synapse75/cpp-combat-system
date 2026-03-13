#include "StunBuff.h"
#include "../entity/Entity.h"
#include <iostream>
StunBuff::StunBuff(const nlohmann::json& config) : Buff(config) {}

void StunBuff::OnApply(Entity& target) {
    BuffApplyEvent applyEvt{
        target.GetId(),
        name_,
        BuffEffectType::Stun,
        duration_
    };
    EventBus::Instance().Emit<BuffApplyEvent>(applyEvt);
}

void StunBuff::OnTick(Entity& target) {
    // Stun buff doesn't do anything on tick
}

void StunBuff::OnRemove(Entity& target) {
    BuffRemoveEvent removeEvt{
        target.GetId(),
        name_,
    };
    EventBus::Instance().Emit<BuffRemoveEvent>(removeEvt);
}
