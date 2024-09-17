#include "MovSysWheel.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationSupport.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationBehaviorControlled.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Defines.h"

AMovSysWheel::AMovSysWheel()
{
	PrimaryActorTick.bCanEverTick = false;
}



// ======================================================
//                     Begin Play
// ======================================================
void AMovSysWheel::BeginPlay()
{
	Super::BeginPlay();

	for (auto& LinkCtrl : LinkedSupportsControlled)
	{
		if (!IsValid(LinkCtrl.RotationSupport)) continue; //  do not check if there is no rotation support linked
		if (!LinkCtrl.ControlDatas.IsDataValid()) continue; //  do not check if there is no datas linked

		if (!IsValid(LinkCtrl.RotationSupport->GetComponentByClass<URotationBehaviorControlled>())) continue;

		LinkedSupportsControlledVerified.Add(FControlledRotInteractionLink{ LinkCtrl.RotationSupport, LinkCtrl.ControlDatas, LinkCtrl.RotationSupport->GetComponentByClass<URotationBehaviorControlled>() });
	}
}



// ======================================================
//             Check links validity (editor)
// ======================================================
void AMovSysWheel::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//  check if the property changed is the rotation support in a link
	if (PropertyChangedEvent.GetPropertyName() != FName("RotationSupport")) return;

	for (auto& LinkCtrl : LinkedSupportsControlled)
	{
		if (!IsValid(LinkCtrl.RotationSupport)) continue; //  do not check if there is no rotation support linked

		if (LinkCtrl.RotationSupport->GetComponentByClass(URotationBehaviorControlled::StaticClass())) continue; //  rot support has the good component, end of the check

		kPRINT_COLOR("Warning! Rotation Support " + UKismetSystemLibrary::GetDisplayName(LinkCtrl.RotationSupport) + " doesn't have the Controlled Rotation Behavior component!", FColor::Orange);

		LinkCtrl.RotationSupport = nullptr;
	}
}



// ======================================================
//                Interaction Interface
// ======================================================
void AMovSysWheel::Interaction_Implementation()
{
	if (bInControl) return;

	bInControl = true;

	for (auto& LinkCtrl : LinkedSupportsControlledVerified)
	{
		LinkCtrl.RotationControlledComponent->StartControlledRotation(LinkCtrl.ControlDatas);
	}

	WheelStartFeedback();
}

bool AMovSysWheel::CanInteract_Implementation()
{
	return !IsInteractableDisabled();
}

bool AMovSysWheel::IsInteractionHeavy_Implementation()
{
	return true;
}

void AMovSysWheel::InteractionHeavyUpdate_Implementation(FVector2D ControlValue)
{
	float ComputedControlValue = 0.0f;
	bool ControlClamp = true;

	if (bActivateAdvancedJoystickControl)
	{
		// TODO: implement the advanced joystick control
	}
	else
	{
		ComputedControlValue = ControlValue.X;
	}

	for (auto& LinkCtrl : LinkedSupportsControlledVerified)
	{
		ControlClamp = ControlClamp && LinkCtrl.RotationControlledComponent->UpdateControlledRotation(ComputedControlValue);
	}


	if (!ControlClamp)
	{
		WheelControlFeedback(ComputedControlValue);
	}
}

void AMovSysWheel::InteractionHeavyFinished_Implementation()
{
	if (!bInControl) return;

	bInControl = false;

	for (auto& LinkCtrl : LinkedSupportsControlledVerified)
	{
		LinkCtrl.RotationControlledComponent->StopControlledRotation();
	}

	WheelStopFeedback();
}

void AMovSysWheel::ForceReleaseInteractable()
{
	InteractionHeavyFinished_Implementation();
}
