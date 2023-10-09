#include "pch.h"
#include "L_PlayPlayerNoise.h"

void L_PlayPlayerNoise::on_update(float dt)
{
	audioManager->PlaySoundEffect(L"Assets\\Audio\\Player.wav");
	on_success();
}

