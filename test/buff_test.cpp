#include <gtest/gtest.h>
#include "entity/Entity.h"
#include "buff/DotBuff.h"
#include "buff/DotPercentBuff.h"
#include "buff/StatModifyBuff.h"
#include "third_party/nlohmann/json.hpp"

using nlohmann::json;

TEST(BuffManager, DotBuff_TickAndExpire) {
    Entity target(1, "Target");
    target.SetStat(StatType::MaxHP, 100.0f);
    target.SetStat(StatType::HP, 100.0f);

    // Create a simple DOT: 5 damage, duration 3s, tick each 1s
    json cfg = {
        {"name", "TestDot"},
        {"type", "dot"},
        {"damage", 5.0f},
        {"duration", 3.0f},
        {"tickInterval", 1.0f},
        {"stackPolicy", "refresh"}
    };

    auto buff = std::make_unique<DotBuff>(cfg);
    target.GetBuffManager().ApplyBuff(std::move(buff), target);
    EXPECT_TRUE(target.GetBuffManager().HasBuff("TestDot"));

    // Advance 1s -> first tick (DOT doesn't deal damage on apply)
    target.GetBuffManager().UpdateAll(1.0f, target);
    EXPECT_NEAR(target.GetStat(StatType::HP), 95.0f, 1e-6f);

    // Advance 1s -> second tick
    target.GetBuffManager().UpdateAll(1.0f, target);
    EXPECT_NEAR(target.GetStat(StatType::HP), 90.0f, 1e-6f);

    // Advance remaining time to expire (two more seconds)
    target.GetBuffManager().UpdateAll(1.0f, target); // -> 85
    target.GetBuffManager().UpdateAll(1.0f, target); // -> 80 and expire
    // After expiration the buff should be removed
    EXPECT_FALSE(target.GetBuffManager().HasBuff("TestDot"));
}

TEST(BuffManager, StatModify_ApplyAndRemove) {
    Entity target(2, "Buffed");
    target.SetStat(StatType::Attack, 10.0f);

    json cfg = {
        {"name", "AtkBoost"},
        {"type", "stat_modify"},
        {"value", 0.5f},
        {"duration", 2.0f},
        {"tickInterval", 1.0f},
        {"stackPolicy", "refresh"}
    };

    auto buff = std::make_unique<StatModifyBuff>(cfg, StatType::Attack);
    target.GetBuffManager().ApplyBuff(std::move(buff), target);

    // OnApply should have modified the stat
    EXPECT_NEAR(target.GetStat(StatType::Attack), 15.0f, 1e-6f);

    // Advance until expire
    target.GetBuffManager().UpdateAll(2.0f, target);

    // OnRemove should revert the stat
    EXPECT_NEAR(target.GetStat(StatType::Attack), 10.0f, 1e-6f);
}