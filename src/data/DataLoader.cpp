#include "DataLoader.h"
#include "../buff/DotBuff.h"
#include "../buff/StatModifyBuff.h"
#include "../buff/DotPercentBuff.h"
#include "../buff/StunBuff.h"
#include "../third_party/nlohmann/json.hpp"
#include <fstream>

// DataLoader.cpp
std::unique_ptr<Skill> DataLoader::LoadSkill(const std::string& name) {
    if (skillConfigs.find(name) == skillConfigs.end()) {
        return nullptr; 
    }
    
    const nlohmann::json& config = skillConfigs[name];

    return std::make_unique<Skill>(config);
}

std::unique_ptr<Buff> DataLoader::LoadBuff(const std::string& name) {
    if (buffConfigs.find(name) == buffConfigs.end()) {
        return nullptr; 
    }
    
    const nlohmann::json& config = buffConfigs[name];
    std::string type = config.value("type", "unknown");

    if (type == "stun") {
        return std::make_unique<StunBuff>(config);
    } else if (type == "dot") {
        return std::make_unique<DotBuff>(config);
    } else if (type == "dot_percent") {
        return std::make_unique<DotPercentBuff>(config);
    } else if (type == "stat_modify") {
        std::string statStr = config.value("stat", "Attack");
        StatType statType = StringToStatType(statStr);
        return std::make_unique<StatModifyBuff>(config, statType);
    }
    
    return nullptr; 
}

void DataLoader::LoadAllConfigs(const std::string& configDir) {
    LoadSkillConfig(configDir + "/skills.json");
    LoadBuffConfig(configDir + "/buffs.json");
}

void DataLoader::LoadSkillConfig(const std::string& path) {
    // 读取 JSON 文件，填充 skillConfigs
    std::ifstream in(path);
    nlohmann::json j;
    in >> j;
    for (const auto& skill : j["skills"]) {
        skillConfigs[skill["name"]] = skill;
    }
}

void DataLoader::LoadBuffConfig(const std::string& path) {
    // 读取 JSON 文件，填充 buffConfigs
    std::ifstream in(path);
    nlohmann::json j;
    in >> j;
    for (const auto& buff : j["buffs"]) {
        buffConfigs[buff["name"]] = buff;
    }
}

std::unordered_map<std::string, nlohmann::json> DataLoader::skillConfigs;
std::unordered_map<std::string, nlohmann::json> DataLoader::buffConfigs;