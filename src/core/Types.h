#pragma once
#include <cstdint>
#include <string>

enum class StatType {
    HP,
    MaxHP,
    Attack,
    Defense,
    Speed,
    CritRate,
    CritDamage
};

StatType StringToStatType(const std::string& str);

std::string StatTypeToString(StatType type);