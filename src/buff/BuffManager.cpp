#include "BuffManager.h"
#include "../event/Events.h"
#include "../event/EventBus.h"
#include "../entity/Entity.h"
#include <iostream>
#include <algorithm>
void BuffManager::ApplyBuff(std::unique_ptr<Buff> buff, Entity& target) {
    // 检查是否已有同名buff
    for (const auto& b : activeBuffs_) {
        if (b->GetName() == buff->GetName()) {
            if (buff->GetStackPolicy() == StackPolicy::Stack) {
                b->AddStack();
                std::cout << "Stacked buff: " << b->GetName() << " now has " << b->GetStacks() << " stacks." << std::endl;
            } else if (buff->GetStackPolicy() == StackPolicy::Refresh) {
                b->RefreshDuration();
                std::cout << "Refreshed buff: " << b->GetName() << " duration reset." << std::endl;
            }
            return; // 已处理完毕，直接返回
        }
    }

    // 没有同名buff，直接添加
    buff->OnApply(target);
    activeBuffs_.push_back(std::move(buff));
}
void BuffManager::UpdateAll(float deltaTime, Entity& target) {
    for (auto& buff : activeBuffs_) {
        buff->Update(deltaTime, target);
    }
}
void BuffManager::RemoveExpired(Entity& target) {
    activeBuffs_.erase(std::remove_if(activeBuffs_.begin(), activeBuffs_.end(),
        [](const std::unique_ptr<Buff>& buff) { return buff->IsExpired(); }), activeBuffs_.end());
}
bool BuffManager::HasBuff(const std::string& name) const {
    for (const auto& buff : activeBuffs_) {
        if (buff->GetName() == name) {
            return true;
        }
    }
    return false;
}
void BuffManager::PrintBuffs() const {
    std::cout << "Active Buffs:" << std::endl;
    for (const auto& buff : activeBuffs_) {
        std::cout << "- " << buff->GetName() << " (Stacks: " << buff->GetStacks() << ", Remaining Time: " << buff->GetRemainingTime() << "s)" << std::endl;
    }
}
