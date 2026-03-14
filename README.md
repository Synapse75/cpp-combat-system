# cpp-combat-system
## 项目简介
一个用 C++ 实现的模块化战斗系统框架（控制台演示）。目标是展示事件系统、技能、Buff 与战斗流程的设计与实现。
## 架构
core（类型/工具）
entity（实体/属性）
event（EventBus）
combat（伤害计算）
skill（技能体系）
buff（状态体系）
battle（回合流程）
data（JSON 配置）
## 构建方式
**前置依赖：**
cmake ( >= 3.15 )，
C++17 支持的编译器（MSVC / GCC / Clang）。

**构建：**
MinGW (single-config)
```
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```
Visual Studio (multi-config)
```
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```
## 运行方式
Unix:
```
build/combat_system
```
Windows + VS:
```
build\Debug\combat_system.exe
```
或
```
build\Release\combat_system.exe
```
（取决于 --config）

## 测试
运行所有测试：
```
ctest --test-dir build --output-on-failure -C Debug
```
（或在单配置生成器中省略 -C Debug）。

## 附录
1. 完整结构图
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
│   │   └── Types.cpp
│   │
│   ├── entity/          # 实体系统 + 属性系统
│   │   ├── Entity.h
│   │   ├── Entity.cpp
│   │   ├── EntityFactory.h
│   │   └── EntityFactory.cpp
│   │
│   ├── event/           # 模板化事件总线
│   │   ├── Event.h
│   │   ├── EventBus.h
│   │   └── EventBus.cpp
│   │
│   ├── combat/          # 战斗计算
│   │   ├── CombatSystem.h
│   │   └── CombatSystem.cpp
│   │
│   ├── skill/           # 技能系统
│   │   ├── Skill.h
│   │   ├── Skill.cpp
│   │   ├── SkillManager.h
│   │   └── SkillManager.cpp
│   │
│   ├── buff/            # Buff / 状态系统
│   │   ├── Buff.h
│   │   ├── Buff.cpp
│   │   ├── DotBuff.h
│   │   ├── DotBuff.cpp
│   │   ├── DotPercentBuff.h
│   │   ├── DotPercentBuff.cpp
│   │   ├── StatModifyBuff.h
│   │   ├── StatModifyBuff.cpp
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
│       ├── EntityRegistry.h
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
    └── nlohmann/        # nlohmann/json
```