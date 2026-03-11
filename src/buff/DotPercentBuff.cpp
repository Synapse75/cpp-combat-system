#include "DotPercentBuff.h"
#include "../entity/Entity.h"
#include <iostream>
DotPercentBuff::DotPercentBuff(const nlohmann::json& config) : Buff(config) {
    percentDamage = config.value("percentDamage", 0.05f); // 默认造成5%当前生命值的伤害
}

void DotPercentBuff::OnApply(Entity& target) {
    std::cout << name_ << " applied to " << target.GetName() << std::endl
                << "Initial damage: " << percentDamage * stacks_ << std::endl;
    target.TakeDamage(target.GetStat(StatType::HP) * percentDamage); // 造成当前生命值5%的伤害
}
void DotPercentBuff::OnTick(Entity& target) {
    std::cout << name_ << " tick on " << target.GetName() << std::endl
                << "Damage: " << percentDamage * stacks_ << std::endl;
    target.TakeDamage(target.GetStat(StatType::HP) * percentDamage); // 造成当前生命值5%的伤害
}
void DotPercentBuff::OnRemove(Entity& target) {
    std::cout << name_ << " removed from " << target.GetName() << std::endl;
}
