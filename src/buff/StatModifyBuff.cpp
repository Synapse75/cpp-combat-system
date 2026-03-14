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
    appliedAmount_ = target.GetStat(stat_) * value_ * stacks_;
    target.ModifyStat(stat_, appliedAmount_);
}
void StatModifyBuff::OnTick(Entity& target) {
    BuffTickEvent tickEvt{
        target.GetId(),
        name_,
        BuffEffectType::StatModify,
        appliedAmount_,
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
    // Revert the previously applied additive amount
    target.ModifyStat(stat_, -appliedAmount_);
}
