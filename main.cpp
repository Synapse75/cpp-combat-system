#include "src/combat/CombatSystem.h"
#include "src/entity/EntityFactory.h"
#include "src/event/EventBus.h"
#include "src/event/Events.h"
#include "src/data/EntityRegistry.h"
#include <iostream>

int main() {
    // 事件系统测试

    std::cout << "[事件系统测试]" << std::endl;
    size_t dmgSubId = EventBus::Instance().Subscribe<DamageEvent>(
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

    size_t deathSubId = EventBus::Instance().Subscribe<EntityDeadEvent>(
        [](const EntityDeadEvent& e) {
            auto& reg = EntityRegistry::Instance();
            std::string entityName = reg.GetName(e.entityId);
            std::cout << "[回调] 实体死亡事件: 实体 " << entityName << " 死亡了！" << std::endl;
        }
    );

    std::cout << "\n[战斗系统演示]" << std::endl;
    auto player = EntityFactory::CreatePlayer("Hero");
    auto enemy = EntityFactory::CreateEnemy("Goblin");

    player->PrintStatus();
    enemy->PrintStatus();

    CombatSystem::ApplyDamage(*player, *enemy, 1);
    CombatSystem::ApplyDamage(*enemy, *player, 1);
    CombatSystem::ApplyDamage(*player, *enemy, 1);
    CombatSystem::ApplyDamage(*player, *enemy, 2); // 强力攻击测试

    std::cout << "\nAfter combat:\n";
    player->PrintStatus();
    enemy->PrintStatus();

    EventBus::Instance().Unsubscribe(dmgSubId);
    EventBus::Instance().Unsubscribe(deathSubId);
    return 0;
}