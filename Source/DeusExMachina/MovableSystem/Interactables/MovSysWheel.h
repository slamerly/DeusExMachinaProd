#pragma once

#include "CoreMinimal.h"
#include "MovSysInteractableBase.h"
#include "DeusExMachina/Interaction/Interactable.h"
#include "ControlledLinkStructs.h"
#include "MovSysWheel.generated.h"


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
	void InteractionHeavyUpdate_Implementation(FVector2D ControlValue, bool KeyboardInput) override;
	void InteractionHeavyFinished_Implementation() override;

	bool CanInteract_Implementation() override;
	bool IsInteractionHeavy_Implementation() override;
	bool IsInteractableBothSides_Implementation() override;


// ======================================================
//              Mov Sys Interactable Base
// ======================================================
	virtual void ForceReleaseInteractable() override;


// ======================================================
//               Advanced Joystick Control
// ======================================================
protected:
	float GetAdvancedJoystickControl(const FVector2D JoystickValue);



// ======================================================
//           Wheel Visual Feedback (blueprint)
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

	// ===========================
	//  Advanced Joystick Control
	// ===========================
	float JoystickAngle{ 0.0f };
	float JoystickForgotTime{ 0.0f };
	float JoystickLastControl{ 0.0f };


// ======================================================
//                 Interaction Links
// ======================================================
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Link Rotation Supports", meta = (tooltip = "Rotation Supports with the Rotation Behavior Controlled you want to link to this wheel."))
	TArray<FControlledRotInteractionLink> LinkedRotSupportsControlled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Link Translation Supports", meta = (tooltip = "Translation Supports with the Translation Behavior Controlled you want to link to this wheel."))
	TArray<FControlledTransInteractionLink> LinkedTransSupportsControlled;

protected:
	TArray<FControlledRotInteractionLink> LinkedRotSupportsControlledVerified;
	TArray<FControlledTransInteractionLink> LinkedTransSupportsControlledVerified;
};
