#pragma once

#include "CoreMinimal.h"
#include "MovSysInteractableBase.h"
#include "DeusExMachina/Interaction/Interactable.h"
#include "DeusExMachina/MovableSystem/Rotation/ControlledRotationDatas.h"
#include "MovSysWheel.generated.h"


class ARotationSupport;
class URotationBehaviorControlled;


USTRUCT(BlueprintType, meta = (Tooltip = "Link a Rotation Support with the Rotation Behavior Controlled component to a MovSys Wheel."))
struct FControlledRotInteractionLink
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The Rotation Support you want to link. It must have the Rotation Behavior Controlled component."))
	ARotationSupport* RotationSupport{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The datas you want to link the support with. It will be used on this Rotation Support when using the button."))
	FControlledRotationDatas ControlDatas;

	URotationBehaviorControlled* RotationControlledComponent{ nullptr };
};


UCLASS()
class DEUSEXMACHINA_API AMovSysWheel : public AMovSysInteractableBase, public IInteractable
{
	GENERATED_BODY()
	
public:
	AMovSysWheel();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;



// ======================================================
//                 Interactable Interface
// ======================================================
public:
	void Interaction_Implementation() override;
	bool CanInteract_Implementation() override;

	bool IsInteractionHeavy_Implementation() override;
	void InteractionHeavyUpdate_Implementation(FVector2D ControlValue) override;
	void InteractionHeavyFinished_Implementation() override;

	virtual void ForceReleaseInteractable() override;


// ======================================================
//          Wheel Visual Feedback (blueprint)
// ======================================================
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Wheel Feedback")
	void WheelStartFeedback();

	UFUNCTION(BlueprintImplementableEvent, Category = "Wheel Feedback")
	void WheelStopFeedback();

	UFUNCTION(BlueprintImplementableEvent, Category = "Wheel Feedback")
	void WheelControlFeedback(float ControlValue);


// ======================================================
//             Wheel Customization (editor)
// ======================================================
public:
	UPROPERTY(EditInstanceOnly, Category = "Options", meta = (Tooltip = "Wether or not activate the advanced joystick control on this wheel.\nTrue by default. Deactivate it only for testing purposes."))
	bool bActivateAdvancedJoystickControl{ true };


// ======================================================
//               Wheel Internal Variables
// ======================================================
protected:
	bool bInControl{ false };


// ======================================================
//                 Interaction Links
// ======================================================
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (tooltip = "Rotation Supports with the Rotation Behavior Controlled you want to link to this wheel."))
	TArray<FControlledRotInteractionLink> LinkedSupportsControlled;

protected:
	TArray<FControlledRotInteractionLink> LinkedSupportsControlledVerified;
};
