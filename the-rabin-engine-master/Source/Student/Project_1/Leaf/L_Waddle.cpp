#include "pch.h"
#include "L_Waddle.h"

void L_Waddle::on_update(float dt)
{

	float roll = agent->get_roll();
	float speed = agent->get_movement_speed();

	if (left)
	{
		roll = roll - dt;
		if (roll < -0.1f)
		{
			left = !left;
		}
	}
	else
	{
		roll = roll + dt;
		if (roll >0.1f)
		{
			left = !left;
		}
	}
	agent->set_roll(roll);
	on_success();
	display_leaf_text();
}

