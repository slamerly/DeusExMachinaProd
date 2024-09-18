#include "MovSysButton.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationSupport.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationBehaviorAutomatic.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationBehaviorStandard.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Defines.h"

AMovSysButton::AMovSysButton()
{
	PrimaryActorTick.bCanEverTick = true;
}



// ======================================================
//               Begin Play & Tick
// ======================================================

void AMovSysButton::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	for (auto& LinkAuto : LinkedSupportsAutomatic)
	{
		if (!IsValid(LinkAuto.RotationSupport)) continue; //  do not check if there is no rotation support linked
		if (!LinkAuto.InteractionDatas.IsDataValid()) continue; //  do not check if there is no datas linked

		if (!IsValid(LinkAuto.RotationSupport->GetComponentByClass<URotationBehaviorAutomatic>())) continue;

		LinkedSupportsAutomaticVerified.Add(FAutoRotInteractionLink{ LinkAuto.RotationSupport, LinkAuto.InteractionDatas, LinkAuto.RotationSupport->GetComponentByClass<URotationBehaviorAutomatic>() });
	}

	for (auto& LinkStand : LinkedSupportsStandard)
	{
		if (!IsValid(LinkStand.RotationSupport)) continue; //  do not check if there is no rotation support linked
		if (!LinkStand.StandardDatas.IsDataValid()) continue; //  do not check if there is no datas linked

		if (!IsValid(LinkStand.RotationSupport->GetComponentByClass<URotationBehaviorStandard>())) continue;

		LinkedSupportsStandardVerified.Add(FStandardRotInteractionLink{ LinkStand.RotationSupport, LinkStand.StandardDatas, LinkStand.RotationSupport->GetComponentByClass<URotationBehaviorStandard>() });
	}
}

void AMovSysButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



// ======================================================
//             Check links validity (editor)
// ======================================================

void AMovSysButton::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//  check if the property changed is the rotation support in a link
	if (PropertyChangedEvent.GetPropertyName() != FName("RotationSupport")) return;

	for (auto& LinkAuto : LinkedSupportsAutomatic)
	{
		if (!IsValid(LinkAuto.RotationSupport)) continue; //  do not check if there is no rotation support linked

		if (LinkAuto.RotationSupport->GetComponentByClass(URotationBehaviorAutomatic::StaticClass())) continue; //  rot support has the good component, end of the check
		
		kPRINT_COLOR("Warning! Rotation Support " + UKismetSystemLibrary::GetDisplayName(LinkAuto.RotationSupport) + " doesn't have the Automatic Rotation Behavior component!", FColor::Orange);

		LinkAuto.RotationSupport = nullptr;
	}

	for (auto& LinkStand : LinkedSupportsStandard)
	{
		if (!IsValid(LinkStand.RotationSupport)) continue; //  do not check if there is no rotation support linked

		if (LinkStand.RotationSupport->GetComponentByClass(URotationBehaviorStandard::StaticClass())) continue; //  rot support has the good component, end of the check

		kPRINT_COLOR("Warning! Rotation Support " + UKismetSystemLibrary::GetDisplayName(LinkStand.RotationSupport) + " doesn't have the Standard Rotation Behavior component!", FColor::Orange);

		LinkStand.RotationSupport = nullptr;
	}
}



// ======================================================
//                Interaction Interface
// ======================================================

void AMovSysButton::Interaction_Implementation()
{
	if (bPressed) return;

	ButtonInteractFeedback();


	for (auto& LinkAuto : LinkedSupportsAutomaticVerified)
	{
		LinkAuto.RotationAutomaticComponent->TriggerAutoRotInteraction(LinkAuto.InteractionDatas);
	}

	for (auto& LinkStand : LinkedSupportsStandardVerified)
	{
		LinkStand.RotationStandardComponent->StartStandardRotation(LinkStand.StandardDatas);
	}
}

bool AMovSysButton::CanInteract_Implementation()
{
	return !bPressed && !IsInteractableDisabled();
}



//  button is not a heavy interactable

bool AMovSysButton::IsInteractionHeavy_Implementation()
{
	return false;
}

void AMovSysButton::InteractionHeavyUpdate_Implementation(FVector2D ControlValue)
{
}

void AMovSysButton::InteractionHeavyFinished_Implementation()
{
}
