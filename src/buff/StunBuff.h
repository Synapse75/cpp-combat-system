#pragma once
#include "Buff.h"

class StunBuff : public Buff {
public:
    StunBuff(float duration, float tickInterval);
    void OnApply(Entity& target) override;
    void OnTick(Entity& target) override;
    void OnRemove(Entity& target) override;
};