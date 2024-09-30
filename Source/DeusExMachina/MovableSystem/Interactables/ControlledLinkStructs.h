#pragma once

#include "CoreMinimal.h"
#include "DeusExMachina/MovableSystem/Rotation/ControlledRotationDatas.h"
#include "DeusExMachina/MovableSystem/Translation/ControlledTranslationDatas.h"
#include "ControlledLinkStructs.generated.h"


class ARotationSupport;
class URotationBehaviorControlled;
class ATranslationSupport;
class UTranslationBehaviorControlled;


USTRUCT(BlueprintType, meta = (Tooltip = "Link a Rotation Support with the Rotation Behavior Controlled component to a MovSys Wheel or Handle."))
struct FControlledRotInteractionLink
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The Rotation Support you want to link. It must have the Rotation Behavior Controlled component."))
	ARotationSupport* RotationSupport{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The datas you want to link the support with. It will be used on this Rotation Support when using the wheel or handle."))
	FControlledRotationDatas ControlDatas;

	URotationBehaviorControlled* RotationControlledComponent{ nullptr };
};


USTRUCT(BlueprintType, meta = (Tooltip = "Link a Translation Support with the Translation Behavior Controlled component to a MovSys Wheel or Handle."))
struct FControlledTransInteractionLink
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The Translation Support you want to link. It must have the Translation Behavior Controlled component."))
	ATranslationSupport* TranslationSupport{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The datas you want to link the support with. It will be used on this Translation Support when using the wheel or handle."))
	FControlledTranslationDatas ControlDatas;

	UTranslationBehaviorControlled* TranslationControlledComponent{ nullptr };
};