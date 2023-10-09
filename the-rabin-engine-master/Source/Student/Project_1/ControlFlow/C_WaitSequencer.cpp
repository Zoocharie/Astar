#include "pch.h"
#include "C_WaitSequencer.h"

void C_WaitSequencer::on_update(float dt)
{

    for (auto&& child : children)
    {
        child->tick(dt);

        if (child->failed() == true)
        {
            on_failure();
        }
        else if (child->succeeded())
        {
            child->set_status(NodeStatus::READY);
            finishedNodes.emplace(child);
        }


    }

    if (finishedNodes.size() == children.size())
    {
        std::cout << "Success" << std::endl;
        finishedNodes.clear();
        on_success();
    }
}