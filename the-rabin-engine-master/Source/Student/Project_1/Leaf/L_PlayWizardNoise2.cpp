#include "pch.h"
#include "L_PlayWizardNoise2.h"

void L_PlayWizardNoise2::on_update(float dt)
{
	audioManager->PlaySoundEffect(L"Assets\\Audio\\Wizard2.wav");
	on_success();
}

