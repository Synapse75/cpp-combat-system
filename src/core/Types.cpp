#include "Types.h"

#include <stdexcept>

std::string StatTypeToString(StatType type) {
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

StatType StringToStatType(const std::string& str) {
    if (str == "HP") return StatType::HP;
    if (str == "MaxHP") return StatType::MaxHP;
    if (str == "Attack") return StatType::Attack;
    if (str == "Defense") return StatType::Defense;
    if (str == "Speed") return StatType::Speed;
    if (str == "CritRate") return StatType::CritRate;
    if (str == "CritDamage") return StatType::CritDamage;
    throw std::invalid_argument("Unknown stat type: " + str);
}