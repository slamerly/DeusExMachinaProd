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

	//  check validity of support linked and their controlled behavior components

	switch (HandleMode)
	{
	case EHandleMode::ControlRotation:
		if (!IsValid(LinkedRotSupportControlled.RotationSupport)) break; //  do not check if there is no rotation support linked
		if (!LinkedRotSupportControlled.ControlDatas.IsDataValid()) break; //  do not check if there is no datas linked

		if (!IsValid(LinkedRotSupportControlled.RotationSupport->GetComponentByClass<URotationBehaviorControlled>())) break;

		LinkedRotSupportControlled.RotationControlledComponent = LinkedRotSupportControlled.RotationSupport->GetComponentByClass<URotationBehaviorControlled>();
		bLinkValid = true;
		break;

	case EHandleMode::ControlTranslation:
		if (!IsValid(LinkedTransSupportControlled.TranslationSupport)) break; //  do not check if there is no translation support linked
		if (!LinkedTransSupportControlled.ControlDatas.IsDataValid()) break; //  do not check if there is no datas linked

		if (!IsValid(LinkedTransSupportControlled.TranslationSupport->GetComponentByClass<UTranslationBehaviorControlled>())) break;

		LinkedTransSupportControlled.TranslationControlledComponent = LinkedTransSupportControlled.TranslationSupport->GetComponentByClass<UTranslationBehaviorControlled>();
		bLinkValid = true;
		break;
	}


	//  apply visual options
	SceneRootComponent->SetHiddenInGame(bHideHandleVisual, true);
	PushDirArrow->SetHiddenInGame(bHideDirectionArrow);
}



// ======================================================
//             Check links validity (editor)
// ======================================================
void AMovSysHandle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//  check if the property changed is the rotation support or the translation support in a link
	if (!(PropertyChangedEvent.GetPropertyName() == FName("RotationSupport") || PropertyChangedEvent.GetPropertyName() == FName("TranslationSupport"))) return;


	//  check if support linked have a controlled behavior component

	switch (HandleMode)
	{
	case EHandleMode::ControlRotation:
		if (!IsValid(LinkedRotSupportControlled.RotationSupport)) break; //  do not check if there is no rotation support linked

		if (IsValid(LinkedRotSupportControlled.RotationSupport->GetComponentByClass<URotationBehaviorControlled>())) break; //  component valid, check done

		LinkedRotSupportControlled.RotationSupport = nullptr;

		kPRINT_COLOR("Warning! Rotation Support " + UKismetSystemLibrary::GetDisplayName(LinkedRotSupportControlled.RotationSupport) + " doesn't have the Controlled Rotation Behavior component!", FColor::Orange);
		break;

	case EHandleMode::ControlTranslation:
		if (!IsValid(LinkedTransSupportControlled.TranslationSupport)) break; //  do not check if there is no translation support linked

		if (IsValid(LinkedTransSupportControlled.TranslationSupport->GetComponentByClass<UTranslationBehaviorControlled>())) break; //  component valid, check done

		LinkedTransSupportControlled.TranslationSupport = nullptr;

		kPRINT_COLOR("Warning! Translation Support " + UKismetSystemLibrary::GetDisplayName(LinkedTransSupportControlled.TranslationSupport) + " doesn't have the Controlled Translation Behavior component!", FColor::Orange);
		break;
	}
}



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

	//  start controlled behavior on linked support
	if (!bLinkValid) return;
	switch (HandleMode)
	{
	case EHandleMode::ControlRotation:
		LinkedRotSupportControlled.RotationControlledComponent->StartControlledRotation(LinkedRotSupportControlled.ControlDatas);
		break;

	case EHandleMode::ControlTranslation:
		LinkedTransSupportControlled.TranslationControlledComponent->StartControlledTranslation(LinkedTransSupportControlled.ControlDatas);
		break;
	}
}


//          Update Interaction
// -------------------------------------------
void AMovSysHandle::InteractionHeavyUpdate_Implementation(FVector2D ControlValue)
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

	//  update controlled behavior on linked support
	if (!bLinkValid) return;
	switch (HandleMode)
	{
	case EHandleMode::ControlRotation:
		LinkedRotSupportControlled.RotationControlledComponent->UpdateControlledRotation(ComputedControlValue);
		break;

	case EHandleMode::ControlTranslation:
		LinkedTransSupportControlled.TranslationControlledComponent->UpdateControlledTranslation(ComputedControlValue);
		break;
	}
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

	//  finish controlled behavior on linked support
	if (!bLinkValid) return;
	switch (HandleMode)
	{
	case EHandleMode::ControlRotation:
		LinkedRotSupportControlled.RotationControlledComponent->StopControlledRotation();
		break;

	case EHandleMode::ControlTranslation:
		LinkedTransSupportControlled.TranslationControlledComponent->StopControlledTranslation();
		break;
	}
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



// ======================================================
//          Mov Sys Interactable Base (override)
// ======================================================
void AMovSysHandle::ForceReleaseInteractable()
{
	InteractionHeavyFinished_Implementation();
}
