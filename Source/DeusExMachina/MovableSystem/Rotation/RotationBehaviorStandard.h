#pragma once

#include "CoreMinimal.h"
#include "RotationBehaviorBase.h"
#include "RotationBehaviorStandard.generated.h"


UCLASS(ClassGroup = (MovableSystem), meta = (BlueprintSpawnableComponent, DisplayName = "Rotation Behavior Standard", Tooltip = "Component to add to a Rotation Support if you want it to be able to perform a standard rotation."))
class DEUSEXMACHINA_API URotationBehaviorStandard : public URotationBehaviorBase
{
	GENERATED_BODY()

public:	
	URotationBehaviorStandard();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



// ======================================================
//              Control Standard Rotation
// ======================================================
public:
	/**
	* Function to call to start the standard rotation of this component.
	* @param	Datas			The standard rotation datas
	* @param	bForceStart		(optionnal) Force the standard rotation to start even if the component is already performing a standard rotation
	*/
	UFUNCTION(BlueprintCallable, Category = "Standard Rotation")
	void StartStandardRotation(struct FStandardRotationDatas Datas, bool bForceStart = false);

	/**
	* Function to call to stop every movement of this component.
	*/
	UFUNCTION(BlueprintCallable, Category = "Standard Rotation")
	void CancelStandardRotation();


// ======================================================
//                   Helper Functions
// ======================================================
protected:
	bool IsStandardRotValid(struct FStandardRotationDatas Datas);


// ======================================================
//         Standard Rotation Internal Variables
// ======================================================
protected:
	bool bCurrentlyRotating{ false };

	float RotationDuration{ 0.0f };
	float RotationTimer{ 0.0f };
	float RotationAngle{ 0.0f };
	UCurveFloat* RotationCurve{ nullptr };
	int DestinationAngle{ 0 };

	float LastFrameRotAngle{ 0.0f };
};
