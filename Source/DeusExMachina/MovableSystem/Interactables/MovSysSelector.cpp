#include "MovSysSelector.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationSupport.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationBehaviorControlled.h"
#include "DeusExMachina/MovableSystem/Translation/TranslationSupport.h"
#include "DeusExMachina/MovableSystem/Translation/TranslationBehaviorControlled.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Defines.h"

AMovSysSelector::AMovSysSelector()
{
	PrimaryActorTick.bCanEverTick = false;
}



// ======================================================
//                     Begin Play
// ======================================================
void AMovSysSelector::BeginPlay()
{
	Super::BeginPlay();
	
	//  apply 'BeginPlayNotch'
	CurrentNotch = FMath::Clamp<int>(BeginPlayNotch, 0, NotchNumber - 1);


	//  check validity of supports linked and their controlled behavior components

	for (auto& LinkSelectR : LinkedRotSupports)
	{
		if (!IsValid(LinkSelectR.RotationSupportLink.RotationSupport)) continue; //  do not check if there is no rotation support linked
		if (!LinkSelectR.RotationSupportLink.ControlDatas.IsDataValid()) continue; //  do not check if there is no datas linked

		if (!IsValid(LinkSelectR.RotationSupportLink.RotationSupport->GetComponentByClass<URotationBehaviorControlled>())) continue;

		//  set the verified struct
		LinkedRotSupportsVerified.Add(
			FControlledRotSelectorLink{ 
				FControlledRotInteractionLink{ 
					LinkSelectR.RotationSupportLink.RotationSupport, 
					LinkSelectR.RotationSupportLink.ControlDatas, 
					LinkSelectR.RotationSupportLink.RotationSupport->GetComponentByClass<URotationBehaviorControlled>() 
				}, 
				LinkSelectR.SelectedNotch
			});
	}

	for (auto& LinkSelectT : LinkedTransSupports)
	{
		if (!IsValid(LinkSelectT.TranslationSupportLink.TranslationSupport)) continue; //  do not check if there is no translation support linked
		if (!LinkSelectT.TranslationSupportLink.ControlDatas.IsDataValid()) continue; //  do not check if there is no datas linked

		if (!IsValid(LinkSelectT.TranslationSupportLink.TranslationSupport->GetComponentByClass<UTranslationBehaviorControlled>())) continue;

		//  set the verified struct
		LinkedTransSupportsVerified.Add(
			FControlledTransSelectorLink{
				FControlledTransInteractionLink{
					LinkSelectT.TranslationSupportLink.TranslationSupport,
					LinkSelectT.TranslationSupportLink.ControlDatas,
					LinkSelectT.TranslationSupportLink.TranslationSupport->GetComponentByClass<UTranslationBehaviorControlled>()
				},
				LinkSelectT.SelectedNotch
			});
	}


	//  visual feedback in blueprint
	SelectorBeginPlayFeedback();
}



#if WITH_EDITOR
// ======================================================
//             Check links validity (editor)
// ======================================================
void AMovSysSelector::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//  check if the property changed is the rotation support or the translation support in a link
	if (!(PropertyChangedEvent.GetPropertyName() == FName("RotationSupport") || PropertyChangedEvent.GetPropertyName() == FName("TranslationSupport"))) return;


	//  check if supports linked have a controlled behavior component

	for (auto& LinkSelectR : LinkedRotSupports)
	{
		if (!IsValid(LinkSelectR.RotationSupportLink.RotationSupport)) continue; //  do not check if there is no rotation support linked

		if (LinkSelectR.RotationSupportLink.RotationSupport->GetComponentByClass(URotationBehaviorControlled::StaticClass())) continue; //  rot support has the good component, end of the check

		kPRINT_COLOR("Warning! Rotation Support " + UKismetSystemLibrary::GetDisplayName(LinkSelectR.RotationSupportLink.RotationSupport) + " doesn't have the Controlled Rotation Behavior component!", FColor::Orange);

		LinkSelectR.RotationSupportLink.RotationSupport = nullptr;
	}

	for (auto& LinkSelectT : LinkedTransSupports)
	{
		if (!IsValid(LinkSelectT.TranslationSupportLink.TranslationSupport)) continue; //  do not check if there is no translation support linked

		if (LinkSelectT.TranslationSupportLink.TranslationSupport->GetComponentByClass(UTranslationBehaviorControlled::StaticClass())) continue; //  trans support has the good component, end of the check

		kPRINT_COLOR("Warning! Translation Support " + UKismetSystemLibrary::GetDisplayName(LinkSelectT.TranslationSupportLink.TranslationSupport) + " doesn't have the Controlled Translation Behavior component!", FColor::Orange);

		LinkSelectT.TranslationSupportLink.TranslationSupport = nullptr;
	}
}
#endif // WITH_EDITOR



