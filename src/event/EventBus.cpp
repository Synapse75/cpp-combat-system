#include "EventBus.h"

void EventBus::Unsubscribe(size_t handlerId) {
    for (auto& [key, handlers] : handlers_) {
        handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
            [handlerId](const Handler& h) { return h.id == handlerId; }), handlers.end());
    }
}

EventBus& EventBus::Instance() {
    static EventBus instance;
    return instance;
}