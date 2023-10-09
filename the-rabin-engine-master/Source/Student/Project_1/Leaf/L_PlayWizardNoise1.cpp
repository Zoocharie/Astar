#include "pch.h"
#include "L_PlayWizardNoise1.h"

void L_PlayWizardNoise1::on_update(float dt)
{
	audioManager->PlaySoundEffect(L"Assets\\Audio\\Wizard1.wav");
	on_success();
}

