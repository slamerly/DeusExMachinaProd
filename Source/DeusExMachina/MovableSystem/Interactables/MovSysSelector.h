#pragma once

#include "CoreMinimal.h"
#include "MovSysInteractableBase.h"
#include "DeusExMachina/Interaction/Interactable.h"
#include "ControlledLinkStructs.h"
#include "MovSysSelector.generated.h"


USTRUCT(BlueprintType, meta = (Tooltip = "Link a Rotation Support to a selector and choose at which notch it will be selected."))
struct FControlledRotSelectorLink
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The actual link."))
	FControlledRotInteractionLink RotationSupportLink;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "At which notch of the Selector this Rotation Support will be selected."))
	TArray<int> SelectedNotch;
};


USTRUCT(BlueprintType, meta = (Tooltip = "Link a Translation Support to a selector and choose at which notch it will be selected."))
struct FControlledTransSelectorLink
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The actual link."))
	FControlledTransInteractionLink TranslationSupportLink;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "At which notch of the Selector this Translation Support will be selected."))
	TArray<int> SelectedNotch;
};


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FMovSysSelectorControlGain, AMovSysSelector, OnMovSysSelectorControlGained);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FMovSysSelectorControlLost, AMovSysSelector, OnMovSysSelectorControlLost);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FMovSysSelectorControlUpdate, AMovSysSelector, OnMovSysSelectorControlUpdated, int, Selection);


UCLASS()
class DEUSEXMACHINA_API AMovSysSelector : public AMovSysInteractableBase, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AMovSysSelector();

protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
public:
	/** Called when a value is changed on this actor in the editor */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR



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
//                  Selector Getters
// ======================================================
public:
	/** Return the current selected notch of the selector. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Moving System Selector")
	int GetCurrentNotch();

	/** Retrieve the linked rotation and translation supports currently selected. */
	UFUNCTION(BlueprintCallable, Category = "Moving System Selector")
	void GetLinkedSupportsCurrentlySelected(TArray<FControlledRotInteractionLink>& LinkedRotSupportsSelected, TArray<FControlledTransInteractionLink>& LinkedTransSupportsSelected);

	/** Retrieve the linked rotation and translation supports currently not selected. */
	UFUNCTION(BlueprintCallable, Category = "Moving System Selector")
	void GetLinkedSupportsCurrentlyNotSelected(TArray<FControlledRotInteractionLink>& LinkedRotSupportsSelected, TArray<FControlledTransInteractionLink>& LinkedTransSupportsSelected);



// ======================================================
//                   Delegate Events
// ======================================================
public:
	/** Called when this selector gained control by the player. */
	UPROPERTY(BlueprintAssignable, Category = "Moving System Selector|Events")
	FMovSysSelectorControlGain OnMovSysSelectorControlGained;

	/** Called when this selector lost control by the player. */
	UPROPERTY(BlueprintAssignable, Category = "Moving System Selector|Events")
	FMovSysSelectorControlLost OnMovSysSelectorControlLost;

	/** Called when this selector control is updated by the player. */
	UPROPERTY(BlueprintAssignable, Category = "Moving System Selector|Events")
	FMovSysSelectorControlUpdate OnMovSysSelectorControlUpdated;



// ======================================================
//          Selector Visual Feedback (blueprint)
// ======================================================
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Selector Feedback")
	void SelectorStartFeedback();

	UFUNCTION(BlueprintImplementableEvent, Category = "Selector Feedback")
	void SelectorStopFeedback();

	UFUNCTION(BlueprintImplementableEvent, Category = "Selector Feedback")
	void SelectorBeginPlayFeedback();

	UFUNCTION(BlueprintImplementableEvent, Category = "Selector Feedback")
	void SelectorUpdateFeedback(float NotchProgressPercent, int NotchProgressTo);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selector Feedback", meta = (Tooltip = "The time the player must keep the joystick in a direction for the selector to change notch.\n(In seconds)"))
	float SelectorFeedbackDuration{ 1.5f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selector Feedback", meta = (Tooltip = "The interpolation curve of the movement of the selector from one notch to another.\n(Must be a normalized curve)"))
	UCurveFloat* SelectorFeedbackCurve{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selector Feedback", meta = (Tooltip = "How much close to the next notch the selector need to be for it to snap on the next notch and not the previous.\n(Between 0 and 1)"))
	float SelectorFeedbackSnap{ 0.9f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selector Feedback", meta = (Tooltip = "The amplitude of the visual selector.\n(In degrees)"))
	float SelectorFeedbackAmplitude{ 60.0f };



// ======================================================
//              Selector Internal Variables
// ======================================================
protected:
	bool bInControl{ false };

	int CurrentNotch{ 0 };
	float NotchProgressTimer{ 0.0f };
	float LastControlSign{ 0.0f };


// ======================================================
//              Interaction Datas & Links
// ======================================================
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selector Links", meta = (Tooltip = "The number of notch this selector have", ClamMin = 1))
	int NotchNumber{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selector Links", meta = (Tooltip = "The notch index this selector starts on at begin play.", ClamMin = 0))
	int BeginPlayNotch{ 0 };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selector Links|Rotation Supports", meta = (tooltip = "Rotation Supports with the Rotation Behavior Controlled you want to link to this selector."))
	TArray<FControlledRotSelectorLink> LinkedRotSupports;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selector Links|Translation Supports", meta = (tooltip = "Translation Supports with the Translation Behavior Controlled you want to link to this selector."))
	TArray<FControlledTransSelectorLink> LinkedTransSupports;

protected:
	TArray<FControlledRotSelectorLink> LinkedRotSupportsVerified;
	TArray<FControlledTransSelectorLink> LinkedTransSupportsVerified;
};
