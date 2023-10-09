#include "pch.h"
#include "C_PrecedenceSequencer.h"

void C_PrecedenceSequencer::on_update(float dt)
{
    bool success = true;
    for (auto&& child : children)
    {
        if (success)
        {
            child->tick(dt);
        }
        else
        {
            child->set_status(NodeStatus::READY);
        }

        if (child->failed())
        {
            on_failure(); success = false;
        }
        else if (child->succeeded())
        {
            child->set_status(NodeStatus::READY);
        }
        else
        {
            success = false;
        }
    }

    if (success)
    {
        on_success();
    }
}