// ======================================================
//          Interaction Interface (Interaction)
// ======================================================

//          Start Interaction
// -------------------------------------------
void AMovSysSelector::Interaction_Implementation()
{
	//  check already in control
	if (bInControl) return;
	bInControl = true;


	//  broadcast OnMovSysSelectorControlGained event
	OnMovSysSelectorControlGained.Broadcast();

	//  visual feedback in blueprint
	SelectorStartFeedback();
}


//          Update Interaction
// -------------------------------------------
void AMovSysSelector::InteractionHeavyUpdate_Implementation(FVector2D ControlValue, bool KeyboardInput)
{
	//  check in control
	if (!bInControl) return;

	//  get control value (should add something to manage front and back of the selector surely)
	const float ControlValueX = ControlValue.X;

	//  preparation
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	const float ControlSign = FMath::Sign<float>(ControlValueX);

	if (ControlSign == 0.0f)
	{
		//  control value is 0
		if (NotchProgressTimer / SelectorFeedbackDuration > SelectorFeedbackSnap)
		{
			//  timer unfinished but snap is reached, so change notch
			NotchProgressTimer = 0.0f; //  reset timer to continue next frame with the new notch
			CurrentNotch += FMath::RoundToInt(LastControlSign);

			//  broadcast OnMovSysSelectorControlUpdated event
			OnMovSysSelectorControlUpdated.Broadcast(CurrentNotch);
		}
		else
		{
			LastControlSign = 0.0f;
			NotchProgressTimer = 0.0f;
		}
	}
	else if (ControlSign > 0.0f && CurrentNotch + 1 >= NotchNumber)
	{
		//  there is no higher notch
		LastControlSign = 0.0f;
		NotchProgressTimer = 0.0f;
	}
	else if (ControlSign < 0.0f && CurrentNotch - 1 < 0)
	{
		//  there is no lower notch
		LastControlSign = 0.0f;
		NotchProgressTimer = 0.0f;
	}
	else
	{
		if (ControlSign == LastControlSign)
		{
			//  good, continue timer
			NotchProgressTimer += DeltaTime;
			if (NotchProgressTimer >= SelectorFeedbackDuration)
			{
				//  timer finished, change notch
				NotchProgressTimer = 0.0f; //  reset timer to continue next frame with the new notch
				CurrentNotch += FMath::RoundToInt(ControlSign);

				//  broadcast OnMovSysSelectorControlUpdated event
				OnMovSysSelectorControlUpdated.Broadcast(CurrentNotch);
			}
		}
		else
		{
			//  reset timer
			NotchProgressTimer = 0.0f;
		}

		//  set last control sign
		LastControlSign = ControlSign;
	}


	//  visual feedback in blueprint
	SelectorUpdateFeedback(NotchProgressTimer / SelectorFeedbackDuration, FMath::RoundToInt(ControlSign));
}


