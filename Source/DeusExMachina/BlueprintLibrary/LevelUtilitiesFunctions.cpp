#include "LevelUtilitiesFunctions.h"
#include <Kismet/GameplayStatics.h>
#include "EngineUtils.h"

AActor* ULevelUtilitiesFunctions::GetActorOfClassInSublevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Sublevel, const TSubclassOf<AActor> ActorClass)
{
	if (!Sublevel.IsValid()) return nullptr;

	// Get the ULevel from the SoftPtr UWorld
	const ULevel* Level = Sublevel.Get()->GetCurrentLevel();

	return GetActorOfClassInLevel(WorldContextObject, Level, ActorClass);
}

AActor* ULevelUtilitiesFunctions::GetActorOfClassInLevel(const UObject* WorldContextObject, const ULevel* Level, const TSubclassOf<AActor> ActorClass)
{
	if (!IsValid(Level)) return nullptr;

	if (!ActorClass) return nullptr;

	// Not sure of what this is doing but this is in the implementation of Unreal's GetActorOfClass
	// I think this has an utility in Unreal's profiling system
	QUICK_SCOPE_CYCLE_COUNTER(ULevelUtilitiesFunctions_GetActorOfClassInSublevel);

	// Retrieve the world that contains all currently loaded actors from all currently loaded sublevels
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// Check all actors that inherit the provided class
		// Verify if actor found is part of provided sublevel and return it
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;

			if (IsActorPartOfLevel(Actor, Level))
			{
				return Actor;
			}
		}
	}

	// Return nothing if no actor of the provided class has been found in the provided sublevel
	return nullptr;
}

void ULevelUtilitiesFunctions::GetAllActorsOfClassInSublevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Sublevel, const TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors)
{
	if (!Sublevel.IsValid()) return;

	// Get the ULevel from the SoftPtr UWorld
	const ULevel* Level = Sublevel.Get()->GetCurrentLevel();

	GetAllActorsOfClassInLevel(WorldContextObject, Level, ActorClass, OutActors);
}

void ULevelUtilitiesFunctions::GetAllActorsOfClassInLevel(const UObject* WorldContextObject, const ULevel* Level, const TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors)
{
	if (!IsValid(Level)) return;

	if (!ActorClass) return;

	// Not sure of what this is doing but this is in the implementation of Unreal's GetActorOfClass
	// I think this has an utility in Unreal's profiling system
	QUICK_SCOPE_CYCLE_COUNTER(ULevelUtilitiesFunctions_GetAllActorsOfClassInSublevel);

	// Retrieve the world that contains all currently loaded actors from all currently loaded sublevels
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// Check all actors that inherit the provided class
		// Verify if actor found is part of provided sublevel and add it to the out list
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;

			if (IsActorPartOfLevel(Actor, Level))
			{
				OutActors.Add(Actor);
			}
		}
	}
}


bool ULevelUtilitiesFunctions::IsActorPartOfSublevel(const AActor* Actor, const TSoftObjectPtr<UWorld> Sublevel)
{
	// Get the ULevel from the SoftPtr UWorld
	const ULevel* Level = Sublevel.Get()->GetCurrentLevel();

	return IsActorPartOfLevel(Actor, Level);
}

bool ULevelUtilitiesFunctions::IsActorPartOfLevel(const AActor* Actor, const ULevel* Level)
{
	// Get the ULevel where the Actor has been created
	const ULevel* ActorLevel = Actor->GetLevel();

	return ActorLevel == Level;
}
