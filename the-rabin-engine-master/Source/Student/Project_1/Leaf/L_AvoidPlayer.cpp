#include "pch.h"
#include "L_AvoidPlayer.h"

void L_AvoidPlayer::on_enter()
{
    if (player == nullptr)
    {
        for (auto agent : agents.get()->get_all_agents())
        {
            if (agent->get_type() == "Player")
            {
                player = agent;
                break;
            }
        }
    }
    BehaviorNode::on_leaf_enter();
}

void L_AvoidPlayer::on_update(float dt)
{
    Vec3 direction = player->get_position() - agent->get_position();
    direction.y = 0;
    if (direction.Length() < 30.f)
    {
        agent->move_toward_point(-direction*agent->get_movement_speed(), dt);
    }
    else
    {
        on_success();
    }
    display_leaf_text();
}