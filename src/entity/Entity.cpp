#include "Entity.h"
#include <iostream>

static const char* ToString(StatType type) {
    switch (type) {
        case StatType::HP: return "HP";
        case StatType::MaxHP: return "MaxHP";
        case StatType::Attack: return "Attack";
        case StatType::Defense: return "Defense";
        case StatType::Speed: return "Speed";
        case StatType::CritRate: return "CritRate";
        case StatType::CritDamage: return "CritDamage";
        default: return "Unknown";
    }
}

int Entity::GetId() const {
    return id_;
}
const std::string& Entity::GetName() const {
    return name_;
}
bool Entity::IsAlive() const {
    return GetStat(StatType::HP) > 0;
}
float Entity::GetStat(StatType type) const {
    auto it = stats_.find(type);
    return it != stats_.end() ? it->second : 0.0f;
}
void Entity::SetStat(StatType type, float value) {
    stats_[type] = value;
}
void Entity::ModifyStat(StatType type, float delta) {
    stats_[type] += delta;
    if (stats_[type] < 0) stats_[type] = 0; // 防止负数
}
void Entity::TakeDamage(float amount) {
    ModifyStat(StatType::HP, -amount);
}
void Entity::Heal(float amount) {
    ModifyStat(StatType::HP, amount);
}
void Entity::PrintStatus() const {
    std::cout << "Entity: " << name_ << " (ID: " << id_ << ")\n";
    for (const auto& stat : stats_) {
        std::cout << "  " << ToString(stat.first) << ": " << stat.second << "\n";
    }
}
