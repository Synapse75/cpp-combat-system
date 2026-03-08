#pragma once
#include "Skill.h"

class Fireball : public Skill {
public:
    Fireball();
    void Execute(Entity& caster, Entity& target) override;
};