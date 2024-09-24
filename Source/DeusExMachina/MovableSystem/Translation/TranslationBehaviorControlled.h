#pragma once

#include "CoreMinimal.h"
#include "TranslationBehaviorBase.h"
#include "ControlledTranslationDatas.h"
#include "TranslationBehaviorControlled.generated.h"


UENUM()
enum class EControlledTranslationState : uint8
{
	Inactive = 0,
	ControlStartup = 1,
	Control = 2,
	Snap = 3
};


UCLASS(ClassGroup = (MovableSystem), meta = (BlueprintSpawnableComponent, DisplayName = "Translation Behavior Controlled", Tooltip = "Component to add to a Translation Support if you want it to be able to be controlled by a Wheel or a Handle.\nNote that Translation Supports with this component will only use the two first points of their splines (index 0 & 1)."))
class DEUSEXMACHINA_API UTranslationBehaviorControlled : public UTranslationBehaviorBase
{
	GENERATED_BODY()

public:	
	UTranslationBehaviorControlled();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


// ======================================================
//             Control Controlled Translation
// ======================================================
public:
	/**
	* Function to call when an interactable linked to this behavior has gained control by the player.
	* @param	Datas	The control datas.
	* @return			True if the control was successfully gained.
	*/
	bool StartControlledTranslation(struct FControlledTranslationDatas Datas);

	/**
	* Function to call when an interactable linked to this behavior has lost control from the player.
	* @param	DontTriggerSnap		Optionnal param to force this behavior to not trigger the snap on release, even if the component has snap enabled.
	*/
	void StopControlledTranslation(bool DontTriggerSnap = false);

	/**
	* Function to call when an interactable controlling this behavior receive update from the player.
	* @param	ControlValue	The control value for this update frame.
	* @return					True if this behavior encountered clamp on this update frame.
	*/
	bool UpdateControlledTranslation(float ControlValue);


// ======================================================
//                   Helper Functions
// ======================================================
protected:
	bool IsControlledTransValid(FControlledTranslationDatas Datas);
	bool IsStartupValid(FControlledTranslationDatas Datas);


// ======================================================
//         Controlled Translation Internal Variables
// ======================================================
protected:
	EControlledTranslationState CurrentState{ EControlledTranslationState::Inactive };

	FControlledTranslationDatas CurrentDatas;

	float TranslationSpeed{ 0.0f };

	bool bUseStartup{ false };
	float StartupDuration{ 0.0f };
	float StartupTimer{ 0.0f };
	UCurveFloat* StartupCurve{ nullptr };
};
