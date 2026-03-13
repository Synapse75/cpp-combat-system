#include "src/combat/CombatSystem.h"
#include "src/entity/EntityFactory.h"
#include "src/event/EventBus.h"
#include "src/event/Events.h"
#include "src/data/EntityRegistry.h"
#include "src/skill/SkillManager.h"
#include "src/data/DataLoader.h"
#include "battle/BattleManager.h"
#include <iostream>

int main() {
    DataLoader::LoadAllConfigs("src/data/config");

    size_t damageSubId = EventBus::Instance().Subscribe<DamageEvent>(
        [](const DamageEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string attackerName = reg.GetName(e.attackerId);
            std::string defenderName = reg.GetName(e.defenderId);
            std::cout << "[Damage Event] " << attackerName
                      << " -> " << defenderName
                      << ", " << e.damage
                      << (e.isCrit ? " (CRITICAL!)" : "") 
                      << std::endl;
        }
    );

    size_t healSubId = EventBus::Instance().Subscribe<HealEvent>(
        [](const HealEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string healerName = reg.GetName(e.healerId);
            std::string targetName = reg.GetName(e.targetId);
            std::cout << "[Heal Event] " << healerName
                      << " -> " << targetName
                      << ", Heal Amount: " << e.healAmount 
                      << std::endl;
        }
    );

    size_t deathSubId = EventBus::Instance().Subscribe<EntityDeadEvent>(
        [](const EntityDeadEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string entityName = reg.GetName(e.entityId);
            std::cout << "[Death Event] Entity " << entityName << " has died!" 
                      << std::endl;
        }
    );

    size_t skillSubId = EventBus::Instance().Subscribe<SkillCastEvent>(
        [](const SkillCastEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string casterName = reg.GetName(e.casterId);
            std::string targetName = reg.GetName(e.targetId);
            std::cout << "[Skill Cast Event] " << casterName
                      << " -> " << targetName
                      << ", Skill: " << e.skillName 
                      << std::endl;
        }
    );

    size_t buffApplySubId = EventBus::Instance().Subscribe<BuffApplyEvent>(
        [](const BuffApplyEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string targetName = reg.GetName(e.targetId);
            std::cout << "[Buff Apply Event] Buff: " << e.buffName
                      << " Applied to " << targetName 
                      << ", Remaining Time: " << e.duration
                      << std::endl;
            if (e.stackPolicy == StackPolicy::Stack) {
                std::cout << "[Buff Apply Event] Stack: " << e.stacks << std::endl;
            }
        }
    );

    size_t buffTickSubId = EventBus::Instance().Subscribe<BuffTickEvent>(
        [](const BuffTickEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string targetName = reg.GetName(e.targetId);
            std::cout << "[Buff Tick Event] Buff: " << e.buffName 
                      << " Ticks on " << targetName 
                      << ", Remaining Time: " << e.remainingTime
                      << std::endl;
            switch (e.effectType)
            {
            case BuffEffectType::Damage:
                std::cout << "[Buff Tick Event] Damage: " << e.amount << std::endl;
                break;
            case BuffEffectType::Heal:
                std::cout << "[Buff Tick Event] Heal: " << e.amount << std::endl;
                break;
            case BuffEffectType::StatModify:
                break;
            case BuffEffectType::Stun:
                std::cout << "[Buff Tick Event] " << targetName << " is stunned!" << std::endl;
                break;
            default:
                break;
            }
        }
    );

    size_t buffRemoveSubId = EventBus::Instance().Subscribe<BuffRemoveEvent>(
        [](const BuffRemoveEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string targetName = reg.GetName(e.targetId);
            std::cout << "[Buff Remove Event] Buff: " << e.buffName
                      << " Removed from " << targetName 
                      << std::endl;
        }
    );

    auto player = EntityFactory::CreatePlayer("Hero");
    auto enemy = EntityFactory::CreateEnemy("Goblin");

    BattleManager battle(std::move(player), std::move(enemy));
    battle.Run();

    EventBus::Instance().Unsubscribe(damageSubId);
    EventBus::Instance().Unsubscribe(deathSubId);
    EventBus::Instance().Unsubscribe(healSubId);
    EventBus::Instance().Unsubscribe(skillSubId);
    EventBus::Instance().Unsubscribe(buffApplySubId);
    EventBus::Instance().Unsubscribe(buffTickSubId);
    EventBus::Instance().Unsubscribe(buffRemoveSubId);
    return 0;
}