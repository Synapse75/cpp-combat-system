#pragma once
#include "../entity/Entity.h"

struct DamageResult {
    float rawDamage;
    float finalDamage;
    bool isCrit;
};

class CombatSystem {
public:
    // 伤害公式：(atk * multiplier - def * 0.5) * (1 + critBonus)
    static DamageResult CalculateDamage(
        const Entity& attacker,
        const Entity& defender,
        float skillMultiplier = 1.0f
    );

    // 执行伤害（计算 + 扣血 + 发事件）
    static void ApplyDamage(
        Entity& attacker,
        Entity& defender,
        float skillMultiplier = 1.0f
    );

    // 执行治疗（计算 + 加血 + 发事件）
    static void ApplyHealing(
        Entity& healer,
        Entity& target,
        float skillMultiplier = 1.0f
    );

private:
    // 使用 mt19937 随机数引擎判断暴击
    static bool RollCrit(float critRate);
};