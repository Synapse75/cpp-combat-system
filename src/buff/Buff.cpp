#include "Buff.h"
#include "../entity/Entity.h"
#include <iostream>

Buff::Buff(const nlohmann::json& config)
    : name_(config.value("name", "Unknown Buff")),
      type_(config.value("type", "generic")),
      duration_(config.value("duration", 0.0f)),
      remainingTime_(config.value("duration", 0.0f)),
      tickInterval_(config.value("tickInterval", 0.0f)),
      tickTimer_(0.0f),
      stacks_(config.value("stacks", 1)),
      policy_(StackPolicy::Refresh) // 默认值，后面再根据配置覆盖
{
    if (config.contains("stackPolicy")) {
        const auto& sp = config["stackPolicy"];
        if (sp.is_string()) {
            std::string s = sp.get<std::string>();
            for (auto &c : s) c = static_cast<char>(tolower(c));
            if (s == "stack") policy_ = StackPolicy::Stack;
            else policy_ = StackPolicy::Refresh;
        } else if (sp.is_number_integer()) {
            int v = sp.get<int>();
            policy_ = static_cast<StackPolicy>(v);
        }
    }
}

const std::string& Buff::GetName() const {
    return name_;
}
bool Buff::IsExpired() const {
    return int(remainingTime_) <= 0;
}
int Buff::GetStacks() const {
    return stacks_;
}
void Buff::Update(float deltaTime, Entity& target) {
    if (IsExpired()) return;

    remainingTime_ -= deltaTime;
    tickTimer_ += deltaTime;

    if (tickTimer_ >= tickInterval_) {
        OnTick(target);
        tickTimer_ = 0;
    }

    if (IsExpired()) {
        OnRemove(target);
    }
}
void Buff::AddStack() {
    if (policy_ == StackPolicy::Stack) {
        stacks_++;
        std::cout << "Buff: " << name_ << " stacked! Current stacks: " << stacks_ << "\n";
    }
}
void Buff::RefreshDuration() {
    remainingTime_ = duration_;
    std::cout << "Buff: " << name_ << " duration refreshed!\n";
}