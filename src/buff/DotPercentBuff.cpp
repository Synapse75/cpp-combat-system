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
        duration_
    };
    EventBus::Instance().Emit<BuffApplyEvent>(applyEvt);
    target.TakeDamage(target.GetStat(StatType::HP) * percentDamage); // 造成当前生命值5%的伤害
}
void DotPercentBuff::OnTick(Entity& target) {
    BuffTickEvent tickEvt{
        target.GetId(),
        name_,
        BuffEffectType::Damage,
        target.GetStat(StatType::HP) * percentDamage,
        remainingTime_
    };
    EventBus::Instance().Emit<BuffTickEvent>(tickEvt);
    target.TakeDamage(target.GetStat(StatType::HP) * percentDamage); // 造成当前生命值5%的伤害
}
void DotPercentBuff::OnRemove(Entity& target) {
    BuffRemoveEvent removeEvt{
        target.GetId(),
        name_
    };
    EventBus::Instance().Emit<BuffRemoveEvent>(removeEvt);
}
