#include "MovSysWheel.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationSupport.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationBehaviorControlled.h"
#include "DeusExMachina/MovableSystem/Translation/TranslationSupport.h"
#include "DeusExMachina/MovableSystem/Translation/TranslationBehaviorControlled.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DeusExMachina/MovableSystem/Rotation/AnglesUtils.h"
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

	//  check validity of supports linked and their controlled behavior components

	for (auto& LinkCtrlR : LinkedRotSupportsControlled)
	{
		if (!IsValid(LinkCtrlR.RotationSupport)) continue; //  do not check if there is no rotation support linked
		if (!LinkCtrlR.ControlDatas.IsDataValid()) continue; //  do not check if there is no datas linked

		if (!IsValid(LinkCtrlR.RotationSupport->GetComponentByClass<URotationBehaviorControlled>())) continue;

		LinkedRotSupportsControlledVerified.Add(FControlledRotInteractionLink{ LinkCtrlR.RotationSupport, LinkCtrlR.ControlDatas, LinkCtrlR.RotationSupport->GetComponentByClass<URotationBehaviorControlled>() });
	}

	for (auto& LinkCtrlT : LinkedTransSupportsControlled)
	{
		if (!IsValid(LinkCtrlT.TranslationSupport)) continue; //  do not check if there is no translation support linked
		if (!LinkCtrlT.ControlDatas.IsDataValid()) continue; //  do not check if there is no datas linked

		if (!IsValid(LinkCtrlT.TranslationSupport->GetComponentByClass<UTranslationBehaviorControlled>())) continue;

		LinkedTransSupportsControlledVerified.Add(FControlledTransInteractionLink{ LinkCtrlT.TranslationSupport, LinkCtrlT.ControlDatas, LinkCtrlT.TranslationSupport->GetComponentByClass<UTranslationBehaviorControlled>() });
	}
}



#if WITH_EDITOR
// ======================================================
//             Check links validity (editor)
// ======================================================
void AMovSysWheel::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//  check if the property changed is the rotation support or the translation support in a link
	if (!(PropertyChangedEvent.GetPropertyName() == FName("RotationSupport") || PropertyChangedEvent.GetPropertyName() == FName("TranslationSupport"))) return;


	//  check if supports linked have a controlled behavior component

	for (auto& LinkCtrlR : LinkedRotSupportsControlled)
	{
		if (!IsValid(LinkCtrlR.RotationSupport)) continue; //  do not check if there is no rotation support linked

		if (LinkCtrlR.RotationSupport->GetComponentByClass(URotationBehaviorControlled::StaticClass())) continue; //  rot support has the good component, end of the check

		kPRINT_COLOR("Warning! Rotation Support " + UKismetSystemLibrary::GetDisplayName(LinkCtrlR.RotationSupport) + " doesn't have the Controlled Rotation Behavior component!", FColor::Orange);

		LinkCtrlR.RotationSupport = nullptr;
	}

	for (auto& LinkCtrlT : LinkedTransSupportsControlled)
	{
		if (!IsValid(LinkCtrlT.TranslationSupport)) continue; //  do not check if there is no translation support linked

		if (LinkCtrlT.TranslationSupport->GetComponentByClass(UTranslationBehaviorControlled::StaticClass())) continue; //  trans support has the good component, end of the check

		kPRINT_COLOR("Warning! Translation Support " + UKismetSystemLibrary::GetDisplayName(LinkCtrlT.TranslationSupport) + " doesn't have the Controlled Translation Behavior component!", FColor::Orange);

		LinkCtrlT.TranslationSupport = nullptr;
	}
}
#endif // WITH_EDITOR



// ======================================================
//          Interaction Interface (Interaction)
// ======================================================

//          Start Interaction
// -------------------------------------------
void AMovSysWheel::Interaction_Implementation()
{
	//  check already in control
	if (bInControl) return;
	bInControl = true;

	//  start controlled behavior on linked supports
	for (auto& LinkCtrlR : LinkedRotSupportsControlledVerified)
	{
		LinkCtrlR.RotationControlledComponent->StartControlledRotation(LinkCtrlR.ControlDatas);
	}

	for (auto& LinkCtrlT : LinkedTransSupportsControlledVerified)
	{
		LinkCtrlT.TranslationControlledComponent->StartControlledTranslation(LinkCtrlT.ControlDatas);
	}

	//  visual feedback in blueprint
	WheelStartFeedback();
}


