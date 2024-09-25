#pragma once

#include "CoreMinimal.h"
#include "MovSysInteractableBase.h"
#include "DeusExMachina/Interaction/Interactable.h"
#include "ControlledLinkStructs.h"
#include "MovSysHandle.generated.h"

class USceneComponent;
class UArrowComponent;
class UMovableObjectComponent;


UENUM(BlueprintType)
enum class EHandleMode : uint8
{
	ControlRotation = 0 UMETA(Tooltip = "Make this handle able to control a Rotation Support."),
	ControlTranslation = 1 UMETA(Tooltip = "Make this handle able to control a Translation Support.")
};



UCLASS()
class DEUSEXMACHINA_API AMovSysHandle : public AMovSysInteractableBase, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AMovSysHandle();

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

	virtual void ForceReleaseInteractable() override;


// ======================================================
//          Handle Visual Feedback (blueprint)
// ======================================================
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Handle Feedback")
	void HandleStartFeedback();

	UFUNCTION(BlueprintImplementableEvent, Category = "Handle Feedback")
	void HandleStopFeedback();

	UFUNCTION(BlueprintImplementableEvent, Category = "Handle Feedback")
	void HandleControlFeedback(float ControlValue);


// ======================================================
//             Handle Customization (editor)
// ======================================================
public:
	UPROPERTY(EditInstanceOnly, Category = "Options", meta = (Tooltip = "Hide the visual for handle in game."))
	bool bHideHandleVisual{ false };

	UPROPERTY(EditInstanceOnly, Category = "Options", meta = (Tooltip = "Hide the push direction arrow in game."))
	bool bHideDirectionArrow{ false };


// ======================================================
//               Handle Internal Variables
// ======================================================
protected:
	bool bInControl{ false };
	APlayerCameraManager* Camera{ nullptr };


// ======================================================
//                 Interaction Links
// ======================================================
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Link Support", meta = (tooltip = "Choose the type of object this handle can control."))
	EHandleMode HandleMode{ EHandleMode::ControlTranslation };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Link Support", meta = (EditCondition = "HandleMode == EHandleMode::ControlRotation", EditConditionHides),
		meta = (tooltip = "Rotation Support with the Rotation Behavior Controlled you want to control with this handle."))
	FControlledRotInteractionLink LinkedRotSupportControlled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Link Support", meta = (EditCondition = "HandleMode == EHandleMode::ControlTranslation", EditConditionHides),
		meta = (tooltip = "Translation Support with the Translation Behavior Controlled you want to control with this handle."))
	FControlledTransInteractionLink LinkedTransSupportControlled;

protected:
	bool bLinkValid{ false };


// ======================================================
//                  Default Components
// ======================================================
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USceneComponent* SceneRootComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UArrowComponent* PushDirArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movable Object Component")
	UMovableObjectComponent* MovableObjectComp;
};
