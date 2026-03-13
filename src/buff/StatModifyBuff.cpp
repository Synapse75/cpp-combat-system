#include "StatModifyBuff.h"
#include "../entity/Entity.h"
#include <iostream>
#include "../data/DataLoader.h"
StatModifyBuff::StatModifyBuff(const nlohmann::json& config, StatType statType) : Buff(config), stat_(statType) {
    stat_ = statType;
    value_ = config.value("value", 0.2f);
}
void StatModifyBuff::OnApply(Entity& target) {
    BuffApplyEvent applyEvt{
        target.GetId(),
        name_,
        BuffEffectType::StatModify,
        duration_,
        policy_,
        stacks_
    };
    EventBus::Instance().Emit<BuffApplyEvent>(applyEvt);
    target.ModifyStat(stat_, value_);
}
void StatModifyBuff::OnTick(Entity& target) {
    BuffTickEvent tickEvt{
        target.GetId(),
        name_,
        BuffEffectType::StatModify,
        value_ * stacks_,
        remainingTime_
    };
    EventBus::Instance().Emit<BuffTickEvent>(tickEvt);
}

void StatModifyBuff::OnRemove(Entity& target) {
    BuffRemoveEvent removeEvt{
        target.GetId(),
        name_
    };
    EventBus::Instance().Emit<BuffRemoveEvent>(removeEvt);
    target.ModifyStat(stat_, -value_);
}
