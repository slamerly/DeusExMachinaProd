#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StandardTranslationDatas.generated.h"



// ========================================================================================================================
//           Primary Data Asset
// ========================================================================================================================


UENUM(BlueprintType)
enum class EStandardTranslationMode : uint8
{
	MoveSplinePoints = 0 UMETA(Tooltip = "In this mode, the Translation Support will move on the spline for a number of points."),
	MoveDistance = 1 UMETA(Tooltip = "In this mode, the Translation Support will move on the spline for a fixed distance.")
};


UENUM(BlueprintType)
enum class EStandardTranslationSpeedMode : uint8
{
	TranslationDuration = 0 UMETA(Tooltip = "In this mode, the Translation Support speed will vary so the translation always take the same duration."),
	TranslationSpeed = 1 UMETA(Tooltip = "In this mode, the Translation Support speed will be always the same (except the spline interpolation).")
};


UCLASS()
class DEUSEXMACHINA_API UStandardTranslationBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (Tooltip = "The standard translation mode to use for this Translation Support."))
	EStandardTranslationMode StandardTranslationMode{ EStandardTranslationMode::MoveSplinePoints };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas", 
		meta = (EditCondition = "StandardTranslationMode == EStandardTranslationMode::MoveSplinePoints", EditConditionHides),
		meta = (Tooltip = "The number of spline points to move this Translation Support (0 mean the support won't move).\nA negative value will make the support going reverse."))
	int TranslationSplinePoints{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "StandardTranslationMode == EStandardTranslationMode::MoveDistance", EditConditionHides),
		meta = (Tooltip = "The distance on the spline this Translation Support will do (0 mean the support won't move).\nA negative value will make the support going reverse.\n(In cm)"))
	float TranslationSplineDistance{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (Tooltip = "The standard translation speed mode to use for this Translation Support."))
	EStandardTranslationSpeedMode StandardTranslationSpeedMode{ EStandardTranslationSpeedMode::TranslationDuration };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "StandardTranslationSpeedMode == EStandardTranslationSpeedMode::TranslationDuration", EditConditionHides),
		meta = (Tooltip = "The time that the complete Standard Translation will take.\n(In seconds)", ClampMin = 0.0f))
	float TranslationDuration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "StandardTranslationSpeedMode == EStandardTranslationSpeedMode::TranslationSpeed", EditConditionHides),
		meta = (Tooltip = "The speed of the Standard Translation.\n(In cm/s)", ClampMin = 0.0f))
	float TranslationSpeed{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (Tooltip = "The interpolation curve that will smooth the Standard Translation.\n(Must be a normalized curve)"))
	UCurveFloat* TranslationCurve{ nullptr };
};



// ========================================================================================================================
//           Override Struct
// ========================================================================================================================


USTRUCT(BlueprintType)
struct FStandardTranslationDatas
{
	GENERATED_BODY()


	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Standard Translation for this link."))
	UStandardTranslationBaseDatas* StandardTransInteractionDatas{ nullptr };


	//  Override datas

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the values of the data asset for this link."))
	bool bOverrideValues{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "The standard translation mode to use for this Translation Support."))
	EStandardTranslationMode StandardTranslationMode{ EStandardTranslationMode::MoveSplinePoints };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "bOverrideValues && StandardTranslationMode == EStandardTranslationMode::MoveSplinePoints", EditConditionHides),
		meta = (Tooltip = "The number of spline points to move this Translation Support (0 mean the support won't move).\nA negative value will make the support going reverse."))
	int TranslationSplinePoints{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "bOverrideValues && StandardTranslationMode == EStandardTranslationMode::MoveDistance", EditConditionHides),
		meta = (Tooltip = "The distance on the spline this Translation Support will do (0 mean the support won't move).\nA negative value will make the support going reverse.\n(In cm)"))
	float TranslationSplineDistance{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "The standard translation speed mode to use for this Translation Support."))
	EStandardTranslationSpeedMode StandardTranslationSpeedMode{ EStandardTranslationSpeedMode::TranslationDuration };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "bOverrideValues && StandardTranslationSpeedMode == EStandardTranslationSpeedMode::TranslationDuration", EditConditionHides),
		meta = (Tooltip = "The time that the complete Standard Translation will take.\n(In seconds)", ClampMin = 0.0f))
	float TranslationDuration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "bOverrideValues && StandardTranslationSpeedMode == EStandardTranslationSpeedMode::TranslationSpeed", EditConditionHides),
		meta = (Tooltip = "The speed of the Standard Translation.\n(In cm/s)", ClampMin = 0.0f))
	float TranslationSpeed{ 1.0f };


	UPROPERTY(EDitAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the curve value of the data asset for this link."))
	bool bOverrideCurveValue{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas", meta = (EditCondition = "bOverrideCurveValue", EditConditionHides),
		meta = (Tooltip = "The interpolation curve that will smooth the Standard Translation.\n(Must be a normalized curve)"))
	UCurveFloat* TranslationCurve{ nullptr };


	//  Getter functions
	bool IsDataValid() const;

	EStandardTranslationMode GetStandardTranslationMode() const;
	int GetTranslationSplinePoints() const;
	float GetTranslationSplineDistance() const;
	EStandardTranslationSpeedMode GetStandardTranslationSpeedMode() const;
	float GetTranslationDuration() const;
	float GetTranslationSpeed() const;
	UCurveFloat* GetTranslationCurve() const;
};



