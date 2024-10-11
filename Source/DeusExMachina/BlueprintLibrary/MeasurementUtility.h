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
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Measurement Utility")
	static bool IsInEditor();


	/**
	* Create a file from a string and save it.
	* @param	StringToFile	The string to save as file.
	* @param	FileSavePath	The save path of the file created.
	* @return					True if success.
	*/
	UFUNCTION(BlueprintCallable, Category = "Measurement Utility")
	static bool SaveStringAsFile(const FString& StringToFile, const FString& FileSavePath);


	/**
	* Load a file into a string.
	* @param	FileSavePath	The save path of the file to load.
	* @param	StringFromFile	The string to save the file content in.
	* @return					True if success.
	*/
	UFUNCTION(BlueprintCallable, Category = "Measurement Utility")
	static bool LoadFileAsString(const FString& FileSavePath, FString& StringFromFile);
};
