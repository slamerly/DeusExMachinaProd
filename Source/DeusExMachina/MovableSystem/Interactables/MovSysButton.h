#pragma once

#include "CoreMinimal.h"
#include "MovSysInteractableBase.h"
#include "DeusExMachina/Interaction/Interactable.h"
#include "AutoRotInteractionDatas.h"
#include "DeusExMachina/MovableSystem/Rotation/StandardRotationDatas.h"
#include "DeusExMachina/MovableSystem/Translation/StandardTranslationDatas.h"
#include "AutoTransInteractionDatas.h"
#include "MovSysButton.generated.h"


class ARotationSupport;
class ATranslationSupport;
class URotationBehaviorAutomatic;
class URotationBehaviorStandard;
class UTranslationBehaviorAutomatic;
class UTranslationBehaviorStandard;


USTRUCT(BlueprintType, meta = (Tooltip = "Link a Rotation Support with the Rotation Behavior Automatic component to a MovSys Button."))
struct FAutoRotInteractionLink
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The Rotation Support you want to link. It must have the Rotation Behavior Automatic component."))
	ARotationSupport* RotationSupport{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The datas you want to link the support with. It will be used on this Rotation Support when using the button."))
	FAutoRotInteractionDatas InteractionDatas;

	URotationBehaviorAutomatic* RotationAutomaticComponent{ nullptr };
};


USTRUCT(BlueprintType, meta = (Tooltip = "Link a Rotation Support with the Rotation Behavior Standard component to a MovSys Button."))
struct FStandardRotInteractionLink
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The Rotation Support you want to link. It must have the Rotation Behavior Standard component."))
	ARotationSupport* RotationSupport{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The datas you want to link the support with. It will be used on this Rotation Support when using the button."))
	FStandardRotationDatas StandardDatas;

	URotationBehaviorStandard* RotationStandardComponent{ nullptr };
};


USTRUCT(BlueprintType, meta = (Tooltip = "Link a Translation Support with the Translation Behavior Automatic component to a MovSys Button."))
struct FAutoTransInteractionLink
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The Translation Support you want to link. It must have the Translation Behavior Automatic component."))
	ATranslationSupport* TranslationSupport{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The datas you want to link the support with. It will be used on this Translation Support when using the button."))
	FAutoTransInteractionDatas InteractionDatas;

	UTranslationBehaviorAutomatic* TranslationAutomaticComponent{ nullptr };
};


USTRUCT(BlueprintType, meta = (Tooltip = "Link a Translation Support with the Translation Behavior Standard component to a MovSys Button."))
struct FStandardTransInteractionLink
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The Translation Support you want to link. It must have the Translation Behavior Standard component."))
	ATranslationSupport* TranslationSupport{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The datas you want to link the support with. It will be used on this Translation Support when using the button."))
	FStandardTranslationDatas StandardDatas;

	UTranslationBehaviorStandard* TranslationStandardComponent{ nullptr };
};



UCLASS()
class DEUSEXMACHINA_API AMovSysButton : public AMovSysInteractableBase, public IInteractable
{
	GENERATED_BODY()

public:
	AMovSysButton();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;



// ======================================================
//                 Interactable Interface
// ======================================================
public:
	void Interaction_Implementation() override;
	bool CanInteract_Implementation() override;

	bool IsInteractionHeavy_Implementation() override;
	void InteractionHeavyUpdate_Implementation(FVector2D ControlValue) override;
	void InteractionHeavyFinished_Implementation() override;

	bool IsInteractableBothSides_Implementation() override;


// ======================================================
//          Button Visual Feedback (blueprint)
// ======================================================
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ButtonInteractFeedback();


// ======================================================
//              Button Internal Variables
// ======================================================
protected:
	UPROPERTY(BlueprintReadWrite)
	bool bPressed{ false };


// ======================================================
//                 Interaction Links
// ======================================================
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Link Rotation Supports", meta = (tooltip = "Rotation Supports with the Rotation Behavior Automatic you want to link to this button."))
	TArray<FAutoRotInteractionLink> LinkedRotSupportsAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Link Rotation Supports", meta = (tooltip = "Rotation Supports with the Rotation Behavior Standard you want to link to this button."))
	TArray<FStandardRotInteractionLink> LinkedRotSupportsStandard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Link Translation Supports", meta = (tooltip = "Translation Supports with the Translation Behavior Automatic you want to link to this button."))
	TArray<FAutoTransInteractionLink> LinkedTransSupportsAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Link Translation Supports", meta = (tooltip = "Translation Supports with the Translation Behavior Standard you want to link to this button."))
	TArray<FStandardTransInteractionLink> LinkedTransSupportsStandard;

protected:
	TArray<FAutoRotInteractionLink> LinkedRotSupportsAutomaticVerified;
	TArray<FStandardRotInteractionLink> LinkedRotSupportsStandardVerified;
	TArray<FAutoTransInteractionLink> LinkedTransSupportsAutomaticVerified;
	TArray<FStandardTransInteractionLink> LinkedTransSupportsStandardVerified;
};