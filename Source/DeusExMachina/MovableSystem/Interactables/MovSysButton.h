#pragma once

#include "CoreMinimal.h"
#include "MovSysInteractableBase.h"
#include "DeusExMachina/Interaction/Interactable.h"
#include "AutoRotInteractionDatas.h"
#include "MovSysButton.generated.h"


class ARotationSupport;
class URotationBehaviorAutomatic;


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



UCLASS()
class DEUSEXMACHINA_API AMovSysButton : public AMovSysInteractableBase, public IInteractable
{
	GENERATED_BODY()

public:
	AMovSysButton();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (tooltip = "Rotation Supports with the Rotation Behavior Automatic you want to link to this button."))
	TArray<FAutoRotInteractionLink> LinkedSupportsAutomatic;

protected:
	TArray<FAutoRotInteractionLink> LinkedSupportsAutomaticVerified;

};