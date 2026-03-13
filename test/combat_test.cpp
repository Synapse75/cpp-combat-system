#include <gtest/gtest.h>
#include "combat/CombatSystem.h"
#include "entity/Entity.h"

TEST(Smoke, Works) {
    EXPECT_EQ(1 + 1, 2);
}

TEST(CombatSystem, CalculateDamage_NoCrit_Deterministic) {
    Entity attacker(1, "Attacker");
    Entity defender(2, "Defender");

    attacker.SetStat(StatType::Attack, 50.0f);
    attacker.SetStat(StatType::CritRate, 0.0f);   // 确保不会暴击
    attacker.SetStat(StatType::CritDamage, 2.0f); // 有无影响均可

    defender.SetStat(StatType::Defense, 20.0f);

    auto res = CombatSystem::CalculateDamage(attacker, defender, 1.0f);

    // 按公式：raw = atk*1.0 - def*0.5 = 50 - 10 = 40 -> no crit -> final = 40
    EXPECT_NEAR(res.finalDamage, 40.0f, 1e-3f);
}