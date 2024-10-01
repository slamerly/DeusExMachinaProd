#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Snap",
		meta = (Tooltip = "When searching for snap, ignore clamp on the Rotation Support."))
	bool bSnapIgnoreClamp{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Snap",
		meta = (Tooltip = "When searching for snap, ignore ranges on snap values."))
	bool bSnapIgnoreRanges{ false };
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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the startup values of the data asset for this link."))
	bool bOverrideStartup{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Startup", meta = (EditCondition = "bOverrideStartup", EditConditionHides),
		meta = (Tooltip = "The duration of the 'startup' phase of the Controlled Rotation.\nThe startup phase occurs when the rotation support starts rotating.\n(In seconds)"))
	float StartupDuration{ 0.8f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Startup", meta = (EditCondition = "bOverrideStartup", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the 'startup' phase of the Controlled Rotation.\n(Must be a normalized curve)"))
	UCurveFloat* StartupCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the snap values of the data asset for this link."))
	bool bOverrideSnap{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Snap", meta = (EditCondition = "bOverrideSnap", EditConditionHides),
		meta = (Tooltip = "When searching for snap, ignore clamp on the Rotation Support"))
	bool bSnapIgnoreClamp{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Rotation Snap", meta = (EditCondition = "bOverrideSnap", EditConditionHides),
		meta = (Tooltip = "When searching for snap, ignore ranges on snap values"))
	bool bSnapIgnoreRanges{ false };


	//  Getter functions
	bool IsDataValid() const;

	float GetRotationSpeed() const;
	float GetStartupDuration() const;
	UCurveFloat* GetStartupCurve() const;
	bool GetSnapIgnoreClamp() const;
	bool GetSnapIgnoreRanges() const;
};


UCLASS()
class DEUSEXMACHINA_API UControlledRotationDatasGet : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	* Get the 'RotationSpeed' value, either the base one or the overriden one if needed out of this struct.
	* @return	The speed of the rotation support when performing a Controlled Rotation.
				(In degrees per second)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Rotation Datas", meta = (ReturnDisplayName = "RotationSpeed"))
	static float GetRotationSpeed(const FControlledRotationDatas& Datas);

	/**
	* Get the 'StartupDuration' value, either the base one or the overriden one if needed out of this struct.
	* @return	The duration of the 'startup' phase of the Controlled Rotation.
				The startup phase occurs when the rotation support starts rotating.
				(In seconds)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Rotation Datas", meta = (ReturnDisplayName = "StartupDuration"))
	static float GetStartupDuration(const FControlledRotationDatas& Datas);

	/**
	* Get the 'StartupCurve' value, either the base one or the overriden one if needed out of this struct.
	* @return	The interpolation curve of the 'startup' phase of the Controlled Rotation.
				(Must be a normalized curve)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Rotation Datas", meta = (ReturnDisplayName = "StartupCurve"))
	static UCurveFloat* GetStartupCurve(const FControlledRotationDatas& Datas);

	/**
	* Get the 'SnapIgnoreClamp' value, either the base one or the overriden one if needed out of this struct.
	* @return	When searching for snap, ignore clamp on the Rotation Support.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Rotation Datas", meta = (ReturnDisplayName = "SnapIgnoreClamp"))
	static bool GetSnapIgnoreClamp(const FControlledRotationDatas& Datas);

	/**
	* Get the 'SnapIgnoreRanges' value, either the base one or the overriden one if needed out of this struct.
	* @return	When searching for snap, ignore ranges on snap values.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Rotation Datas", meta = (ReturnDisplayName = "SnapIgnoreRanges"))
	static bool GetSnapIgnoreRanges(const FControlledRotationDatas& Datas);
};