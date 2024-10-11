#include "MovSysButton.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationSupport.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationBehaviorAutomatic.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationBehaviorStandard.h"
#include "DeusExMachina/MovableSystem/Translation/TranslationSupport.h"
#include "DeusExMachina/MovableSystem/Translation/TranslationBehaviorAutomatic.h"
#include "DeusExMachina/MovableSystem/Translation/TranslationBehaviorStandard.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Defines.h"

AMovSysButton::AMovSysButton()
{
	PrimaryActorTick.bCanEverTick = true; //  blueprint need tick for visual
}



// ======================================================
//                     Begin Play
// ======================================================
void AMovSysButton::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	//  check validity of supports linked and their standard or automatic behavior component

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

	for (auto& LinkStandT : LinkedTransSupportsStandard)
	{
		if (!IsValid(LinkStandT.TranslationSupport)) continue; //  do not check if there is no translation support linked
		if (!LinkStandT.StandardDatas.IsDataValid()) continue; //  do not check if there is no datas linked

		if (!IsValid(LinkStandT.TranslationSupport->GetComponentByClass<UTranslationBehaviorStandard>())) continue;

		LinkedTransSupportsStandardVerified.Add(FStandardTransInteractionLink{ LinkStandT.TranslationSupport, LinkStandT.StandardDatas, LinkStandT.TranslationSupport->GetComponentByClass<UTranslationBehaviorStandard>() });
	}
}



#if WITH_EDITOR
// ======================================================
//             Check links validity (editor)
// ======================================================
void AMovSysButton::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//  check if the property changed is the rotation support or the translation support in a link
	if (!(PropertyChangedEvent.GetPropertyName() == FName("RotationSupport") || PropertyChangedEvent.GetPropertyName() == FName("TranslationSupport"))) return;

	//  check if supports linked have a standard or automatic behavior component

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

	for (auto& LinkStandT : LinkedTransSupportsStandard)
	{
		if (!IsValid(LinkStandT.TranslationSupport)) continue; //  do not check if there is no translation support linked

		if (LinkStandT.TranslationSupport->GetComponentByClass(UTranslationBehaviorStandard::StaticClass())) continue; //  trans support has the good component, end of the check

		kPRINT_COLOR("Warning! Translation Support " + UKismetSystemLibrary::GetDisplayName(LinkStandT.TranslationSupport) + " doesn't have the Standard Translation Behavior component!", FColor::Orange);

		LinkStandT.TranslationSupport = nullptr;
	}
}
#endif // WITH_EDITOR



// ======================================================
//           Interaction Interface (Interaction)
// ======================================================
void AMovSysButton::Interaction_Implementation()
{
	//  check already pressed
	if (bPressed) return;

	//  visual feedback in blueprint
	ButtonInteractFeedback();


	//  trigger standard or automatic behavior on linked supports
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

	for (auto& LinkStandT : LinkedTransSupportsStandardVerified)
	{
		LinkStandT.TranslationStandardComponent->StartStandardTranslation(LinkStandT.StandardDatas);
	}

	//  broadcast OnMovSysButtonUsed event
	OnMovSysButtonUsed.Broadcast();
}

bool AMovSysButton::CanInteract_Implementation()
{
	return !bPressed && !IsInteractableDisabled();
}



// ======================================================
//        Interaction Interface (Heavy and other)
// ======================================================
bool AMovSysButton::IsInteractionHeavy_Implementation()
{
	return false;
}

void AMovSysButton::InteractionHeavyUpdate_Implementation(FVector2D ControlValue, bool KeyboardInput)
{
}

void AMovSysButton::InteractionHeavyFinished_Implementation()
{
}

bool AMovSysButton::IsInteractableBothSides_Implementation()
{
	return false;
}

AActor* AMovSysButton::GetInteractableFocusActor_Implementation()
{
	if (IsValid(ActorToFocus)) return ActorToFocus;

	kPRINT_ERROR("Called 'Get Focus Actor' on Button " + UKismetSystemLibrary::GetDisplayName(this) + " but it has no valid Focus Actor!");
	return nullptr;
}
