#include "src/entity/EntityFactory.h"
#include <iostream>

int main() {
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