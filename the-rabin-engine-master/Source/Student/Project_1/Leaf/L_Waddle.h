#pragma once

#include "BehaviorNode.h"

NODE(L_Waddle)
{
protected:
    virtual void on_update(float dt) override;
    bool left = true;
};

