# C++ Modular Combat System — 开发路线图

## 项目目标

使用 C++ 从零实现一个可扩展的游戏战斗系统框架，通过模拟场景验证技能、Buff、事件系统等核心机制。

输出形式：控制台模拟战斗（不需要图形界面，核心是系统设计）

---

## 最终目录结构

```
cpp-combat-system/
├── CMakeLists.txt
├── main.cpp
├── README.md
├── ROADMAP.md
│
├── src/
│   ├── core/            # 类型定义、前置声明、通用工具
│   │   ├── Types.h
│   │   └── Common.h
│   │
│   ├── entity/          # 实体系统 + 属性系统
│   │   ├── Entity.h
│   │   ├── Entity.cpp
│   │   ├── EntityFactory.h
│   │   └── EntityFactory.cpp
│   │
│   ├── event/           # 模板化事件总线
│   │   ├── EventBus.h
│   │   └── EventBus.cpp
│   │
│   ├── combat/          # 战斗计算
│   │   ├── CombatSystem.h
│   │   └── CombatSystem.cpp
│   │
│   ├── skill/           # 技能系统
│   │   ├── Skill.h
│   │   ├── Fireball.h
│   │   ├── Fireball.cpp
│   │   ├── Heal.h
│   │   ├── Heal.cpp
│   │   ├── DashAttack.h
│   │   ├── DashAttack.cpp
│   │   ├── SkillManager.h
│   │   └── SkillManager.cpp
│   │
│   ├── buff/            # Buff / 状态系统
│   │   ├── Buff.h
│   │   ├── BurnBuff.h
│   │   ├── BurnBuff.cpp
│   │   ├── PoisonBuff.h
│   │   ├── PoisonBuff.cpp
│   │   ├── AttackBoostBuff.h
│   │   ├── AttackBoostBuff.cpp
│   │   ├── StunBuff.h
│   │   ├── StunBuff.cpp
│   │   ├── BuffManager.h
│   │   └── BuffManager.cpp
│   │
│   ├── battle/          # 战斗流程控制
│   │   ├── BattleManager.h
│   │   └── BattleManager.cpp
│   │
│   └── data/            # 数据驱动（JSON 加载）
│       ├── DataLoader.h
│       ├── DataLoader.cpp
│       └── config/
│           ├── skills.json
│           └── buffs.json
│
├── test/                # 单元测试 (GoogleTest)
│   ├── CMakeLists.txt
│   ├── combat_test.cpp
│   ├── buff_test.cpp
│   ├── event_test.cpp
│   └── skill_test.cpp
│
└── third_party/         # 第三方库
    ├── nlohmann/        # nlohmann/json (header-only)
    └── googletest/      # GoogleTest
```

---

## 阶段 1：基础框架搭建（Day 1-2）

### 目标
能创建角色并打印属性。

### 任务清单

- [ ] 创建项目目录结构
- [ ] 编写 `CMakeLists.txt`（用 CMake 构建，面试加分项）
- [ ] 实现 `core/Types.h`：定义 `StatType` 枚举

```cpp
// core/Types.h
#pragma once
#include <cstdint>

enum class StatType {
    HP,
    MaxHP,
    Attack,
    Defense,
    Speed,
    CritRate,    // 0.0 ~ 1.0
    CritDamage   // 倍率，如 1.5
};
```

- [ ] 实现 `Entity` 类：id、名称、属性系统

```cpp
// entity/Entity.h
#pragma once
#include <string>
#include <unordered_map>
#include "../core/Types.h"

class Entity {
public:
    Entity(int id, const std::string& name);

    int GetId() const;
    const std::string& GetName() const;
    bool IsAlive() const;

    // 属性系统：用 map 存储，Buff 可以优雅地修改
    float GetStat(StatType type) const;
    void SetStat(StatType type, float value);
    void ModifyStat(StatType type, float delta);

    void TakeDamage(float amount);
    void Heal(float amount);

    void PrintStatus() const;

private:
    int id_;
    std::string name_;
    std::unordered_map<StatType, float> stats_;
};
```

- [ ] 实现 `EntityFactory`：快速创建预设角色

```cpp
// entity/EntityFactory.h
#pragma once
#include <memory>
#include "Entity.h"

class EntityFactory {
public:
    static std::unique_ptr<Entity> CreatePlayer(const std::string& name);
    static std::unique_ptr<Entity> CreateEnemy(const std::string& name);
};
```

- [ ] 在 `main.cpp` 中测试：创建玩家和敌人，打印属性

