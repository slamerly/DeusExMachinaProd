#include "MovSysHandle.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationSupport.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationBehaviorControlled.h"
#include "DeusExMachina/MovableSystem/Translation/TranslationSupport.h"
#include "DeusExMachina/MovableSystem/Translation/TranslationBehaviorControlled.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "DeusExMachina/MovableSystem/MovableObjectComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Defines.h"

AMovSysHandle::AMovSysHandle()
{
	PrimaryActorTick.bCanEverTick = false;

	//  initialize default components

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SceneRootComponent->SetupAttachment(GetRootComponent());

	PushDirArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Push Direction"));
	PushDirArrow->SetupAttachment(SceneRootComponent);
	PushDirArrow->SetHiddenInGame(false);

	MovableObjectComp = CreateDefaultSubobject<UMovableObjectComponent>(TEXT("Movable Object"));
}



// ======================================================
//                     Begin Play
// ======================================================
void AMovSysHandle::BeginPlay()
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


	//  apply visual options
	SceneRootComponent->SetHiddenInGame(bHideHandleVisual, true);
	PushDirArrow->SetHiddenInGame(bHideDirectionArrow);
}



#if WITH_EDITOR
// ======================================================
//             Check links validity (editor)
// ======================================================
void AMovSysHandle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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
void AMovSysHandle::Interaction_Implementation()
{
	//  check already in control
	if (bInControl) return;
	bInControl = true;

	//  visual feedback on blueprint
	HandleStartFeedback();

	//  retrieve the camera used (assume that the camera won't change during an interaction with a handle)
	Camera = UGameplayStatics::GetPlayerCameraManager(this, 0);

	//  start controlled behavior on linked supports
	for (auto& LinkCtrlR : LinkedRotSupportsControlledVerified)
	{
		LinkCtrlR.RotationControlledComponent->StartControlledRotation(LinkCtrlR.ControlDatas);
	}

	for (auto& LinkCtrlT : LinkedTransSupportsControlledVerified)
	{
		LinkCtrlT.TranslationControlledComponent->StartControlledTranslation(LinkCtrlT.ControlDatas);
	}

	//  broadcast OnMovSysHandleControlGained event
	OnMovSysHandleControlGained.Broadcast();
}


//          Update Interaction
// -------------------------------------------
void AMovSysHandle::InteractionHeavyUpdate_Implementation(FVector2D ControlValue, bool KeyboardInput)
{
	//  check in control
	if (!bInControl) return;

	//  compute control direction with the camera
	const FVector2D ControlDirection = UKismetMathLibrary::Conv_VectorToVector2D(
		FRotator{ 0.0f, Camera->GetCameraRotation().Yaw, 0.0f }
		.RotateVector(FVector{ ControlValue.Y, ControlValue.X, 0.0f })
	);

	//  compute control value with the handle push direction
	const FVector2D PushDirection = UKismetMathLibrary::Conv_VectorToVector2D(PushDirArrow->GetForwardVector());
	const float ComputedControlValue = UKismetMathLibrary::DotProduct2D(ControlDirection, PushDirection);

	//  visual feedback in blueprint
	HandleControlFeedback(ComputedControlValue);

	//  update controlled behavior on linked supports and check if this control cause clamp
	for (auto& LinkCtrlR : LinkedRotSupportsControlledVerified)
	{
		LinkCtrlR.RotationControlledComponent->UpdateControlledRotation(ComputedControlValue);
	}

	for (auto& LinkCtrlT : LinkedTransSupportsControlledVerified)
	{
		LinkCtrlT.TranslationControlledComponent->UpdateControlledTranslation(ComputedControlValue);
	}

	//  broadcast OnMovSysHandleControlUpdated event
	OnMovSysHandleControlUpdated.Broadcast(ComputedControlValue);
}


//          Finish Interaction
// -------------------------------------------
void AMovSysHandle::InteractionHeavyFinished_Implementation()
{
	//  check in control
	if (!bInControl) return;
	bInControl = false;

	//  visual feedback in blueprint
	HandleStopFeedback();

	//  finish controlled behavior on linked supports
	for (auto& LinkCtrlR : LinkedRotSupportsControlledVerified)
	{
		LinkCtrlR.RotationControlledComponent->StopControlledRotation();
	}

	for (auto& LinkCtrlT : LinkedTransSupportsControlledVerified)
	{
		LinkCtrlT.TranslationControlledComponent->StopControlledTranslation();
	}

	//  broadcast OnMovSysHandleControlLost event
	OnMovSysHandleControlLost.Broadcast();
}



// ======================================================
//            Interaction Interface (Getters)
// ======================================================
bool AMovSysHandle::CanInteract_Implementation()
{
	return !IsInteractableDisabled();
}

bool AMovSysHandle::IsInteractionHeavy_Implementation()
{
	return true;
}

bool AMovSysHandle::IsInteractableBothSides_Implementation()
{
	return true;
}

AActor* AMovSysHandle::GetInteractableFocusActor_Implementation()
{
	if (IsValid(ActorToFocus)) return ActorToFocus;

	kPRINT_ERROR("Called 'Get Focus Actor' on Handle " + UKismetSystemLibrary::GetDisplayName(this) + " but it has no valid Focus Actor!");
	return nullptr;
}



// ======================================================
//          Mov Sys Interactable Base (override)
// ======================================================
void AMovSysHandle::ForceReleaseInteractable()
{
	InteractionHeavyFinished_Implementation();
}
