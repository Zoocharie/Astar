#pragma once
#include "BehaviorNode.h"

NODE(L_TeleportToMouse)
{
protected:
    virtual void on_update(float dt) override;
    bool teleporting = false;
    bool appear = true;
};