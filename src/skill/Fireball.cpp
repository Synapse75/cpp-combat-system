#include "Fireball.h"
#include "../combat/CombatSystem.h"
#include <iostream>

Fireball::Fireball() : Skill("Fireball", 5.0f, 1.5f) {}

void Fireball::Execute(Entity& caster, Entity& target) {
    std::cout << caster.GetName() << " casts " << name_ << " on " << target.GetName() << "!\n";
    CombatSystem::ApplyDamage(caster, target, multiplier_);
}