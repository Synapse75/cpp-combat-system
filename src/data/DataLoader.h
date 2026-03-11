#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "../skill/Skill.h"
#include "../buff/Buff.h"
#include "../core/Types.h"

class DataLoader {
public:
    static std::unique_ptr<Skill> LoadSkill(const std::string& name);
    static std::unique_ptr<Buff> LoadBuff(const std::string& name);
    static void LoadAllConfigs(const std::string& configDir);

private:
    static void LoadSkillConfig(const std::string& path);
    static void LoadBuffConfig(const std::string& path);
    static std::unordered_map<std::string, nlohmann::json> skillConfigs;
    static std::unordered_map<std::string, nlohmann::json> buffConfigs;
};