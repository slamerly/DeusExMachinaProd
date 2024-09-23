#include "MovSysButton.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationSupport.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationBehaviorAutomatic.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationBehaviorStandard.h"
#include "DeusExMachina/MovableSystem/Translation/TranslationSupport.h"
#include "DeusExMachina/MovableSystem/Translation/TranslationBehaviorAutomatic.h"
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

	for (auto& LinkAutoR : LinkedRotSupportsAutomatic)
	{
		if (!IsValid(LinkAutoR.RotationSupport)) continue; //  do not check if there is no rotation support linked
		if (!LinkAutoR.InteractionDatas.IsDataValid()) continue; //  do not check if there is no datas linked

		if (!IsValid(LinkAutoR.RotationSupport->GetComponentByClass<URotationBehaviorAutomatic>())) continue;

		LinkedRotSupportsAutomaticVerified.Add(FAutoRotInteractionLink{ LinkAutoR.RotationSupport, LinkAutoR.InteractionDatas, LinkAutoR.RotationSupport->GetComponentByClass<URotationBehaviorAutomatic>() });
	}

	for (auto& LinkStandR : LinkedRotSupportsStandard)
	{
		if (!IsValid(LinkStandR.RotationSupport)) continue; //  do not check if there is no rotation support linked
		if (!LinkStandR.StandardDatas.IsDataValid()) continue; //  do not check if there is no datas linked

		if (!IsValid(LinkStandR.RotationSupport->GetComponentByClass<URotationBehaviorStandard>())) continue;

		LinkedRotSupportsStandardVerified.Add(FStandardRotInteractionLink{ LinkStandR.RotationSupport, LinkStandR.StandardDatas, LinkStandR.RotationSupport->GetComponentByClass<URotationBehaviorStandard>() });
	}

	for (auto& LinkAutoT : LinkedTransSupportsAutomatic)
	{
		if (!IsValid(LinkAutoT.TranslationSupport)) continue; //  do not check if there is no translation support linked
		if (!LinkAutoT.InteractionDatas.IsDataValid()) continue; //  do not check if there is no datas linked

		if (!IsValid(LinkAutoT.TranslationSupport->GetComponentByClass<UTranslationBehaviorAutomatic>())) continue;

		LinkedTransSupportsAutomaticVerified.Add(FAutoTransInteractionLink{ LinkAutoT.TranslationSupport, LinkAutoT.InteractionDatas, LinkAutoT.TranslationSupport->GetComponentByClass<UTranslationBehaviorAutomatic>() });
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

	for (auto& LinkAutoR : LinkedRotSupportsAutomatic)
	{
		if (!IsValid(LinkAutoR.RotationSupport)) continue; //  do not check if there is no rotation support linked

		if (LinkAutoR.RotationSupport->GetComponentByClass(URotationBehaviorAutomatic::StaticClass())) continue; //  rot support has the good component, end of the check
		
		kPRINT_COLOR("Warning! Rotation Support " + UKismetSystemLibrary::GetDisplayName(LinkAutoR.RotationSupport) + " doesn't have the Automatic Rotation Behavior component!", FColor::Orange);

		LinkAutoR.RotationSupport = nullptr;
	}

	for (auto& LinkStandR : LinkedRotSupportsStandard)
	{
		if (!IsValid(LinkStandR.RotationSupport)) continue; //  do not check if there is no rotation support linked

		if (LinkStandR.RotationSupport->GetComponentByClass(URotationBehaviorStandard::StaticClass())) continue; //  rot support has the good component, end of the check

		kPRINT_COLOR("Warning! Rotation Support " + UKismetSystemLibrary::GetDisplayName(LinkStandR.RotationSupport) + " doesn't have the Standard Rotation Behavior component!", FColor::Orange);

		LinkStandR.RotationSupport = nullptr;
	}

	for (auto& LinkAutoT : LinkedTransSupportsAutomatic)
	{
		if (!IsValid(LinkAutoT.TranslationSupport)) continue; //  do not check if there is no translation support linked

		if (LinkAutoT.TranslationSupport->GetComponentByClass(UTranslationBehaviorAutomatic::StaticClass())) continue; //  trans support has the good component, end of the check

		kPRINT_COLOR("Warning! Translation Support " + UKismetSystemLibrary::GetDisplayName(LinkAutoT.TranslationSupport) + " doesn't have the Automatic Translation Behavior component!", FColor::Orange);

		LinkAutoT.TranslationSupport = nullptr;
	}
}



// ======================================================
//                Interaction Interface
// ======================================================

void AMovSysButton::Interaction_Implementation()
{
	if (bPressed) return;

	ButtonInteractFeedback();


	for (auto& LinkAutoR : LinkedRotSupportsAutomaticVerified)
	{
		LinkAutoR.RotationAutomaticComponent->TriggerAutoRotInteraction(LinkAutoR.InteractionDatas);
	}

	for (auto& LinkStandR : LinkedRotSupportsStandardVerified)
	{
		LinkStandR.RotationStandardComponent->StartStandardRotation(LinkStandR.StandardDatas);
	}

	for (auto& LinkAutoT : LinkedTransSupportsAutomaticVerified)
	{
		LinkAutoT.TranslationAutomaticComponent->TriggerAutoTransInteraction(LinkAutoT.InteractionDatas);
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
