#include "AttackBoost.h"
#include "../entity/Entity.h"
#include <iostream>
AttackBoost::AttackBoost(float duration, float tickInterval)
    : Buff("Attack Boost", duration, tickInterval, StackPolicy::Refresh) {}
void AttackBoost::OnApply(Entity& target) {
    std::cout << "Attack Boost applied to " << target.GetName() << std::endl;
    std::cout << "Attack increased by 20%" << std::endl;
    target.ModifyStat(StatType::Attack, 0.2f); // 增加20%攻击力
}
void AttackBoost::OnTick(Entity& target) {
    // 这个buff不需要每tick做什么，可以留空
}
void AttackBoost::OnRemove(Entity& target) {
    std::cout << "Attack Boost removed from " << target.GetName() << std::endl;
    std::cout << "Attack returned to normal" << std::endl;
    target.ModifyStat(StatType::Attack, -0.2f); // 恢复正常攻击力
}
