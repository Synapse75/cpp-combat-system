#pragma once
#include <memory>
#include "../entity/Entity.h"
#include "../skill/SkillManager.h"
#include "../buff/BuffManager.h"

class BattleManager {
public:
    BattleManager(
        std::unique_ptr<Entity> player,
        std::unique_ptr<Entity> enemy
    );

    void Run(); // 主战斗循环

private:
    void PlayerTurn();
    void EnemyTurn();                   // 简单 AI：随机选技能
    void TickBuffs(Entity& target);     // 推进所有 Buff
    void TickCooldowns(Entity& target); // 推进技能冷却
    void PrintBattleState();
    bool IsBattleOver() const;

    std::unique_ptr<Entity> player_;
    std::unique_ptr<Entity> enemy_;
    int turnCount_;
};