#pragma once

#include "BehaviorNode.h"

NODE(C_PrecedenceSequencer)
{
protected:
    virtual void on_update(float dt) override;
};