//          Finish Interaction
// -------------------------------------------
void AMovSysSelector::InteractionHeavyFinished_Implementation()
{
	//  check in control
	if (!bInControl) return;
	bInControl = false;


	//  reset notch progress
	if (NotchProgressTimer / SelectorFeedbackDuration > SelectorFeedbackSnap)
	{
		//  timer unfinished but snap is reached, so change notch
		NotchProgressTimer = 0.0f;
		CurrentNotch += FMath::RoundToInt(LastControlSign);

		//  broadcast OnMovSysSelectorControlUpdated event
		OnMovSysSelectorControlUpdated.Broadcast(CurrentNotch);
	}
	else
	{
		NotchProgressTimer = 0.0f;
	}
	LastControlSign = 0.0f;

	//  visual feedback in blueprint
	SelectorUpdateFeedback(NotchProgressTimer / SelectorFeedbackDuration, 0);


	//  broadcast OnMovSysSelectorControlLost event
	OnMovSysSelectorControlLost.Broadcast();

	//  visual feedback in blueprint
	SelectorStopFeedback();
}



// ======================================================
//            Interaction Interface (Getters)
// ======================================================
bool AMovSysSelector::CanInteract_Implementation()
{
	return !IsInteractableDisabled();
}

bool AMovSysSelector::IsInteractionHeavy_Implementation()
{
	return true;
}

bool AMovSysSelector::IsInteractableBothSides_Implementation()
{
	return true;
}



// ======================================================
//          Mov Sys Interactable Base (override)
// ======================================================
void AMovSysSelector::ForceReleaseInteractable()
{
	InteractionHeavyFinished_Implementation();
}



// ======================================================
//                   Selector Getters
// ======================================================
int AMovSysSelector::GetCurrentNotch()
{
	return CurrentNotch;
}

void AMovSysSelector::GetLinkedSupportsCurrentlySelected(TArray<FControlledRotInteractionLink>& LinkedRotSupportsSelected, TArray<FControlledTransInteractionLink>& LinkedTransSupportsSelected)
{
	//  send all linked rot supports that have the current notch in their list of selected notch
	for (auto& LinkSelectR : LinkedRotSupportsVerified)
	{
		bool LinkSelected = false;
		for (int LinkNotch : LinkSelectR.SelectedNotch)
		{
			LinkSelected |= (CurrentNotch == LinkNotch);
		}
		if (LinkSelected) LinkedRotSupportsSelected.Add(LinkSelectR.RotationSupportLink);
	}

	//  send all linked trans supports that have the current notch in their list of selected notch
	for (auto& LinkSelectT : LinkedTransSupportsVerified)
	{
		bool LinkSelected = false;
		for (int LinkNotch : LinkSelectT.SelectedNotch)
		{
			LinkSelected |= (CurrentNotch == LinkNotch);
		}
		if (LinkSelected) LinkedTransSupportsSelected.Add(LinkSelectT.TranslationSupportLink);
	}
}

void AMovSysSelector::GetLinkedSupportsCurrentlyNotSelected(TArray<FControlledRotInteractionLink>& LinkedRotSupportsSelected, TArray<FControlledTransInteractionLink>& LinkedTransSupportsSelected)
{
	//  send all linked rot supports that doesn't have the current notch in their list of selected notch
	for (auto& LinkSelectR : LinkedRotSupportsVerified)
	{
		bool LinkSelected = false;
		for (int LinkNotch : LinkSelectR.SelectedNotch)
		{
			LinkSelected |= (CurrentNotch == LinkNotch);
		}
		if (!LinkSelected) LinkedRotSupportsSelected.Add(LinkSelectR.RotationSupportLink);
	}

	//  send all linked trans supports that doesn't have the current notch in their list of selected notch
	for (auto& LinkSelectT : LinkedTransSupportsVerified)
	{
		bool LinkSelected = false;
		for (int LinkNotch : LinkSelectT.SelectedNotch)
		{
			LinkSelected |= (CurrentNotch == LinkNotch);
		}
		if (!LinkSelected) LinkedTransSupportsSelected.Add(LinkSelectT.TranslationSupportLink);
	}
}