### 验收标准

```
[Player] Warrior — HP: 200/200, ATK: 50, DEF: 30
[Enemy]  Goblin  — HP: 100/100, ATK: 25, DEF: 10
```

---

## 阶段 2：事件系统（Day 3-5）

### 目标
实现类型安全的事件总线，这是所有模块解耦的基础。

### 任务清单

- [ ] 定义事件结构体

```cpp
// event/Events.h
#pragma once
#include <string>

struct DamageEvent {
    int attackerId;
    int defenderId;
    float damage;
    bool isCrit;
};

struct SkillCastEvent {
    int casterId;
    int targetId;
    std::string skillName;
};

struct EntityDeadEvent {
    int entityId;
    std::string entityName;
};

struct BuffAppliedEvent {
    int entityId;
    std::string buffName;
    float duration;
};
```

- [ ] 实现**模板化 EventBus**（关键亮点！）

```cpp
// event/EventBus.h
#pragma once
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <any>
#include <algorithm>

class EventBus {
public:
    // 订阅：EventBus::Subscribe<DamageEvent>([](const DamageEvent& e) { ... });
    template<typename EventType>
    size_t Subscribe(std::function<void(const EventType&)> callback) {
        auto key = std::type_index(typeid(EventType));
        size_t id = nextId_++;
        handlers_[key].push_back({id, [callback](const std::any& event) {
            callback(std::any_cast<const EventType&>(event));
        }});
        return id; // 返回 id 用于取消订阅
    }

    // 发布：EventBus::Emit<DamageEvent>(damageEvent);
    template<typename EventType>
    void Emit(const EventType& event) {
        auto key = std::type_index(typeid(EventType));
        auto it = handlers_.find(key);
        if (it != handlers_.end()) {
            for (auto& handler : it->second) {
                handler.callback(event);
            }
        }
    }

    // 取消订阅
    void Unsubscribe(size_t handlerId);

    // 全局单例（简化使用）
    static EventBus& Instance();

private:
    struct Handler {
        size_t id;
        std::function<void(const std::any&)> callback;
    };

    std::unordered_map<std::type_index, std::vector<Handler>> handlers_;
    size_t nextId_ = 0;
};
```

- [ ] 编写测试验证：订阅 → 发布 → 收到回调 → 取消订阅 → 不再收到

### 验收标准

```
[Test] Subscribe DamageEvent... OK
[Test] Emit DamageEvent... callback triggered, damage=30 OK
[Test] Unsubscribe... OK
[Test] Emit again... no callback OK
```

---

## 阶段 3：战斗计算（Day 6-8）

### 目标
两个 Entity 能互相造成伤害，伤害事件通过 EventBus 广播。

### 任务清单

- [ ] 实现 `CombatSystem`

```cpp
// combat/CombatSystem.h
#pragma once
#include "../entity/Entity.h"

struct DamageResult {
    float rawDamage;
    float finalDamage;
    bool isCrit;
};

class CombatSystem {
public:
    // 伤害公式：(atk * multiplier - def * 0.5) * (1 + critBonus)
    static DamageResult CalculateDamage(
        const Entity& attacker,
        const Entity& defender,
        float skillMultiplier = 1.0f
    );

    // 执行伤害（计算 + 扣血 + 发事件）
    static void ApplyDamage(
        Entity& attacker,
        Entity& defender,
        float skillMultiplier = 1.0f
    );

private:
    // 使用 mt19937 随机数引擎判断暴击
    static bool RollCrit(float critRate);
};
```

- [ ] 伤害公式实现

```cpp
DamageResult CombatSystem::CalculateDamage(
    const Entity& attacker, const Entity& defender, float skillMultiplier)
{
    float atk = attacker.GetStat(StatType::Attack);
    float def = defender.GetStat(StatType::Defense);
    float critRate = attacker.GetStat(StatType::CritRate);
    float critDmg = attacker.GetStat(StatType::CritDamage);

    float raw = atk * skillMultiplier - def * 0.5f;
    if (raw < 1.0f) raw = 1.0f; // 最低伤害保底

    bool crit = RollCrit(critRate);
    float final_ = crit ? raw * critDmg : raw;

    return { raw, final_, crit };
}
```

- [ ] `ApplyDamage` 中 emit `DamageEvent`，如果目标死亡 emit `EntityDeadEvent`
- [ ] 在 main 中测试两个角色互殴

### 验收标准

