#include "DotBuff.h"
#include "../entity/Entity.h"
#include <iostream>

DotBuff::DotBuff(const nlohmann::json& config) : Buff(config) {
    // 读取 JSON 里的 damage
    damage = config.value("damage", 0.0f);
}

void DotBuff::OnApply(Entity& target) {
    std::cout << name_ << " applied to " << target.GetName() << std::endl
                << "Initial damage: " << damage * stacks_ << std::endl;
    target.TakeDamage(damage * stacks_);
}
void DotBuff::OnTick(Entity& target) {
    std::cout << name_ << " tick on " << target.GetName() << std::endl
                << "Damage: " << damage * stacks_ << std::endl;
    target.TakeDamage(damage * stacks_);
}
void DotBuff::OnRemove(Entity& target) {
    std::cout << name_ << " removed from " << target.GetName() << std::endl;
}
