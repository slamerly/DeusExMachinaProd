#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ControlledRotationDatas.generated.h"


UCLASS()
class DEUSEXMACHINA_API UControlledRotationBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Speed", 
		meta = (Tooltip = "The speed of the rotation support when performing a Controlled Rotation.\n(In degrees per second)"))
	float RotationSpeed{ 60.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Startup",
		meta = (Tooltip = "The duration of the 'startup' phase of the Controlled Rotation.\nThe startup phase occurs when the rotation support starts rotating.\n(In seconds)"))
	float StartupDuration{ 0.8f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Startup",
		meta = (Tooltip = "The interpolation curve of the 'startup' phase of the Controlled Rotation.\n(Must be a normalized curve)"))
	UCurveFloat* StartupCurve{ nullptr };
};


USTRUCT(BlueprintType)
struct FControlledRotationDatas
{
	GENERATED_BODY()

	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Controlled Rotation for this link."))
	UControlledRotationBaseDatas* ControlledRotInteractionDatas{ nullptr };


	//  Override datas

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the speed value of the data asset for this link."))
	bool bOverrideSpeed{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Speed", meta = (EditCondition = "bOverrideSpeed", EditConditionHides),
		meta = (Tooltip = "The speed of the rotation support when performing a Controlled Rotation.\n(In degrees per second)"))
	float RotationSpeed{ 60.0f };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the startup value of the data asset for this link."))
	bool bOverrideStartup{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Startup", meta = (EditCondition = "bOverrideStartup", EditConditionHides),
		meta = (Tooltip = "The duration of the 'startup' phase of the Controlled Rotation.\nThe startup phase occurs when the rotation support starts rotating.\n(In seconds)"))
	float StartupDuration{ 0.8f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Startup", meta = (EditCondition = "bOverrideStartup", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the 'startup' phase of the Controlled Rotation.\n(Must be a normalized curve)"))
	UCurveFloat* StartupCurve{ nullptr };


	//  Getter functions
	bool IsDataValid();

	float GetRotationSpeed();
	float GetStartupDuration();
	UCurveFloat* GetStartupCurve();
};
