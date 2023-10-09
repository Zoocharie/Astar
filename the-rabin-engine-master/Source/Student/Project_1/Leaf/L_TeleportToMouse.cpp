#include "pch.h"
#include "L_TeleportToMouse.h"

void L_TeleportToMouse::on_update(float dt)
{
    const auto& bb = agent->get_blackboard();
    if (bb.has_value<Vec3>("Click Position") == false)
    {
        on_success();
        display_leaf_text();
        return;
    }

    if (!teleporting)
    {
        teleporting = true;
        appear = false;
    }
    else
    {
        Vec3 pos = agent->get_position();
        if (!appear)
        {
            Vec3 scale = agent->get_scaling();
            if (scale.x > 0)
                scale.x -= dt* 10;
            if (scale.z > 0)
                scale.z -= dt* 10;
            if (scale.y < 10)
                scale.y += dt*10;
            else
            {
                appear = true;
                Vec3 targetPoint = bb.get_value<Vec3>("Click Position");
                pos.x = targetPoint.x;
                pos.z = targetPoint.z;
            }
            pos.y += dt * 5;
            agent->set_position(pos);
            agent->set_scaling(scale);
        }
        else
        {
            Vec3 scale = agent->get_scaling();
            if (scale.x < 3)
                scale.x += dt * 10;
            if (scale.z < 3)
                scale.z += dt * 10;
            if (scale.y > 3)
                scale.y -= dt * 10;
            pos.y -= dt * 5;
            if (pos.y < 0)
            {
                teleporting = false;
                on_success();
            }
            agent->set_position(pos);
            agent->set_scaling(scale);
        }
    }
    display_leaf_text();
}