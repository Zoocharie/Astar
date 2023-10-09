#include "pch.h"
#include "L_AttackGoblin.h"

void L_AttackGoblin::on_update(float dt)
{
    Vec3 position = agent->get_position() + agent->get_up_vector() * agent->get_scaling().y * 10.f;
    position.y = 0;
    float pitch = agent->get_pitch();
    if (cooldown)
    {
        float newPitch = pitch - dt * 2.f;
        agent->set_pitch(newPitch);
        if (newPitch < 1.f)
        {
            cooldown = false;
            on_success();
        };
    }
    else
    {
        //Look for targets
        if (target == nullptr)
        {
            for (auto agent : agents.get()->get_all_agents())
            {
                if (agent->get_type() == "Goblin")
                {
                    float dist = Vec3::Distance(agent->get_position(), position);
                    if (target == nullptr && dist < 5.f)
                    {
                        target = agent;
                    }
                    break;
                }
            }
        }
        //Swinging
        else
        {
            float newPitch = pitch + dt * 2.f;
            agent->set_pitch(newPitch);
            if (newPitch > 2.f)
            {
                target = nullptr;
                cooldown = true;
            }
            Vec3 pos = agent->get_position();
            if (newPitch > 1.7f)
            {
                for (auto agent : agents.get()->get_all_agents())
                {
                    if (agent->get_type() == "Goblin")
                    {
                        float dist = Vec3::Distance(agent->get_position(), position);
                        Vec3 dir = agent->get_position() - pos;
                        dir.y = 0;
                        dir.Normalize();
                        if (dist < 5.f)
                        {
                            agent->set_position(agent->get_position() + dir * 2.f);
                        }
                    }
                }
            }
        }
    }
    display_leaf_text();
}