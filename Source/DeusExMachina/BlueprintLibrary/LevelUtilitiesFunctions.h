// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelUtilitiesFunctions.generated.h"

/**
 * 
 */
UCLASS()
class DEUSEXMACHINA_API ULevelUtilitiesFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 *	Find the first Actor in a specified Sublevel of the specified class.
	 *	This is a slow operation, use with caution e.g. do not use every frame.
	 *	@param  Sublevel    Sublevel where Actor will be searched in. Must be specified or result will be empty.
	 *	@param	ActorClass	Class of Actor to find. Must be specified or result will be empty.
	 *	@return				Actor of the specified class.
	 */
	UFUNCTION(BlueprintCallable, Category = "Level Utility", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "ActorClass"))
	static AActor* GetActorOfClassInSublevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Sublevel, const TSubclassOf<AActor> ActorClass);

	/**
	 *	Find the first Actor in a specified ULevel of the specified class.
	 *	This is a slow operation, use with caution e.g. do not use every frame.
	 *	@param  Level    ULevel where Actor will be searched in. Must be specified or result will be empty.
	 *	@param	ActorClass	Class of Actor to find. Must be specified or result will be empty.
	 *	@return				Actor of the specified class.
	 */
	UFUNCTION(BlueprintCallable, Category = "Level Utility", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "ActorClass"))
	static AActor* GetActorOfClassInLevel(const UObject* WorldContextObject, const ULevel* Level, const TSubclassOf<AActor> ActorClass);


	/**
	 *	Find all Actors in a specified Sublevel of the specified class. 
	 *	This is a slow operation, use with caution e.g. do not use every frame.
	 *	@param  Sublevel    Sublevel where Actors will be searched in. Must be specified or result will be empty.
	 *	@param	ActorClass	Class of Actor to find. Must be specified or result will be empty.
	 *	@param	OutActors	Array of Actors of the specified class.
	 */
	UFUNCTION(BlueprintCallable, Category = "Level Utility", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "ActorClass", DynamicOutputParam = "OutActors"))
	static void GetAllActorsOfClassInSublevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Sublevel, const TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors);


	/**
	 *	Find all Actors in a specified ULevel of the specified class.
	 *	This is a slow operation, use with caution e.g. do not use every frame.
	 *	@param  Level    ULevel where Actors will be searched in. Must be specified or result will be empty.
	 *	@param	ActorClass	Class of Actor to find. Must be specified or result will be empty.
	 *	@param	OutActors	Array of Actors of the specified class.
	 */
	UFUNCTION(BlueprintCallable, Category = "Level Utility", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "ActorClass", DynamicOutputParam = "OutActors"))
	static void GetAllActorsOfClassInLevel(const UObject* WorldContextObject, const ULevel* Level, const TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors);



	/**
	* Check if the Actor has been created in the specified Sublevel.
	* @param  Actor     Actor that will be checked.
	* @param  Sublevel  Sublevel that will be checked in.
	* @return           True if Actor has been created in this Sublevel, False otherwise.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level Utility")
	static bool IsActorPartOfSublevel(const AActor* Actor, const TSoftObjectPtr<UWorld> Sublevel);

	/**
	* Check if the Actor has been created in the specified ULevel.
	* @param  Actor     Actor that will be checked.
	* @param  Level		ULevel that will be checked in.
	* @return           True if Actor has been created in this ULevel, False otherwise.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level Utility")
	static bool IsActorPartOfLevel(const AActor* Actor, const ULevel* Level);
};
