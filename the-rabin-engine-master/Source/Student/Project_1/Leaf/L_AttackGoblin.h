#pragma once

#include "BehaviorNode.h"

NODE(L_AttackGoblin)
{
protected:
    virtual void on_update(float dt) override;
    Agent* target = nullptr;
    bool cooldown = false;
};
