#include "PoisonBuff.h"
#include "../entity/Entity.h"
#include <iostream>
PoisonBuff::PoisonBuff(float duration, float tickInterval)
    : Buff("Poison", duration, tickInterval, StackPolicy::Stack) {}

void PoisonBuff::OnApply(Entity& target) {
    std::cout << "Poison applied to " << target.GetName() << std::endl
                << "Initial damage: " << 5 * stacks_ << std::endl;
    target.TakeDamage(target.GetStat(StatType::HP) * 0.05f); // 造成当前生命值5%的伤害
}
void PoisonBuff::OnTick(Entity& target) {
    std::cout << "Poison tick on " << target.GetName() << std::endl
                << "Damage: " << 5 * stacks_ << std::endl;
    target.TakeDamage(target.GetStat(StatType::HP) * 0.05f); // 造成当前生命值5%的伤害
}
void PoisonBuff::OnRemove(Entity& target) {
    std::cout << "Poison removed from " << target.GetName() << std::endl;
}
