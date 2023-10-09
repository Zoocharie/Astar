#include "pch.h"
#include "L_AttachPlayerRightArm.h"

void L_AttachPlayerRightArm::on_enter()
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

void L_AttachPlayerRightArm::on_update(float dt)
{
    Vec3 pos = player->get_position();
    pos += Vec3::Up * 10.f;
    pos += player->get_right_vector() * 3.f;
    agent->set_position(pos);
    agent->set_yaw(player->get_yaw());
    on_success();
    display_leaf_text();
}