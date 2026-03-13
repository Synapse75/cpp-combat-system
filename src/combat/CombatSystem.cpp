#include "CombatSystem.h"
#include "../event/EventBus.h"
#include <random>

DamageResult CombatSystem::CalculateDamage(
    const Entity& attacker,
    const Entity& defender,
    float skillMultiplier
) {
    float atk = attacker.GetStat(StatType::Attack);
    float def = defender.GetStat(StatType::Defense);
    float critRate = attacker.GetStat(StatType::CritRate);
    float critDamage = attacker.GetStat(StatType::CritDamage);
    bool isCrit = RollCrit(critRate);

    float critBonus = isCrit ? critDamage : 1.0f; // 暴击加成
    float rawDamage = (atk * skillMultiplier - def * 0.5f);
    if (rawDamage < 0) rawDamage = 0; // 最小伤害为0
    float finalDamage = rawDamage * critBonus;

    return {rawDamage, finalDamage, isCrit};
}

void CombatSystem::ApplyDamage(
    Entity& attacker,
    Entity& defender,
    float skillMultiplier
) {
    DamageResult result = CalculateDamage(attacker, defender, skillMultiplier);
    defender.TakeDamage(result.finalDamage);

    // 发出伤害事件
    DamageEvent damageEvt{
        attacker.GetId(),
        defender.GetId(),
        result.finalDamage,
        result.isCrit
    };
    EventBus::Instance().Emit<DamageEvent>(damageEvt);
    if (!defender.IsAlive()) {
        EntityDeadEvent deathEvt{defender.GetId()};
        EventBus::Instance().Emit<EntityDeadEvent>(deathEvt);
    }
}

void CombatSystem::ApplyHealing(
    Entity& healer,
    Entity& target,
    float healAmount
) {
    target.Heal(healAmount);

    // 发出治疗事件
    HealEvent healEvt{
        healer.GetId(),
        target.GetId(),
        healAmount
    };
    EventBus::Instance().Emit<HealEvent>(healEvt);
}

bool CombatSystem::RollCrit(float critRate) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(rng) < critRate;
}