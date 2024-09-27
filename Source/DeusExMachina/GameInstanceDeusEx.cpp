// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceDeusEx.h"
#include <Kismet/GameplayStatics.h>
#include "ProgressSystem/SaveProgress.h"
#include "Manager/SceneManager.h"

void UGameInstanceDeusEx::Init()
{
	Super::Init();

	// Verify if the savefile exist
	if (UGameplayStatics::DoesSaveGameExist("DEMSave", 0))
	{
		SaveRef = UGameplayStatics::LoadGameFromSlot("DEMSave", 0);
	}
	else
	{
		SaveRef = UGameplayStatics::CreateSaveGameObject(USaveProgress::StaticClass());
	}
}

void UGameInstanceDeusEx::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Update the ScenesNames when change Scenes.
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGameInstanceDeusEx, MainLevelsList))
	{
		if (!MainLevelsList.IsEmpty())
		{
			for (int i = 0; i < MainLevelsList.Num(); i++)
			{
				// Verify if an element in the MainLevelList is not valid.
				if (MainLevelsList[i] != nullptr) {
					TArray<ULevelStreaming*> SubLevels = MainLevelsList[i]->GetStreamingLevels();

					for (int j = 0; j < SubLevels.Num(); j++)
					{
						// To have the right name of the scene we should pass by the package name and
						// split the path to only have the scene name.
						FString RightName, LeftName;
						SubLevels[j]->GetWorldAssetPackageName().Split(TEXT("/"), &RightName, &LeftName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

						// Add in the list the scene name with the level where it is.
						ScenesList.Add(*LeftName, MainLevelsList[i]);
					}
				}
			}
		}
	}
}

void UGameInstanceDeusEx::SetSaveProgress(FString pSceneName, bool bIsDone)
{
	if(!DebugMode)
	{
		USaveProgress* SaveProgress = Cast<USaveProgress>(SaveRef);

		// Add the modification in save reference
		SaveProgress->SetProgress(pSceneName, bIsDone);

		// Debug to display what we recieved here
		//GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Blue, FString::Printf(TEXT("Scene: %s, puzzle done %d"), *pSceneName, bIsDone));

		// Debug to display all elements there are in the array of the save reference
		//DisplayScenesSaved();

		// Save the reference in the savefile
		if (UGameplayStatics::SaveGameToSlot(SaveProgress, "DEMSave", 0))
		{
			GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Green, TEXT("Progress Saved."));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("Progress Not Saved."));
		}
	}
}

void UGameInstanceDeusEx::DisplayScenesSaved()
{
	USaveProgress* SaveProgress = Cast<USaveProgress>(SaveRef);
	if (!SaveProgress->GetScenesProgress().IsEmpty())
	{
		// for each elements in the reference, we display the scene's name and its puzzle status 
		for (auto& scene : SaveProgress->GetScenesProgress())
		{
			GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Green, FString::Printf(TEXT("Scene: %s, puzzle done %d"), *scene.SceneName, scene.bPuzzleDone));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, FString::Printf(TEXT("The array is empty.")));
	}
}

void UGameInstanceDeusEx::LoadProgress(FString pLevelName, FString pSceneName)
{
	// Load level if this is another then the current
	if (GetWorld()->GetName() != pLevelName)
	{
		UGameplayStatics::OpenLevel(this, FName(*pLevelName));
	}

	// Find the 
	AActor* ActorSceneManager = UGameplayStatics::GetActorOfClass(this, ASceneManager::StaticClass());
	if(ActorSceneManager != nullptr)
	{
		ASceneManager* SceneManager = Cast<ASceneManager>(ActorSceneManager);
		SceneManager->ChangeSceneByFName(FName(*pSceneName), false);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, FString::Printf(TEXT("No SceneManager find in the level: %s"), *pLevelName));
}

void UGameInstanceDeusEx::SetDebugMode(bool NewStatus)
{
	DebugMode = NewStatus;
}

bool UGameInstanceDeusEx::GetDebugMode()
{
	return DebugMode;
}
