#include "StunBuff.h"
#include "../entity/Entity.h"
#include <iostream>
StunBuff::StunBuff(float duration, float tickInterval)
    : Buff("Stun", duration, tickInterval, StackPolicy::None) {}

void StunBuff::OnApply(Entity& target) {
    std::cout << "Stun applied to " << target.GetName() << std::endl;
}

void StunBuff::OnTick(Entity& target) {
    // Stun buff doesn't do anything on tick
}

void StunBuff::OnRemove(Entity& target) {
    std::cout << "Stun removed from " << target.GetName() << std::endl;
}
