#pragma once
#include "BehaviorNode.h"
#include <unordered_set>

NODE(C_WaitSequencer)
{
protected:
    virtual void on_update(float dt) override;
    std::unordered_set<BehaviorNode*> finishedNodes;
};