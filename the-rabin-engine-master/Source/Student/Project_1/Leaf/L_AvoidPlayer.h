#pragma once
#include "BehaviorNode.h"

NODE(L_AvoidPlayer)
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    Agent* player;
};
