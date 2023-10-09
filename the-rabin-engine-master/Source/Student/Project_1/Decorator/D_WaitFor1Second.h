#pragma once

#include "BehaviorNode.h"

NODE(D_WaitFor1Second)
{
protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;
	float delay;
};