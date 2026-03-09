#pragma once
#include "Buff.h"

class AttackBoost : public Buff {
public:
    AttackBoost(float duration, float tickInterval);
    void OnApply(Entity& target) override;
    void OnTick(Entity& target) override;
    void OnRemove(Entity& target) override;
};