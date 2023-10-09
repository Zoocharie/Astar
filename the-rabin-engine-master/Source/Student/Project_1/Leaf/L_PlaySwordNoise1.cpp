#include "pch.h"
#include "L_PlaySwordNoise1.h"

void L_PlaySwordNoise1::on_update(float dt)
{
	audioManager->PlaySoundEffect(L"Assets\\Audio\\Sword1.wav");
	on_success();
}
