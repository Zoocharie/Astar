#include "pch.h"
#include "L_PlaySwordNoise2.h"

void L_PlaySwordNoise2::on_update(float dt)
{
	audioManager->PlaySoundEffect(L"Assets\\Audio\\Sword2.wav");
	on_success();
}
