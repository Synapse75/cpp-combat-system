#include "EntityFactory.h"
#include "../data/EntityRegistry.h"
#include "../data/DataLoader.h"


std::unique_ptr<Entity> EntityFactory::CreatePlayer(const std::string& name) {
    auto player = std::make_unique<Entity>(1, name);
    player->SetStat(StatType::HP, 100.0f);
    player->SetStat(StatType::Attack, 20.0f);
    player->SetStat(StatType::Defense, 10.0f);
    player->SetStat(StatType::CritDamage, 1.5f);
    player->SetStat(StatType::CritRate, 0.1f);
    player->GetSkillManager().AddSkill(DataLoader::LoadSkill("Fireball"));
    player->GetSkillManager().AddSkill(DataLoader::LoadSkill("Heal"));
    EntityRegistry::Instance().Register(player->GetId(), name);
    return player;
}

std::unique_ptr<Entity> EntityFactory::CreateEnemy(const std::string& name) {
    auto enemy = std::make_unique<Entity>(2, name);
    enemy->SetStat(StatType::HP, 80.0f);
    enemy->SetStat(StatType::Attack, 15.0f);
    enemy->SetStat(StatType::Defense, 5.0f);
    enemy->SetStat(StatType::CritDamage, 1.0f);
    enemy->SetStat(StatType::CritRate, 0.0f);
    enemy->GetSkillManager().AddSkill(DataLoader::LoadSkill("Fireball"));
    enemy->GetSkillManager().AddSkill(DataLoader::LoadSkill("Heal"));
    EntityRegistry::Instance().Register(enemy->GetId(), name);
    return enemy;
}