//          Update Interaction
// -------------------------------------------
void AMovSysWheel::InteractionHeavyUpdate_Implementation(FVector2D ControlValue, bool KeyboardInput)
{
	//  check in control
	if (!bInControl) return;

	//  compute control value depending of the mode
	float ComputedControlValue = 0.0f;
	if (bActivateAdvancedJoystickControl && !KeyboardInput)
	{
		ComputedControlValue = GetAdvancedJoystickControl(ControlValue);
	}
	else
	{
		ComputedControlValue = ControlValue.X;
	}

	//  update controlled behavior on linked supports and check if this control cause clamp
	bool ControlClamp = true;
	for (auto& LinkCtrlR : LinkedRotSupportsControlledVerified)
	{
		ControlClamp = ControlClamp && LinkCtrlR.RotationControlledComponent->UpdateControlledRotation(ComputedControlValue);
	}

	for (auto& LinkCtrlT : LinkedTransSupportsControlledVerified)
	{
		ControlClamp = ControlClamp && LinkCtrlT.TranslationControlledComponent->UpdateControlledTranslation(ComputedControlValue);
	}


	if (!ControlClamp) //  has the wheel been clamped in its control this frame
	{
		//  visual feedback in blueprint
		WheelControlFeedback(ComputedControlValue);
	}
}


//          Finish Interaction
// -------------------------------------------
void AMovSysWheel::InteractionHeavyFinished_Implementation()
{
	//  check in control
	if (!bInControl) return;
	bInControl = false;

	//  finish controlled behavior on linked supports
	for (auto& LinkCtrlR : LinkedRotSupportsControlledVerified)
	{
		LinkCtrlR.RotationControlledComponent->StopControlledRotation();
	}

	for (auto& LinkCtrlT : LinkedTransSupportsControlledVerified)
	{
		LinkCtrlT.TranslationControlledComponent->StopControlledTranslation();
	}

	//  visual feedback in blueprint
	WheelStopFeedback();
}



// ======================================================
//            Interaction Interface (Getters)
// ======================================================
bool AMovSysWheel::CanInteract_Implementation()
{
	return !IsInteractableDisabled();
}

bool AMovSysWheel::IsInteractionHeavy_Implementation()
{
	return true;
}
bool AMovSysWheel::IsInteractableBothSides_Implementation()
{
	return false;
}



// ======================================================
//          Mov Sys Interactable Base (override)
// ======================================================
void AMovSysWheel::ForceReleaseInteractable()
{
	InteractionHeavyFinished_Implementation();
}



// ======================================================
//               Advanced Joystick Control
// ======================================================
float AMovSysWheel::GetAdvancedJoystickControl(const FVector2D JoystickValue)
{
	if (JoystickValue.IsZero()) return 0.0f;

	//  compute the joystick angle
	const float NewJoystickAngle = UKismetMathLibrary::DegAcos(UKismetMathLibrary::DotProduct2D(JoystickValue, FVector2D::UnitY())) * 
		(JoystickValue.X < 0.0f ? -1.0f : 1.0f);

	//  compute the delta between the last joystick angle and the new one
	const float JoystickDelta = UAnglesUtils::SignedDeltaAngle(JoystickAngle, NewJoystickAngle);
	
	//  set control value if the delta is in the correct range
	float JoystickControl = 0.0f;
	if (FMath::Abs(JoystickDelta) > 5.0f && FMath::Abs(JoystickDelta) < 45.0f)
	{
		JoystickControl = FMath::Sign<float>(JoystickDelta);
	}

	JoystickAngle = NewJoystickAngle;
	if (JoystickControl != 0.0f)
	{
		//  if the control is valid this frame, replace last control and reset the forgot timer
		JoystickLastControl = JoystickControl;
		JoystickForgotTime = UKismetSystemLibrary::GetGameTimeInSeconds(this);

		return JoystickControl;
	}
	else
	{
		//  if the control is invalid this frame, return the last control if the forgot timer is low, else return 0
		if (UKismetSystemLibrary::GetGameTimeInSeconds(this) - JoystickForgotTime < 0.2f)
		{
			return JoystickLastControl;
		}
		else
		{
			return 0.0f;
		}
	}
}
