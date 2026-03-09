#include "Buff.h"
#include "../entity/Entity.h"

Buff::Buff(const std::string& name, float duration, float tickInterval, StackPolicy policy)
    : name_(name), duration_(duration), remainingTime_(duration), tickInterval_(tickInterval), tickTimer_(0), stacks_(1), policy_(policy) {}

const std::string& Buff::GetName() const {
    return name_;
}
bool Buff::IsExpired() const {
    return remainingTime_ <= 0;
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
    }
}
void Buff::RefreshDuration() {
    if (policy_ == StackPolicy::Refresh) {
        remainingTime_ = duration_;
    }
}