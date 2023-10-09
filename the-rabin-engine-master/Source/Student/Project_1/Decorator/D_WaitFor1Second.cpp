#include "pch.h"
#include "D_WaitFor1Second.h"

void D_WaitFor1Second::on_enter()
{
    delay = 1.f;
    BehaviorNode::on_enter();
}

void D_WaitFor1Second::on_update(float dt)
{
    delay -= dt;
    if (delay < 0.0f)
    {
        BehaviorNode* child = children.front();

        child->tick(dt);

        // assume same status as child
        set_status(child->get_status());
        set_result(child->get_result());
    }
}