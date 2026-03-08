#include "Heal.h"
#include "../combat/CombatSystem.h"
#include <iostream>

Heal::Heal() : Skill("Heal", 3.0f, 1.0f) {}

void Heal::Execute(Entity& caster, Entity& target) {
    std::cout << caster.GetName() << " casts " << name_ << " on " << target.GetName() << "!\n";
    CombatSystem::ApplyHealing(caster, target, multiplier_);
}