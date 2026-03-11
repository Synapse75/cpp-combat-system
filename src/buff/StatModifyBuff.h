#pragma once
#include "Buff.h"
#include "../core/Types.h"

class StatModifyBuff : public Buff {
public:
    StatModifyBuff(const nlohmann::json& config, StatType statType);
    void OnApply(Entity& target) override;
    void OnTick(Entity& target) override;
    void OnRemove(Entity& target) override;
private:
    StatType stat_;
    float value_;
};