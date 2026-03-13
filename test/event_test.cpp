#include <gtest/gtest.h>
#include "event/EventBus.h"
#include "event/Events.h"

TEST(EventBus, SubscribeEmitUnsubscribe) {
    using namespace std;
    bool called = false;
    float lastDamage = 0.0f;

    auto& bus = EventBus::Instance();
    size_t handlerId = bus.Subscribe<DamageEvent>([&](const DamageEvent& e){
        called = true;
        lastDamage = e.damage;
    });

    // Emit -> handler should be called
    bus.Emit<DamageEvent>({1, 2, 12.5f, false});
    EXPECT_TRUE(called);
    EXPECT_NEAR(lastDamage, 12.5f, 1e-6f);

    // Unsubscribe and emit again -> handler should NOT be called
    called = false;
    bus.Unsubscribe(handlerId);
    bus.Emit<DamageEvent>({1, 2, 7.0f, false});
    EXPECT_FALSE(called);
}