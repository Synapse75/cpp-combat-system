#pragma once
#include "Skill.h"

class Heal : public Skill {
public:
    Heal();
    void Execute(Entity& caster, Entity& target) override;
};