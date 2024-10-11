#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AutoTransInteractionDatas.generated.h"



// ========================================================================================================================
//           Primary Data Asset
// ========================================================================================================================


UCLASS()
class DEUSEXMACHINA_API UAutoTransInteractionBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation Interaction",
		meta = (Tooltip = "If true, the automatic translation will start or stop on the interaction."))
	bool bStartStop{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation Interaction",
		meta = (Tooltip = "If true, the automatic translation will go reverse on the interaction.\nNote: If combined with StartStop, will reverse only when the translation will restart."))
	bool bReverse{ false };
};



// ========================================================================================================================
//           Override Struct
// ========================================================================================================================


USTRUCT(BlueprintType)
struct FAutoTransInteractionDatas
{
	GENERATED_BODY()

	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Auto Trans Interaction for this link."))
	UAutoTransInteractionBaseDatas* AutoTransInteractionDatas{ nullptr };


	//  Override datas

	UPROPERTY(EDitAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the values of the data asset for this link."))
	bool bOverrideValues{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation Interaction", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "If true, the automatic translation will start or stop on the interaction."))
	bool bStartStop{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation Interaction", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "If true, the automatic translation will go reverse on the interaction.\nNote: If combined with StartStop, will reverse only when the translation will restart."))
	bool bReverse{ false };


	//  Getter functions
	bool IsDataValid() const;

	bool GetStartStop() const;
	bool GetReverse() const;
};



// ========================================================================================================================
//           Blueprint Function Library
// ========================================================================================================================


UCLASS()
class DEUSEXMACHINA_API UAutoTransInteractionDatasGet : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	* Get the 'StartStop' value, either the base one or the overriden one if needed out of this struct.
	* @return	If true, the automatic translation will start or stop on the interaction.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Auto Trans Interaction Datas", meta = (ReturnDisplayName = "StartStop"))
	static bool GetStartStop(const FAutoTransInteractionDatas& Datas);

	/**
	* Get the 'Reverse' value, either the base one or the overriden one if needed out of this struct.
	* @return	If true, the automatic translation will go reverse on the interaction.
	*			Note: If combined with StartStop, will reverse only when the translation will restart.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Auto Trans Interaction Datas", meta = (ReturnDisplayName = "Reverse"))
	static bool GetReverse(const FAutoTransInteractionDatas& Datas);



	/**
	* Make an Automatic Translation Interaction Datas struct with custom values.
	* @param	bStartStop		If true, the automatic translation will start or stop on the interaction.
	* @param	bReverse		If true, the automatic translation will go reverse on the interaction.
	*							Note: If combined with StartStop, will reverse only when the translation will restart.
	* @return					The created Automatic Translation Interaction Datas struct.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Auto Trans Interaction Datas", meta = (ReturnDisplayName = "AutoTranslationInteractionDatas"))
	static FAutoTransInteractionDatas MakeAutoTranslationInteractionDatas(bool bStartStop, bool bReverse);
};