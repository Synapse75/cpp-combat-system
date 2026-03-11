#pragma once
#include "Buff.h"

class StunBuff : public Buff {
public:
    StunBuff(const nlohmann::json& config);
    void OnApply(Entity& target) override;
    void OnTick(Entity& target) override;
    void OnRemove(Entity& target) override;
};