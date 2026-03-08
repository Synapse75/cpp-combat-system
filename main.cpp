#include "src/combat/CombatSystem.h"
#include "src/entity/EntityFactory.h"
#include "src/event/EventBus.h"
#include "src/event/Events.h"
#include "src/data/EntityRegistry.h"
#include "src/skill/SkillManager.h"
#include <iostream>

int main() {
    // 事件系统测试

    std::cout << "[事件系统测试]" << std::endl;
    size_t damageSubId = EventBus::Instance().Subscribe<DamageEvent>(
        [](const DamageEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string attackerName = reg.GetName(e.attackerId);
            std::string defenderName = reg.GetName(e.defenderId);
            std::cout << "[回调] 伤害事件: 攻击者 " << attackerName
                      << " -> 防御者 " << defenderName
                      << ", 伤害 " << e.damage
                      << (e.isCrit ? " (暴击)" : "") << std::endl;
        }
    );

    size_t healSubId = EventBus::Instance().Subscribe<HealEvent>(
        [](const HealEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string healerName = reg.GetName(e.healerId);
            std::string targetName = reg.GetName(e.targetId);
            std::cout << "[回调] 治疗事件: 治疗者 " << healerName
                      << " -> 目标 " << targetName
                      << ", 治疗量 " << e.healAmount << std::endl;
        }
    );

    size_t deathSubId = EventBus::Instance().Subscribe<EntityDeadEvent>(
        [](const EntityDeadEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string entityName = reg.GetName(e.entityId);
            std::cout << "[回调] 实体死亡事件: 实体 " << entityName << " 死亡了！" << std::endl;
        }
    );

    size_t skillSubId = EventBus::Instance().Subscribe<SkillCastEvent>(
        [](const SkillCastEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string casterName = reg.GetName(e.casterId);
            std::string targetName = reg.GetName(e.targetId);
            std::cout << "[回调] 技能施放事件: 施法者 " << casterName
                      << " -> 目标 " << targetName
                      << ", 技能 " << e.skillName << std::endl;
        }
    );

    std::cout << "\n[战斗系统演示]" << std::endl;
    auto player = EntityFactory::CreatePlayer("Hero");
    auto enemy = EntityFactory::CreateEnemy("Goblin");

    player->PrintStatus();
    enemy->PrintStatus();

    CombatSystem::ApplyDamage(*player, *enemy, 1);
    CombatSystem::ApplyDamage(*enemy, *player, 1);
    player->GetSkillManager().GetSkill("Fireball")->Use(*player, *enemy);
    CombatSystem::ApplyDamage(*player, *enemy, 2); // 强力攻击测试

    std::cout << "\nAfter combat:\n";
    player->PrintStatus();
    enemy->PrintStatus();

    EventBus::Instance().Unsubscribe(damageSubId);
    EventBus::Instance().Unsubscribe(deathSubId);
    EventBus::Instance().Unsubscribe(skillSubId);
    return 0;
}