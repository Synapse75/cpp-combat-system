#pragma once
#include "Buff.h"

class PoisonBuff : public Buff {
public:
    PoisonBuff(float duration, float tickInterval);
    void OnApply(Entity& target) override;
    void OnTick(Entity& target) override;
    void OnRemove(Entity& target) override;
};