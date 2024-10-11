#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ControlledTranslationDatas.generated.h"



// ========================================================================================================================
//           Primary Data Asset
// ========================================================================================================================


UCLASS()
class DEUSEXMACHINA_API UControlledTranslationBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Speed",
		meta = (Tooltip = "The speed of the Translation Support when performing a Controlled Translation.\n(In cm/s)"))
	float TranslationSpeed{ 60.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Startup",
		meta = (Tooltip = "The duration of the 'startup' phase of the Controlled Translation.\nThe startup phase occurs when the translation support starts moving.\n(In seconds)"))
	float StartupDuration{ 0.8f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Startup",
		meta = (Tooltip = "The interpolation curve of the 'startup' phase of the Controlled Translation.\n(Must be a normalized curve)"))
	UCurveFloat* StartupCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap",
		meta = (Tooltip = "Does the Translation Support snap on a spline point when releasing the control?"))
	bool bUseSnap{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (ClampMin = 0.0f, ClampMax = 1.0f),
		meta = (Tooltip = "Does the Translation Support prefer snapping to the spline point 0 or spline point 1"))
	float SnapPreference{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap",
		meta = (Tooltip = "The speed of the Translation Support when snapping.\nNote that the curves can make it feel faster or slower in game.\n(In cm/s)"))
	float SnapSpeed{ 60.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap",
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The Translation Support is not moving (neutral)\n- The snap goes in the opposite direction than the Translation Support movement (reverse)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveNeutralReverse{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap",
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The snap goes in the same direction than the Translation Support movement (continue)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveContinue{ nullptr };
};



// ========================================================================================================================
//           Override Struct
// ========================================================================================================================


USTRUCT(BlueprintType)
struct FControlledTranslationDatas
{
	GENERATED_BODY()

	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Controlled Translation for this link."))
	UControlledTranslationBaseDatas* ControlledTransInteractionDatas{ nullptr };


	//  Override datas

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the speed value of the data asset for this link."))
	bool bOverrideSpeed{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Speed", meta = (EditCondition = "bOverrideSpeed", EditConditionHides),
		meta = (Tooltip = "The speed of the Translation Support when performing a Controlled Translation.\n(In cm/s)"))
	float TranslationSpeed{ 60.0f };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the startup values of the data asset for this link."))
	bool bOverrideStartup{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Startup", meta = (EditCondition = "bOverrideStartup", EditConditionHides),
		meta = (Tooltip = "The duration of the 'startup' phase of the Controlled Translation.\nThe startup phase occurs when the translation support starts moving.\n(In seconds)"))
	float StartupDuration{ 0.8f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Startup", meta = (EditCondition = "bOverrideStartup", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the 'startup' phase of the Controlled Translation.\n(Must be a normalized curve)"))
	UCurveFloat* StartupCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the snap values of the data asset for this link."))
	bool bOverrideSnap{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (EditCondition = "bOverrideSnap", EditConditionHides),
		meta = (Tooltip = "Does the Translation Support snap on a spline point when releasing the control?"))
	bool bUseSnap{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (ClampMin = 0.0f, ClampMax = 1.0f, EditCondition = "bOverrideSnap", EditConditionHides),
		meta = (Tooltip = "Does the Translation Support prefer snapping to the spline point 0 or spline point 1."))
	float SnapPreference{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (EditCondition = "bOverrideSnap", EditConditionHides),
		meta = (Tooltip = "The speed of the Translation Support when snapping.\nNote that the curves can make it feel faster or slower in game.\n(In cm/s)"))
	float SnapSpeed{ 60.0f };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the snap curves of the data asset for this link."))
	bool bOverrideSnapCurves{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (EditCondition = "bOverrideSnapCurves", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The Translation Support is not moving (neutral)\n- The snap goes in the opposite direction than the Translation Support movement (reverse)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveNeutralReverse{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (EditCondition = "bOverrideSnapCurves", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The snap goes in the same direction than the Translation Support movement (continue)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveContinue{ nullptr };


	//  Getter functions
	bool IsDataValid() const;

	float GetTranslationSpeed() const;
	float GetStartupDuration() const;
	UCurveFloat* GetStartupCurve() const;

	bool GetUseSnap() const;
	float GetSnapPreference() const;
	float GetSnapSpeed() const;
	UCurveFloat* GetSnapCurveNeutralReverse() const;
	UCurveFloat* GetSnapCurveContinue() const;
};



// ========================================================================================================================
//           Blueprint Function Library
// ========================================================================================================================


UCLASS()
class DEUSEXMACHINA_API UControlledTranslationDatasGet : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	* Get the 'TranslationSpeed' value, either the base one or the overriden one if needed out of this struct.
	* @return	The speed of the Translation Support when performing a Controlled Translation.
	*			(In cm/s)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Translation Datas", meta = (ReturnDisplayName = "TranslationSpeed"))
	static float GetTranslationSpeed(const FControlledTranslationDatas& Datas);

	/**
	* Get the 'StartupDuration' value, either the base one or the overriden one if needed out of this struct.
	* @return	The duration of the 'startup' phase of the Controlled Translation.
	*			The startup phase occurs when the translation support starts moving.
	*			(In seconds)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Translation Datas", meta = (ReturnDisplayName = "StartupDuration"))
	static float GetStartupDuration(const FControlledTranslationDatas& Datas);

	/**
	* Get the 'StartupCurve' value, either the base one or the overriden one if needed out of this struct.
	* @return	The interpolation curve of the 'startup' phase of the Controlled Translation.
	*			(Must be a normalized curve)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Translation Datas", meta = (ReturnDisplayName = "StartupCurve"))
	static UCurveFloat* GetStartupCurve(const FControlledTranslationDatas& Datas);


	/**
	* Get the 'UseSnap' value, either the base one or the overriden one if needed out of this struct.
	* @return	Does the Translation Support snap on a spline point when releasing the control?
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Translation Datas|Snap", meta = (ReturnDisplayName = "UseSnap"))
	static bool GetUseSnap(const FControlledTranslationDatas& Datas);

	/**
	* Get the 'SnapPreference' value, either the base one or the overriden one if needed out of this struct.
	* @return	Does the Translation Support prefer snapping to the spline point 0 or spline point 1.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Translation Datas|Snap", meta = (ReturnDisplayName = "SnapPreference"))
	static float GetSnapPreference(const FControlledTranslationDatas& Datas);

	/**
	* Get the 'SnapSpeed' value, either the base one or the overriden one if needed out of this struct.
	* @return	The speed of the Translation Support when snapping.
	*			Note that the curves can make it feel faster or slower in game.
	*			(In cm/s)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Translation Datas|Snap", meta = (ReturnDisplayName = "SnapSpeed"))
	static float GetSnapSpeed(const FControlledTranslationDatas& Datas);

	/**
	* Get the 'SnapCurveNeutralReverse' value, either the base one or the overriden one if needed out of this struct.
	* @return	The interpolation curve of the snapping used when:
	*			- The Translation Support is not moving (neutral)
	*			- The snap goes in the opposite direction than the Translation Support movement (reverse)
	*			(Must be a normalized curve)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Translation Datas|Snap", meta = (ReturnDisplayName = "SnapCurveNeutralReverse"))
	static UCurveFloat* GetSnapCurveNeutralReverse(const FControlledTranslationDatas& Datas);

	/**
	* Get the 'SnapCurveContinue' value, either the base one or the overriden one if needed out of this struct.
	* @return	The interpolation curve of the snapping used when:
	*			- The snap goes in the same direction than the Translation Support movement (continue)
	*			(Must be a normalized curve)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Translation Datas|Snap", meta = (ReturnDisplayName = "SnapCurveContinue"))
	static UCurveFloat* GetSnapCurveContinue(const FControlledTranslationDatas& Datas);



	/**
	* Make a Controlled Translation Datas struct with custom values.
	* @param	TranslationSpeed			The speed of the Translation Support when performing a Controlled Translation.
	*										(In cm/s)
	* @param	StartupDuration				The duration of the 'startup' phase of the Controlled Translation.
	*										The startup phase occurs when the translation support starts moving.
	*										(In seconds)
	* @param	StartupCurve				The interpolation curve of the 'startup' phase of the Controlled Translation.
	*										(Must be a normalized curve)
	* 
	* @param	bUseSnap					Does the Translation Support snap on a spline point when releasing the control?
	* @param	SnapPreference				Does the Translation Support prefer snapping to the spline point 0 or spline point 1.
	* @param	SnapSpeed					The speed of the Translation Support when snapping.
	*										Note that the curves can make it feel faster or slower in game.
	*										(In cm/s)
	* 
	* @param	SnapCurveNeutralReverse		The interpolation curve of the snapping used when:
	*										- The Translation Support is not moving (neutral)
	*										- The snap goes in the opposite direction than the Translation Support movement (reverse)
	*										(Must be a normalized curve)
	* @param	SnapCurveContinue			The interpolation curve of the snapping used when:
	*										- The snap goes in the same direction than the Translation Support movement (continue)
	*										(Must be a normalized curve)
	* @return								The created Controlled Translation Datas struct.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controlled Translation Datas|Snap", meta = (ReturnDisplayName = "ControlledTranslationDatas"))
	static FControlledTranslationDatas MakeControlledTranslationDatas(float TranslationSpeed, float StartupDuration, UCurveFloat* StartupCurve,
		bool bUseSnap, float SnapPreference, float SnapSpeed, UCurveFloat* SnapCurveNeutralReverse, UCurveFloat* SnapCurveContinue);
};