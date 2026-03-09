#include "Fireball.h"
#include "../combat/CombatSystem.h"
#include <iostream>
#include <random>
#include "../buff/BurnBuff.h"

static std::mt19937 rng(std::random_device{}());

Fireball::Fireball() : Skill("Fireball", 5.0f, 1.5f) {}

void Fireball::Execute(Entity& caster, Entity& target) {
    std::cout << caster.GetName() << " casts " << name_ << " on " << target.GetName() << "!\n";
    CombatSystem::ApplyDamage(caster, target, multiplier_);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    if (dis(gen) < 1.0f) { // 30% 概率触发额外效果
        target.GetBuffManager().ApplyBuff(std::make_unique<BurnBuff>(5.0f, 1.0f), target);
    }
}