
#include "src/entity/EntityFactory.h"
#include "src/event/EventBus.h"
#include "src/event/Events.h"
#include <iostream>

int main() {
    // 事件系统测试
    std::cout << "[事件系统测试]" << std::endl;
    size_t dmgSubId = EventBus::Instance().Subscribe<DamageEvent>(
        [](const DamageEvent& e) {
            std::cout << "[回调] 伤害事件: 攻击者 " << e.attackerId
                      << " -> 防御者 " << e.defenderId
                      << ", 伤害 " << e.damage
                      << (e.isCrit ? " (暴击)" : "") << std::endl;
        }
    );

    DamageEvent evt{1, 2, 42.0f, true};
    std::cout << "Emit DamageEvent..." << std::endl;
    EventBus::Instance().Emit<DamageEvent>(evt);

    EventBus::Instance().Unsubscribe(dmgSubId);
    std::cout << "Emit DamageEvent after unsubscribe..." << std::endl;
    EventBus::Instance().Emit<DamageEvent>(evt);

    std::cout << "\n[战斗系统演示]" << std::endl;
    auto player = EntityFactory::CreatePlayer("Hero");
    auto enemy = EntityFactory::CreateEnemy("Goblin");

    player->PrintStatus();
    enemy->PrintStatus();

    player->TakeDamage(30);
    enemy->TakeDamage(50);

    std::cout << "\nAfter combat:\n";
    player->PrintStatus();
    enemy->PrintStatus();

    return 0;
}