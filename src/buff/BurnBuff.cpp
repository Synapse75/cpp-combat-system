#include "BurnBuff.h"
#include "../entity/Entity.h"
#include <iostream>
BurnBuff::BurnBuff(float duration, float tickInterval)
    : Buff("Burn", duration, tickInterval, StackPolicy::Stack) {}

void BurnBuff::OnApply(Entity& target) {
    std::cout << "Burn applied to " << target.GetName() << std::endl
                << "Initial damage: " << 5 * stacks_ << std::endl;
    target.TakeDamage(5 * stacks_);
}
void BurnBuff::OnTick(Entity& target) {
    std::cout << "Burn tick on " << target.GetName() << std::endl
                << "Damage: " << 5 * stacks_ << std::endl;
    target.TakeDamage(5 * stacks_);
}
void BurnBuff::OnRemove(Entity& target) {
    std::cout << "Burn removed from " << target.GetName() << std::endl;
}
