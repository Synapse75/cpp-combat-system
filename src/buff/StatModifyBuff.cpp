#include "StatModifyBuff.h"
#include "../entity/Entity.h"
#include <iostream>
#include "../data/DataLoader.h"
StatModifyBuff::StatModifyBuff(const nlohmann::json& config, StatType statType) : Buff(config), stat_(statType) {
    stat_ = statType;
    value_ = config.value("value", 0.2f); // 默认增加20%
}
void StatModifyBuff::OnApply(Entity& target) {
    std::cout << name_ << " applied to " << target.GetName() << std::endl;
    std::cout << StatTypeToString(stat_) << " increased by " << value_ * 100 << "%" << std::endl;
    target.ModifyStat(stat_, value_); // 增加20%攻击力
}
void StatModifyBuff::OnTick(Entity& target) {
    // 这个buff不需要每tick做什么，可以留空
}
void StatModifyBuff::OnRemove(Entity& target) {
    std::cout << name_ << " removed from " << target.GetName() << std::endl;
    std::cout << StatTypeToString(stat_) << " returned to normal" << std::endl;
    target.ModifyStat(stat_, -value_); // 恢复正常攻击力
}
