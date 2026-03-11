#include "StunBuff.h"
#include "../entity/Entity.h"
#include <iostream>
StunBuff::StunBuff(const nlohmann::json& config) : Buff(config) {}

void StunBuff::OnApply(Entity& target) {
    std::cout << name_ << " applied to " << target.GetName() << std::endl;
}

void StunBuff::OnTick(Entity& target) {
    // Stun buff doesn't do anything on tick
}

void StunBuff::OnRemove(Entity& target) {
    std::cout << name_ << " removed from " << target.GetName() << std::endl;
}