```
Warrior attacks Goblin → 35 damage
Goblin attacks Warrior → 12 damage
Warrior attacks Goblin → 52 damage (CRIT!)
Goblin is defeated!
```

---

## 阶段 4：技能系统（Day 9-13）

### 目标
角色能释放不同技能，有冷却限制。

### 任务清单

- [ ] 实现 `Skill` 基类

```cpp
// skill/Skill.h
#pragma once
#include <string>
#include "../entity/Entity.h"

class Skill {
public:
    Skill(const std::string& name, float cooldown, float multiplier);
    virtual ~Skill() = default;

    const std::string& GetName() const;
    bool IsReady() const;
    void Use(Entity& caster, Entity& target);
    void UpdateCooldown(float deltaTime); // 每 tick 调用

    virtual void Execute(Entity& caster, Entity& target) = 0; // 子类实现具体效果

protected:
    std::string name_;
    float cooldown_;        // 冷却时间（秒）
    float currentCooldown_; // 当前剩余冷却
    float multiplier_;      // 伤害倍率
};
```

- [ ] 实现具体技能（多态，面试必问）

```cpp
// Fireball：高倍率 + 附加 Burn Buff
class Fireball : public Skill {
public:
    Fireball();
    void Execute(Entity& caster, Entity& target) override;
};

// Heal：回复 HP
class Heal : public Skill {
public:
    Heal();
    void Execute(Entity& caster, Entity& target) override;
};

// DashAttack：中等倍率，短冷却
class DashAttack : public Skill {
public:
    DashAttack();
    void Execute(Entity& caster, Entity& target) override;
};
```

- [ ] 实现 `SkillManager`：管理角色拥有的技能列表，统一更新冷却

```cpp
class SkillManager {
public:
    void AddSkill(std::unique_ptr<Skill> skill);
    Skill* GetSkill(const std::string& name);
    void UpdateAllCooldowns(float deltaTime);
    void PrintSkillStatus() const;

private:
    std::vector<std::unique_ptr<Skill>> skills_;
};
```

- [ ] 技能释放时 emit `SkillCastEvent`

### 验收标准

```
[Player] Skills: Fireball (Ready), Heal (Ready), DashAttack (Ready)
Player casts Fireball → Enemy takes 45 damage
[Player] Skills: Fireball (CD: 5.0s), Heal (Ready), DashAttack (Ready)
--- tick 1s ---
[Player] Skills: Fireball (CD: 4.0s), Heal (Ready), DashAttack (Ready)
```

---

## 阶段 5：Buff 系统（Day 14-19）

### 目标
技能能附加 Buff，Buff 能随时间 tick 并自动过期。

### 任务清单

- [ ] 实现 `Buff` 基类

```cpp
// buff/Buff.h
#pragma once
#include <string>
#include "../entity/Entity.h"

enum class StackPolicy {
    None,       // 不可叠加
    Refresh,    // 刷新持续时间
    Stack       // 叠层数（效果加强）
};

class Buff {
public:
    Buff(const std::string& name, float duration, float tickInterval, StackPolicy policy);
    virtual ~Buff() = default;

    const std::string& GetName() const;
    bool IsExpired() const;
    int GetStacks() const;

    virtual void OnApply(Entity& target) = 0;   // 首次施加
    virtual void OnTick(Entity& target) = 0;    // 每次 tick
    virtual void OnRemove(Entity& target) = 0;  // 到期移除

    void Update(float deltaTime, Entity& target); // 推进时间，触发 tick
    void AddStack();
    void RefreshDuration();

protected:
    std::string name_;
    float duration_;
    float remainingTime_;
    float tickInterval_;
    float tickTimer_;
    int stacks_;
    StackPolicy policy_;
};
```

- [ ] 实现 4 种 Buff

```
BurnBuff     — 持续伤害（固定值/tick）
PoisonBuff   — 持续伤害（按目标最大 HP 百分比）
AttackBoost  — 增加攻击力属性（OnApply 加，OnRemove 减）
StunBuff     — 标记目标无法行动（需在 BattleManager 中检查）
```

- [ ] 实现 `BuffManager`

```cpp
class BuffManager {
public:
    void ApplyBuff(std::unique_ptr<Buff> buff, Entity& target);
    void UpdateAll(float deltaTime, Entity& target);
    void RemoveExpired(Entity& target);
    bool HasBuff(const std::string& name) const;
    void PrintBuffs() const;

private:
    std::vector<std::unique_ptr<Buff>> activeBuffs_;
};
```

