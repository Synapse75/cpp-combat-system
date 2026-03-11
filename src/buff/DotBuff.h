#pragma once
#include "Buff.h"

class DotBuff : public Buff {
public:
    DotBuff(const nlohmann::json& config);
    void OnApply(Entity& target) override;
    void OnTick(Entity& target) override;
    void OnRemove(Entity& target) override;
private:
    float damage;
};