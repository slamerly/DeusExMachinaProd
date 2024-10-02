#pragma once

#include "CoreMinimal.h"
#include "RotationBehaviorBase.h"
#include "ControlledRotationDatas.h"
#include "RotationBehaviorControlled.generated.h"


UENUM()
enum class EControlledRotationState : uint8
{
	Inactive = 0,
	ControlStartup = 1,
	Control = 2,
	Snap = 3
};


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FControlledRotationStart, URotationBehaviorControlled, OnControlledRotationStart, const FControlledRotationDatas&, Datas);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FControlledRotationStop, URotationBehaviorControlled, OnControlledRotationStop, bool, Snap);


UCLASS(ClassGroup = (MovableSystem), meta = (BlueprintSpawnableComponent, DisplayName = "Rotation Behavior Controlled", Tooltip = "Component to add to a Rotation Support if you want it to be able to be controlled by a Wheel or a Handle."))
class DEUSEXMACHINA_API URotationBehaviorControlled : public URotationBehaviorBase
{
	GENERATED_BODY()

public:
	URotationBehaviorControlled();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



// ======================================================
//              Control Controlled Rotation
// ======================================================
public:
	/**
	* Function to call when an interactable linked to this behavior has gained control by the player.
	* @param	Datas	The control datas.
	* @return			True if the control was successfully gained.
	*/
	UFUNCTION(BlueprintCallable, Category = "Controlled Rotation")
	bool StartControlledRotation(FControlledRotationDatas Datas);
	
	/**
	* Function to call when an interactable linked to this behavior has lost control from the player.
	* @param	DontTriggerSnap		(optionnal) Force this behavior to not trigger the snap on release, even if the support has snap enabled.
	*/
	UFUNCTION(BlueprintCallable, Category = "Controlled Rotation")
	void StopControlledRotation(bool DontTriggerSnap = false);
	
	/**
	* Function to call when an interactable controlling this behavior receive update from the player.
	* @param	ControlValue	The control value for this update frame.
	* @return					True if this behavior encountered clamp on this update frame.
	*/
	UFUNCTION(BlueprintCallable, Category = "Controlled Rotation")
	bool UpdateControlledRotation(float ControlValue);


// ======================================================
//                   Helper Functions
// ======================================================
protected:
	bool IsControlledRotValid(FControlledRotationDatas Datas);
	bool IsStartupValid(FControlledRotationDatas Datas);


// ======================================================
//                   Delegate Events
// ======================================================
public:
	/** Called when the player start using the controlled rotation on this component. */
	UPROPERTY(BlueprintAssignable, Category = "Controlled Rotation|Events")
	FControlledRotationStart OnControlledRotationStart;

	/** Called when the the player stop using the controlled rotation on this component. */
	UPROPERTY(BlueprintAssignable, Category = "Controlled Rotation|Events")
	FControlledRotationStop OnControlledRotationStop;


// ======================================================
//         Controlled Rotation Internal Variables
// ======================================================
protected:
	EControlledRotationState CurrentState{ EControlledRotationState::Inactive };
	FControlledRotationDatas CurrentDatas;

	// =====================
	//  Controlled Speed
	// =====================
	float RotationSpeed{ 0.0f };

	// =====================
	//  Last input direction
	// =====================
	int LastInputedDirection{ 0 };
	float LastInputedDirectionTimer{ 0.0f };


	// =====================
	//  Startup
	// =====================
	bool bUseStartup{ false };
	float StartupDuration{ 0.0f };
	float StartupTimer{ 0.0f };
	UCurveFloat* StartupCurve{ nullptr };


	// =====================
	//  Snap
	// =====================
	float SnapAngleStart{ 0.0f };
	float SnapAngleDest{ 0.0f };
	float SnapDuration{ 0.0f };
	float SnapTimer{ 0.0f };
	UCurveFloat* SnapCurve{ nullptr };
};
