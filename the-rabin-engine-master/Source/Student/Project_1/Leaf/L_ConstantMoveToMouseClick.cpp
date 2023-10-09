#include "pch.h"
#include "L_ConstantMoveToMouseClick.h"

void L_ConstantMoveToMouseClick::on_update(float dt)
{
    const auto& bb = agent->get_blackboard();
    if (bb.has_value<Vec3>("Click Position") == false)
    {
        on_success();
    }
    else
    {
        Vec3 vec3 = bb.get_value<Vec3>("Click Position");
        const auto result = agent->move_toward_point(vec3, dt);

        if (result == true)
        {
            on_success();
        }

    }
    display_leaf_text();
}