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
    std::cout << "===== Battle Start! =====\n";
    while (!IsBattleOver()) {
        std::cout << "===== Turn " << ++turnCount_ << " =====\n";
        PrintBattleState();
        PlayerTurn();
        if (!IsBattleOver()) {
            EnemyTurn();
        }
    }
    std::cout << "===== Battle Over! =====\n";
}

void BattleManager::PlayerTurn() {
    std::cout << "===== Player's turn =====\n";
    std::cout << "[DEBUG] Enter PlayerTurn\n";
    TickBuffs(*player_);
    TickCooldowns(*player_);
    if (player_->GetBuffManager().HasBuff("Stun")) {
        std::cout << "[DEBUG] Player is stunned, skipping turn\n";
        return;
    }
    std::cout << "[DEBUG] Player not stunned, continuing\n";
    std::vector<Skill> skills = player_->GetSkillManager().GetSkills(); // 伪 API：列出玩家技能
    std::cout << "===== Available Skills =====\n";
    for (size_t i = 1; i < skills.size() + 1; ++i) {
        std::cout << i << ": " << skills[i - 1].GetName() << "\n"
                    << "   Type: " << skills[i - 1].GetType() << "\n"
                    << "   " << (skills[i - 1].IsReady() ? "(Ready)" : "(Cooldown: " + std::to_string(int(skills[i - 1].GetCurrentCooldown())) + ")") << "\n";
    }
    std::cout << "============================\n";
    int choice = -1;
    while (true) {
        std::cout << "Choose skill index: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input\n";
            continue;
        }
        if (choice >= 1 && choice <= (int)skills.size()) {
            if (skills[choice - 1].IsReady()) {
                break;
            } else {
                std::cout << "Skill cast failed! Cooldown: " << skills[choice - 1].GetCurrentCooldown() << "\n";
                continue;
            }
        }
        std::cout << "Out of range\n";
    }
    std::cout << "============================\n";
    if (skills[choice - 1].GetType() == "damage") {
        player_->CastSkill(skills[choice - 1].GetName(), *enemy_);
    } else {
        player_->CastSkill(skills[choice - 1].GetName(), *player_);
    }
}

void BattleManager::EnemyTurn() {
    std::cout << "===== Enemy's turn =====\n";
    TickBuffs(*enemy_);
    TickCooldowns(*enemy_);
    if (enemy_->GetBuffManager().HasBuff("Stun")) return;
    std::vector<Skill> skills = enemy_->GetSkillManager().GetSkills();
    if (skills.empty()) return;
    if (enemy_->GetStat(StatType::HP) < 30.0f) {
        // 简单 AI：如果 HP 低于 30%，优先使用治疗技能
        for (const auto& skill : skills) {
            if (skill.GetType() == "heal" && skill.IsReady()) {
                enemy_->CastSkill(skill.GetName(), *enemy_);
                return;
            }
        }
    } 
    std::vector<Skill> readyDamageSkills;
    for (const auto& skill : skills) {
        if (!skill.IsReady()) continue;
        if (skill.GetType() == "heal") continue;
        readyDamageSkills.push_back(skill);
    }

    if (readyDamageSkills.empty()) return;

    int idx = rand() % readyDamageSkills.size();
    enemy_->CastSkill(readyDamageSkills[idx].GetName(), *player_);

}

void BattleManager::TickBuffs(Entity& target) {
    target.GetBuffManager().UpdateAll(1.0f, target);
}

void BattleManager::TickCooldowns(Entity& target) {
    target.GetSkillManager().UpdateAllCooldowns(1.0f);
}

void BattleManager::PrintBattleState() {
    std::cout << player_->GetName() << " HP: " << player_->GetStat(StatType::HP) << "\n";
    std::cout << enemy_->GetName() << " HP: " << enemy_->GetStat(StatType::HP) << "\n";
}

bool BattleManager::IsBattleOver() const {
    return !player_->IsAlive() || !enemy_->IsAlive();
}