#include "src/combat/CombatSystem.h"
#include "src/entity/EntityFactory.h"
#include "src/event/EventBus.h"
#include "src/event/Events.h"
#include "src/data/EntityRegistry.h"
#include "src/skill/SkillManager.h"
#include <iostream>

int main() {
    size_t damageSubId = EventBus::Instance().Subscribe<DamageEvent>(
        [](const DamageEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string attackerName = reg.GetName(e.attackerId);
            std::string defenderName = reg.GetName(e.defenderId);
            std::cout << "[Callback] DamageEvent: " << attackerName
                      << " -> " << defenderName
                      << ", " << e.damage
                      << (e.isCrit ? " (CRITICAL!)" : "") << std::endl;
        }
    );

    size_t healSubId = EventBus::Instance().Subscribe<HealEvent>(
        [](const HealEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string healerName = reg.GetName(e.healerId);
            std::string targetName = reg.GetName(e.targetId);
            std::cout << "[Callback] HealEvent: " << healerName
                      << " -> " << targetName
                      << ", Heal Amount: " << e.healAmount << std::endl;
        }
    );

    size_t deathSubId = EventBus::Instance().Subscribe<EntityDeadEvent>(
        [](const EntityDeadEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string entityName = reg.GetName(e.entityId);
            std::cout << "[Callback] EntityDeadEvent: Entity " << entityName << " has died!" << std::endl;
        }
    );

    size_t skillSubId = EventBus::Instance().Subscribe<SkillCastEvent>(
        [](const SkillCastEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string casterName = reg.GetName(e.casterId);
            std::string targetName = reg.GetName(e.targetId);
            std::cout << "[Callback] SkillCastEvent: Caster " << casterName
                      << " -> Target " << targetName
                      << ", Skill " << e.skillName << std::endl;
        }
    );

    auto player = EntityFactory::CreatePlayer("Hero");
    auto enemy = EntityFactory::CreateEnemy("Goblin");

    player->PrintStatus();
    enemy->PrintStatus();

    player->CastSkill("Fireball", *enemy);
    enemy->CastSkill("Fireball", *player);
    player->CastSkill("Heal", *player);

    std::cout << "\nAfter combat:\n";
    player->PrintStatus();
    enemy->PrintStatus();

    EventBus::Instance().Unsubscribe(damageSubId);
    EventBus::Instance().Unsubscribe(deathSubId);
    EventBus::Instance().Unsubscribe(skillSubId);
    return 0;
}