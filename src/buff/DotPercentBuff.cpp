#include "DotPercentBuff.h"
#include "../entity/Entity.h"
#include <iostream>
DotPercentBuff::DotPercentBuff(const nlohmann::json& config) : Buff(config) {
    percentDamage = config.value("percentDamage", 0.05f); // 默认造成5%当前生命值的伤害
}

void DotPercentBuff::OnApply(Entity& target) {
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
void DotPercentBuff::OnTick(Entity& target) {
    BuffTickEvent tickEvt{
        target.GetId(),
        name_,
        BuffEffectType::Damage,
        target.GetStat(StatType::HP) * percentDamage * stacks_,
        remainingTime_
    };
    EventBus::Instance().Emit<BuffTickEvent>(tickEvt);
    target.TakeDamage(target.GetStat(StatType::HP) * percentDamage * stacks_);
}
void DotPercentBuff::OnRemove(Entity& target) {
    BuffRemoveEvent removeEvt{
        target.GetId(),
        name_
    };
    EventBus::Instance().Emit<BuffRemoveEvent>(removeEvt);
}
