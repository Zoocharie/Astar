
#include "pch.h"
#include "L_PlayGoblinNoises.h"

void L_PlayGoblinNoises::on_update(float dt)
{
	int i = RNG::d2();
	if (i == 1)
	{
		audioManager->PlaySoundEffect(L"Assets\\Audio\\Goblin1.wav");
	}
	else
	{
		audioManager->PlaySoundEffect(L"Assets\\Audio\\Goblin2.wav");
	}
	on_success();
	display_leaf_text();
}