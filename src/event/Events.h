#pragma once
#include <string>

enum class BuffEffectType { Damage, Heal, StatModify, Stun, None };

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

struct BuffApplyEvent {
    int targetId; // ID of the entity receiving the buff
    std::string buffName; // Name of the buff being applied
    BuffEffectType effectType; // Type of effect this buff applies (damage, heal, stat modify, etc.)
    float duration; // Duration of the buff
};

struct BuffTickEvent {
    int targetId; // ID of the entity affected by the buff tick
    std::string buffName; // Name of the buff ticking
    BuffEffectType effectType; // Type of effect this tick applies (damage, heal, etc.)
    float amount; // Amount of damage/heal/effect applied this tick
    float remainingTime; // Remaining time of the buff after this tick
};

struct BuffRemoveEvent {
    int targetId; // ID of the entity losing the buff
    std::string buffName; // Name of the buff being removed
};

struct HealEvent {
    int healerId; // ID of the entity healing
    int targetId; // ID of the entity being healed
    float healAmount; // Amount of health restored
};
