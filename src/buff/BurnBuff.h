#pragma once
#include "Buff.h"

class BurnBuff : public Buff {
public:
    BurnBuff(float duration, float tickInterval);
    void OnApply(Entity& target) override;
    void OnTick(Entity& target) override;
    void OnRemove(Entity& target) override;
};