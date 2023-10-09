#pragma once

// Include all node headers in this file

// Example Control Flow Nodes
#include "ControlFlow/C_ParallelSequencer.h"
#include "ControlFlow/C_RandomSelector.h"
#include "ControlFlow/C_Selector.h"
#include "ControlFlow/C_Sequencer.h"

// Student Control Flow Nodes
#include "ControlFlow/C_WaitSequencer.h"
#include "ControlFlow/C_PrecedenceSequencer.h"


// Example Decorator Nodes
#include "Decorator/D_Delay.h"
#include "Decorator/D_InvertedRepeater.h"
#include "Decorator/D_RepeatFourTimes.h"

// Student Decorator Nodes
#include "Decorator/D_WaitFor1Second.h"

// Example Leaf Nodes
#include "Leaf/L_CheckMouseClick.h"
#include "Leaf/L_Idle.h"
#include "Leaf/L_MoveToFurthestAgent.h"
#include "Leaf/L_MoveToMouseClick.h"
#include "Leaf/L_MoveToRandomPosition.h"
#include "Leaf/L_PlaySound.h"

// Student Leaf Nodes
#include "Leaf/L_ConstantMoveToMouseClick.h"
#include "Leaf/L_Waddle.h"
#include "Leaf/L_AvoidPlayer.h"
#include "Leaf/L_AttachPlayerRightArm.h"
#include "Leaf/L_AttackGoblin.h"
#include "Leaf/L_PlayGoblinNoises.h"
#include "Leaf/L_TeleportToMouse.h"
#include "Leaf/L_PlaySwordNoise1.h"
#include "Leaf/L_PlaySwordNoise2.h"
#include "Leaf/L_PlayWizardNoise1.h"
#include "Leaf/L_PlayWizardNoise2.h"
#include "Leaf/L_PlayPlayerNoise.h"