- [ ] Buff 叠加逻辑：根据 `StackPolicy` 决定
  - `None` → 如果已存在同名 Buff，不施加
  - `Refresh` → 刷新已有 Buff 的持续时间
  - `Stack` → 增加层数
- [ ] Buff 施加时 emit `BuffAppliedEvent`

### 验收标准

```
Fireball hits Enemy → Burn applied (5s, 8 dmg/tick)
--- tick 1s ---
  [Burn] deals 8 damage to Enemy
--- tick 2s ---
  [Burn] deals 8 damage to Enemy
  [AttackBoost] active on Player (ATK +20, 3s remaining)
--- tick 5s ---
  [Burn] expired on Enemy
```

---

## 阶段 6：数据驱动（Day 20-23）

### 目标
改 JSON 就能调整技能/Buff 参数，不用重新编译。

### 任务清单

- [ ] 引入 `nlohmann/json`（header-only，放 `third_party/nlohmann/json.hpp`）
- [ ] 编写技能配置文件

```json
// data/config/skills.json
{
    "skills": [
        {
            "name": "Fireball",
            "type": "damage",
            "multiplier": 1.8,
            "cooldown": 5.0,
            "applyBuff": "Burn"
        },
        {
            "name": "Heal",
            "type": "heal",
            "healAmount": 50,
            "cooldown": 8.0
        },
        {
            "name": "DashAttack",
            "type": "damage",
            "multiplier": 1.2,
            "cooldown": 2.0
        }
    ]
}
```

- [ ] 编写 Buff 配置文件

```json
// data/config/buffs.json
{
    "buffs": [
        {
            "name": "Burn",
            "type": "dot",
            "damage": 8,
            "duration": 5.0,
            "tickInterval": 1.0,
            "stackPolicy": "refresh"
        },
        {
            "name": "Poison",
            "type": "dot_percent",
            "percentDamage": 0.03,
            "duration": 6.0,
            "tickInterval": 2.0,
            "stackPolicy": "stack"
        },
        {
            "name": "AttackBoost",
            "type": "stat_modify",
            "stat": "Attack",
            "value": 20,
            "duration": 5.0,
            "stackPolicy": "refresh"
        }
    ]
}
```

- [ ] 实现 `DataLoader`

```cpp
// data/DataLoader.h
#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../skill/Skill.h"
#include "../buff/Buff.h"

class DataLoader {
public:
    static std::unique_ptr<Skill> LoadSkill(const std::string& name);
    static std::unique_ptr<Buff> LoadBuff(const std::string& name);
    static void LoadAllConfigs(const std::string& configDir);

private:
    static void LoadSkillConfig(const std::string& path);
    static void LoadBuffConfig(const std::string& path);
};
```

- [ ] 修改 Fireball 等技能：构造函数参数从 JSON 读取而非硬编码

### 验收标准

修改 `skills.json` 中 Fireball 的 `multiplier` 从 1.8 改为 3.0，重新运行，伤害明显增大。无需重新编译（如果用动态加载）或仅重新运行即可。

---

## 阶段 7：Game Loop + 战斗模拟（Day 24-28）

### 目标
完整的 Player vs Enemy 战斗流程。

### 任务清单

- [ ] 实现 `BattleManager`

```cpp
// battle/BattleManager.h
#pragma once
#include <memory>
#include "../entity/Entity.h"
#include "../skill/SkillManager.h"
#include "../buff/BuffManager.h"

class BattleManager {
public:
    BattleManager(
        std::unique_ptr<Entity> player,
        std::unique_ptr<Entity> enemy
    );

    void Run(); // 主战斗循环

private:
    void PlayerTurn();
    void EnemyTurn();    // 简单 AI：随机选技能
    void TickBuffs();    // 推进所有 Buff
    void TickCooldowns();// 推进技能冷却
    void PrintBattleState();
    bool IsBattleOver() const;

    std::unique_ptr<Entity> player_;
    std::unique_ptr<Entity> enemy_;
    SkillManager playerSkills_;
    SkillManager enemySkills_;
    BuffManager playerBuffs_;
    BuffManager enemyBuffs_;
    int turnCount_;
};
```

- [ ] 玩家回合：控制台选择技能（输入数字）

```
=== Your Turn ===
1. Fireball (Ready)
2. Heal (CD: 3s)
3. DashAttack (Ready)
Choose skill:
```

