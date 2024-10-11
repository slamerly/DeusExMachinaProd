#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerInputMode : uint8
{
	PlayerMovement = 0 UMETA(ToolTip = "The input mode when the player move around in the scene and interact with objects."),
	InteractionHeavy = 1 UMETA(ToolTip = "The input mode when the player is controlling a heavy interaction, like a wheel."),
	NarrationControl = 2 UMETA(ToolTip = "The input mode for narration phase with dialogues.")
};

UENUM(BlueprintType)
enum class EBlockPlayerCause : uint8
{
	PauseMenu = 0 UMETA(ToolTip = "Block player inputs cause using pause menu."),
	SceneTransition = 1 UMETA(ToolTip = "Block player inputs cause changing scene."),
	TeleportationInsideScene = 2 UMETA(ToolTip = "Block player inputs cause using a teleporting door."),
	MovingPlatform = 3 UMETA(ToolTip = "Block player inputs cause he's on a moving platform."),
	Narration = 4 UMETA(ToolTip = "Block player inputs cause of narration or cinematic.")
};