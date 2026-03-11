#pragma once
#include "Buff.h"

class DotPercentBuff : public Buff {
public:
    DotPercentBuff(const nlohmann::json& config);
    void OnApply(Entity& target) override;
    void OnTick(Entity& target) override;
    void OnRemove(Entity& target) override;
private:
    float percentDamage;
};