- [ ] 敌人回合：随机选一个就绪技能
- [ ] 每回合结束后 tick Buff + 冷却
- [ ] 战斗结束判定（一方 HP <= 0）
- [ ] 注册 EventBus 监听，打印战斗日志

### 验收标准

```
===== BATTLE START =====
[Player] Warrior HP: 200/200 | [Enemy] Goblin HP: 150/150

=== Turn 1 ===
> Your skills: 1.Fireball 2.Heal 3.DashAttack
> Choose: 1
[Warrior] casts Fireball → Goblin takes 45 damage (CRIT!)
  ◆ Burn applied to Goblin (5s)
[Goblin] casts Slash → Warrior takes 18 damage

--- Buff Tick ---
  Goblin: Burn deals 8 damage

[Warrior] HP: 182/200 | [Goblin] HP: 97/150

=== Turn 2 ===
...

=== Turn 5 ===
[Warrior] casts DashAttack → Goblin takes 28 damage
Goblin is defeated!

===== BATTLE END =====
Winner: Warrior (HP: 145/200)
```

---

## 阶段 8：测试 + 打磨（Day 29-35）

### 目标
项目可以自信地放到简历上。

### 任务清单

- [ ] 引入 GoogleTest（放 `third_party/googletest/` 或用 CMake FetchContent）
- [ ] 编写单元测试

```
test/combat_test.cpp  — 伤害公式测试（固定 seed 消除随机性）
test/buff_test.cpp    — Buff 叠加、过期、tick 伤害
test/event_test.cpp   — 订阅、发布、取消订阅
test/skill_test.cpp   — 技能冷却、释放条件
```

- [ ] 代码审查 checklist
  - [ ] 所有 `new` 都用 `unique_ptr` 或 `shared_ptr` 包装
  - [ ] 没有裸 `new` / `delete`
  - [ ] 虚析构函数（基类）
  - [ ] `override` 关键字
  - [ ] `const` 正确性
  - [ ] 无内存泄漏（可用 Valgrind / AddressSanitizer 验证）
- [ ] 编写 `README.md`
  - 项目简介
  - 架构图（文字或 Mermaid）
  - 构建方式（`cmake -B build && cmake --build build`）
  - 运行方式
  - 设计思路（为什么用事件系统、为什么用模板）
- [ ] 确保 `cmake` 构建无 warning

---

## 核心技术点速查表

| 话题 | 用到的技术 | 面试怎么说 |
|---|---|---|
| 多态 | `Skill` / `Buff` 虚函数 + `override` | "技能和 Buff 通过基类指针统一管理，新增类型只需继承" |
| 智能指针 | `unique_ptr<Buff>` 在 Entity 上 | "用 unique_ptr 表达独占所有权，避免手动管理内存" |
| 模板 | EventBus 类型安全分发 | "用 type_index + any 实现编译期类型安全的事件系统" |
| STL 容器 | `unordered_map`, `vector`, `function` | "属性系统用 map 做动态查询，Buff 列表用 vector 管理" |
| 设计模式 | 观察者、策略、工厂 | "事件系统是观察者模式，技能是策略模式，角色创建用工厂" |
| RAII | 智能指针 + 析构函数 | "资源获取即初始化，生命周期由作用域控制" |
| 数据驱动 | JSON 配置 + 运行时加载 | "策划改 JSON 即可调参，不需要程序重新编译" |

---

## 面试高频问题准备

### 系统设计类
- **为什么用事件系统？** → 解耦模块，技能不需要知道战斗日志的存在，只管 Emit 事件
- **Buff 叠加怎么处理？** → StackPolicy 枚举：None / Refresh / Stack，由 BuffManager 统一判断
- **技能冷却怎么实现？** → 每个 Skill 维护 currentCooldown，BattleManager 每 tick 调用 UpdateCooldown
- **如何避免内存泄漏？** → 全部使用智能指针，RAII 管理资源

### C++ 语言类
- **为什么用虚函数？** → 运行时多态，基类指针调用派生类实现
- **为什么用 unique_ptr 而不是 shared_ptr？** → 所有权明确时用 unique_ptr，语义更清晰，开销更小
- **std::any 的原理？** → 类型擦除，内部存储 type_info + void* + 小对象优化
- **如何避免对象拷贝？** → move 语义 + 禁用拷贝构造（`= delete`）

---

## 每日建议

- 每完成一个阶段就 **git commit + push**
- commit message 用英文，格式如 `feat: implement event bus with type-safe dispatch`
- 遇到 bug 先写测试复现，再修复
- 不要追求完美，先跑通再优化
