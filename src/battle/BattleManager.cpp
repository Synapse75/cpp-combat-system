#include "BattleManager.h"
#include <iostream>
#include "../buff/BuffManager.h"
#include "../skill/SkillManager.h"
#include "../entity/Entity.h"
#include <limits>

BattleManager::BattleManager(
    std::unique_ptr<Entity> player,
    std::unique_ptr<Entity> enemy
) : player_(std::move(player)), enemy_(std::move(enemy)), turnCount_(0) {
}

void BattleManager::Run() {
    while (!IsBattleOver()) {
        PlayerTurn();
        if (!IsBattleOver()) {
            EnemyTurn();
        }
    }
}

void BattleManager::PlayerTurn() {
    std::cout << "Player's turn:\n";
    PrintBattleState();
    TickBuffs(*player_);
    TickCooldowns();

    std::vector<Skill> skills = player_->GetSkillManager().GetSkills(); // 伪 API：列出玩家技能
    for (size_t i = 0; i < skills.size(); ++i) std::cout << i << ": " << skills[i].GetName() << "\n";
    int choice = -1;
    while (true) {
        std::cout << "Choose skill index: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input\n";
            continue;
        }
        if (choice >= 0 && choice < (int)skills.size()) break;
        std::cout << "Out of range\n";
    }
    Entity* target = &*enemy_; // 根据技能类型选择目标（自体/敌方）
    player_->CastSkill(skills[choice].GetName(), *target);
}

void BattleManager::EnemyTurn() {
    std::cout << "Enemy's turn:\n";
    PrintBattleState();
    TickBuffs(*enemy_);
    TickCooldowns();
    
    std::vector<Skill> skills = enemy_->GetSkillManager().GetSkills(); // 伪 API：列出敌人技能
    if (skills.empty()) return;
    if (enemy_->GetStat(StatType::HP) < 30) {
        // 简单 AI：如果 HP 低于 30%，优先使用治疗技能
        for (const auto& skill : skills) {
            if (skill.GetType() == "heal" && skill.IsReady()) {
                enemy_->CastSkill(skill.GetName(), *enemy_);
                return;
            }
        }
    } else {
        // 否则随机使用攻击技能
        std::vector<Skill> readySkills;
        for (const auto& skill : skills) {
            if (skill.IsReady()) {
                readySkills.push_back(skill);
            }
        }
        if (!readySkills.empty()) {
            int idx = rand() % readySkills.size();
            enemy_->CastSkill(readySkills[idx].GetName(), *player_);
        }
    }

}

void BattleManager::TickBuffs(Entity& target) {
    player_->GetBuffManager().UpdateAll(1.0f, target);
    enemy_->GetBuffManager().UpdateAll(1.0f, target);
}

void BattleManager::TickCooldowns() {
    player_->GetSkillManager().UpdateAllCooldowns(1.0f);
    enemy_->GetSkillManager().UpdateAllCooldowns(1.0f);
}

void BattleManager::PrintBattleState() {
    std::cout << player_->GetName() << " HP: " << player_->GetStat(StatType::HP) << "\n";
    std::cout << enemy_->GetName() << " HP: " << enemy_->GetStat(StatType::HP) << "\n";
}

bool BattleManager::IsBattleOver() const {
    return !player_->IsAlive() || !enemy_->IsAlive();
}