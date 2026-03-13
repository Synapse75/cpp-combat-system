#include <gtest/gtest.h>
#include "entity/Entity.h"
#include "skill/Skill.h"
#include "third_party/nlohmann/json.hpp"
#include "event/EventBus.h"
#include "event/Events.h"

using nlohmann::json;

TEST(Skill, UseAndCooldownAndEvent) {
    Entity attacker(1, "A");
    Entity defender(2, "B");

    attacker.SetStat(StatType::Attack, 50.0f);
    attacker.SetStat(StatType::CritRate, 0.0f);
    attacker.SetStat(StatType::CritDamage, 1.0f);

    defender.SetStat(StatType::MaxHP, 200.0f);
    defender.SetStat(StatType::HP, 200.0f);
    defender.SetStat(StatType::Defense, 10.0f);

    // Create damage skill with 2s cooldown
    json sj = {
        {"name", "TestStrike"},
        {"type", "damage"},
        {"multiplier", 1.0f},
        {"cooldown", 2.0f}
    };

    auto skill = std::make_unique<Skill>(sj);
    attacker.GetSkillManager().AddSkill(std::move(skill));

    bool skillCastEventSeen = false;
    EventBus::Instance().Subscribe<SkillCastEvent>([&](const SkillCastEvent& e){
        skillCastEventSeen = true;
        EXPECT_EQ(e.casterId, attacker.GetId());
        EXPECT_EQ(e.targetId, defender.GetId());
        EXPECT_EQ(e.skillName, "TestStrike");
    });

    // Cast skill via Entity helper
    attacker.CastSkill("TestStrike", defender);

    // Damage applied: raw = atk*1 - def*0.5 = 50 - 5 = 45 => defender HP drops by 45
    EXPECT_NEAR(defender.GetStat(StatType::HP), 155.0f, 1e-3f);

    // Skill should be on cooldown now
    Skill* s = attacker.GetSkillManager().GetSkill("TestStrike");
    ASSERT_NE(s, nullptr);
    EXPECT_GT(s->GetCurrentCooldown(), 0.0f);

    // Advance cooldown
    attacker.GetSkillManager().UpdateAllCooldowns(2.0f);
    EXPECT_TRUE(s->IsReady());
    EXPECT_TRUE(skillCastEventSeen);
}