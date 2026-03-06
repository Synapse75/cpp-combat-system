#pragma once
#include <string>

struct DamageEvent {
    int attackerId; // ID of the entity causing damage
    int defenderId; // ID of the entity receiving damage
    float damage; // Amount of damage dealt
    bool isCrit;
};

struct SkillCastEvent {
    int casterId; // ID of the entity casting the skill
    int targetId; // ID of the target entity (could be -1 for self-targeted skills)
    std::string skillName; // Name of the skill being cast
};

struct EntityDeadEvent {
    int entityId; // ID of the entity that died
};

struct BuffAppliedEvent {
    int entityId; // ID of the entity applying the buff
    int targetId; // ID of the entity receiving the buff
    std::string buffName; // Name of the buff being applied
};
