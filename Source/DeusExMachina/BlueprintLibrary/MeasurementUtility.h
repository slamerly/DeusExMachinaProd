#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeasurementUtility.generated.h"


UCLASS()
class DEUSEXMACHINA_API UMeasurementUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	* Get if the game is running in the editor (PIE).
	* @return	True if the game is running in the editor (PIE).
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Measurement Utility", meta = (WorldContext = "WorldContextObject"))
	bool IsInEditor(const UObject* WorldContectObject);


	/**
	* Create a file from a string and save it.
	* @param	StringToFile	The string to save as file.
	* @param	FileSavePath	The save path of the file created.
	*/
	UFUNCTION(BlueprintCallable, Category = "Measurement Utility")
	void SaveStringAsFile(const FString& StringToFile, const FString& FileSavePath);
};
