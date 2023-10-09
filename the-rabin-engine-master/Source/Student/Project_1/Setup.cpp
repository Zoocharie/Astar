#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"

void ProjectOne::setup()
{
    // Create your inital agents
    agents->create_behavior_agent("Player", BehaviorTreeTypes::PlayerBehaviour);

    for (int i = 1; i < 5; ++i)
    {
        auto goblin = agents->create_behavior_agent("Goblin", BehaviorTreeTypes::GoblinBehaviour);
        goblin->set_color({ .25,.75,.25 });
        goblin->set_scaling({ 2.5,.6,2 });
        goblin->set_movement_speed({ -5.f });
        goblin->set_position({ 50.f+ 10*i,0,50.f + 10 * i });
    }

    auto sword = agents->create_behavior_agent("Sword", BehaviorTreeTypes::SwordBehaviour);
    sword->set_scaling({ .2f,2.f,1.f });
    sword->set_pitch(1.f);

    auto wizard = agents->create_behavior_agent("Wizard", BehaviorTreeTypes::WizardBehaviour);
    wizard->set_position({ 75,0,75 });
    // you can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code that yourself
    // that's the realm of project 2 though
    terrain->goto_map(0);

    // you can also enable the pathing layer and set grid square colors as you see fit
    // works best with map 0, the completely blank map
    //terrain->pathLayer.set_enabled(true);
    //terrain->pathLayer.set_value(0, 0, Colors::Red);

    // camera position can be modified from this default as well
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees

    audioManager->SetVolume(.5f);
    //audioManager->PlaySoundEffect(L"Assets\\Audio\\retro.wav");
    // uncomment for example on playing music in the engine (must be .wav)
    audioManager->PlayMusic(L"Assets\\Audio\\BGM_Combat.wav");
    // audioManager->PauseMusic(...);
    // audioManager->ResumeMusic(...);
    // audioManager->StopMusic(...);
}