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
#include "Defines.h"

AMovSysHandle::AMovSysHandle()
{
	PrimaryActorTick.bCanEverTick = false;

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

	//  check link
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
//                Interaction Interface
// ======================================================
void AMovSysHandle::Interaction_Implementation()
{
	if (bInControl) return;
	bInControl = true;

	HandleStartFeedback();


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

bool AMovSysHandle::CanInteract_Implementation()
{
	return !IsInteractableDisabled();
}

bool AMovSysHandle::IsInteractionHeavy_Implementation()
{
	return true;
}

void AMovSysHandle::InteractionHeavyUpdate_Implementation(FVector2D ControlValue)
{
	if (!bInControl) return;

	const FVector2D ControlDirection = FVector2D(ControlValue.Y, ControlValue.X).GetSafeNormal();
	const FVector2D PushDirection = UKismetMathLibrary::Conv_VectorToVector2D(PushDirArrow->GetForwardVector());
	const float ComputedControlValue = UKismetMathLibrary::DotProduct2D(ControlDirection, PushDirection);

	HandleControlFeedback(ComputedControlValue);


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

void AMovSysHandle::InteractionHeavyFinished_Implementation()
{
	if (!bInControl) return;
	bInControl = false;

	HandleStopFeedback();


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

bool AMovSysHandle::IsInteractableBothSides_Implementation()
{
	return true;
}

void AMovSysHandle::ForceReleaseInteractable()
{
	InteractionHeavyFinished_Implementation();
}
