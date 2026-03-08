#pragma once
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <any>
#include <algorithm>
#include "Events.h"

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