// ========================================================================================================================
//           Blueprint Function Library
// ========================================================================================================================


UCLASS()
class DEUSEXMACHINA_API UStandardTranslationDatasGet : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	* Get the 'StandardTranslationMode' value, either the base one or the overriden one if needed out of this struct.
	* @return	The standard translation mode to use for this Translation Support.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Standard Translation Datas", meta = (ReturnDisplayName = "StandardTranslationMode"))
	static EStandardTranslationMode GetStandardTranslationMode(const FStandardTranslationDatas& Datas);

	/**
	* Get the 'TranslationSplinePoints' value, either the base one or the overriden one if needed out of this struct.
	* @return	The number of spline points to move this Translation Support (0 mean the support won't move).
	*			A negative value will make the support going reverse.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Standard Translation Datas", meta = (ReturnDisplayName = "TranslationSplinePoints"))
	static int GetTranslationSplinePoints(const FStandardTranslationDatas& Datas);

	/**
	* Get the 'TranslationSplineDistance' value, either the base one or the overriden one if needed out of this struct.
	* @return	The distance on the spline this Translation Support will do (0 mean the support won't move).
	*			A negative value will make the support going reverse.
	*			(In cm)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Standard Translation Datas", meta = (ReturnDisplayName = "TranslationSplineDistance"))
	static float GetTranslationSplineDistance(const FStandardTranslationDatas& Datas);

	/**
	* Get the 'StandardTranslationSpeedMode' value, either the base one or the overriden one if needed out of this struct.
	* @return	The standard translation speed mode to use for this Translation Support.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Standard Translation Datas", meta = (ReturnDisplayName = "StandardTranslationSpeedMode"))
	static EStandardTranslationSpeedMode GetStandardTranslationSpeedMode(const FStandardTranslationDatas& Datas);

	/**
	* Get the 'TranslationDuration' value, either the base one or the overriden one if needed out of this struct.
	* @return	The time that the complete Standard Translation will take.
	*			(In seconds)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Standard Translation Datas", meta = (ReturnDisplayName = "TranslationDuration"))
	static float GetTranslationDuration(const FStandardTranslationDatas& Datas);

	/**
	* Get the 'TranslationSpeed' value, either the base one or the overriden one if needed out of this struct.
	* @return	The speed of the Standard Translation.
	*			(In cm/s)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Standard Translation Datas", meta = (ReturnDisplayName = "TranslationSpeed"))
	static float GetTranslationSpeed(const FStandardTranslationDatas& Datas);

	/**
	* Get the 'TranslationCurve' value, either the base one or the overriden one if needed out of this struct.
	* @return	The interpolation curve that will smooth the Standard Translation.
	*			(Must be a normalized curve)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Standard Translation Datas", meta = (ReturnDisplayName = "TranslationCurve"))
	static UCurveFloat* GetTranslationCurve(const FStandardTranslationDatas& Datas);



	/**
	* Make a Sandard Translation Datas struct with custom values.
	* @param	StandardTranslationMode			The standard translation mode to use for this Translation Support.
	* @param	TranslationSplinePoints			The number of spline points to move this Translation Support (0 mean the support won't move).
	*											A negative value will make the support going reverse.
	* @param	TranslationSplineDistance		The distance on the spline this Translation Support will do (0 mean the support won't move).
	*											A negative value will make the support going reverse.
	*											(In cm)
	* @param	StandardTranslationSpeedMode	The standard translation speed mode to use for this Translation Support.
	* @param	TranslationDuration				The time that the complete Standard Translation will take.
	*											(In seconds)
	* @param	TranslationSpeed				The speed of the Standard Translation.
	*											(In cm/s)
	* @param	TranslationCurve				The interpolation curve that will smooth the Standard Translation.
	*											(Must be a normalized curve)
	* @return									The created Standard Translation Datas struct.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Standard Translation Datas", meta = (ReturnDisplayName = "StandardTranslationDatas"))
	static FStandardTranslationDatas MakeStandardTranslationDatas(EStandardTranslationMode StandardTranslationMode, int TranslationSplinePoints, float TranslationSplineDistance,
		EStandardTranslationSpeedMode StandardTranslationSpeedMode, float TranslationDuration, float TranslationSpeed, UCurveFloat* TranslationCurve);
};