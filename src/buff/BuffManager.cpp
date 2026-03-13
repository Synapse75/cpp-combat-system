#include "BuffManager.h"
#include "../event/Events.h"
#include "../event/EventBus.h"
#include "../entity/Entity.h"
#include "../data/EntityRegistry.h"
#include <iostream>
#include <algorithm>

BuffManager::BuffManager() : ownerId_(-1) {
    removeSubscriptionId_ = EventBus::Instance().Subscribe<BuffRemoveEvent>(
    [this](const BuffRemoveEvent& e) {
        if (e.targetId != ownerId_) return;
        if (inUpdate_) {
            pendingRemovals_.push_back(e.buffName);
        } else {
            // safe to remove immediately
            activeBuffs_.erase(std::remove_if(activeBuffs_.begin(), activeBuffs_.end(),
                [&](const std::unique_ptr<Buff>& b){ return b->GetName() == e.buffName; }),
                activeBuffs_.end());
        }
    }
);
}

void BuffManager::ProcessPendingRemovals() {
    if (pendingRemovals_.empty()) return;
    for (const auto& name : pendingRemovals_) {
        activeBuffs_.erase(std::remove_if(activeBuffs_.begin(), activeBuffs_.end(),
            [&](const std::unique_ptr<Buff>& b){ return b->GetName() == name; }),
            activeBuffs_.end());
    }
    pendingRemovals_.clear();
}

void BuffManager::ApplyBuff(std::unique_ptr<Buff> buff, Entity& target) {
    // 查找是否已有同名 buff
    for (const auto& b : activeBuffs_) {
        if (b->GetName() == buff->GetName()) {
            if (buff->GetStackPolicy() == StackPolicy::Stack) {
                b->AddStack();
                b->RefreshDuration();
            } else if (buff->GetStackPolicy() == StackPolicy::Refresh) {
                b->RefreshDuration();
            } 
            return;
        }
    }

    // 没有同名 buff：直接添加
    buff->OnApply(target);
    activeBuffs_.push_back(std::move(buff));
}

void BuffManager::UpdateAll(float deltaTime, Entity& target) {
    ownerId_ = target.GetId();
    inUpdate_ = true;
    for (auto& buff : activeBuffs_) buff->Update(deltaTime, target);
    inUpdate_ = false;

    ProcessPendingRemovals();
    RemoveExpired();
}

void BuffManager::RemoveExpired() {
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