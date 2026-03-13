#include "StunBuff.h"
#include "../entity/Entity.h"
#include <iostream>
StunBuff::StunBuff(const nlohmann::json& config) : Buff(config) {}

void StunBuff::OnApply(Entity& target) {
    BuffApplyEvent applyEvt{
        target.GetId(),
        name_,
        BuffEffectType::Stun,
        duration_,
        policy_,
        stacks_
    };
    EventBus::Instance().Emit<BuffApplyEvent>(applyEvt);
}

void StunBuff::OnTick(Entity& target) {
    BuffTickEvent tickEvt{
        target.GetId(),
        name_,
        BuffEffectType::Stun,
        0.0f, // 不造成伤害
        remainingTime_
    };
    EventBus::Instance().Emit<BuffTickEvent>(tickEvt);
}

void StunBuff::OnRemove(Entity& target) {
    BuffRemoveEvent removeEvt{
        target.GetId(),
        name_,
    };
    EventBus::Instance().Emit<BuffRemoveEvent>(removeEvt);
}
