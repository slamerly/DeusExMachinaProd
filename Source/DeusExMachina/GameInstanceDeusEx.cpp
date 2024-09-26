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

	// Update the ScenesNames when change Scenes
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGameInstanceDeusEx, MainLevelsList))
	{
		if (!MainLevelsList.IsEmpty())
		{
			for (int i = 0; i < MainLevelsList.Num(); i++)
			{
				if (MainLevelsList[i] != nullptr) {
					TArray<ULevelStreaming*> subLevels = MainLevelsList[i]->GetStreamingLevels();

					for (int j = 0; j < subLevels.Num(); j++)
					{
						FString RightName, LeftName;
						subLevels[j]->GetWorldAssetPackageName().Split(TEXT("/"), &RightName, &LeftName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

						ScenesList.Add(*LeftName, MainLevelsList[i]);
					}
				}
			}
		}
	}
}

USaveGame* UGameInstanceDeusEx::GetSaveProgressRef()
{
	return SaveRef;
}

void UGameInstanceDeusEx::SetRefProgress(FString pSceneName, bool bIsDone)
{
	USaveProgress* SaveProgress = Cast<USaveProgress>(SaveRef);
	SaveProgress->SetSaveProgress(pSceneName, bIsDone);
	GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Blue, FString::Printf(TEXT("Scene: %s, puzzle done %d"), *pSceneName, bIsDone));
	GetSaveMap();

	if (UGameplayStatics::SaveGameToSlot(SaveProgress, "DEMSave", 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Yellow, TEXT("Progress Saved."));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("Progress Not Saved."));
	}
}

void UGameInstanceDeusEx::GetSaveMap()
{
	USaveProgress* SaveProgress = Cast<USaveProgress>(SaveRef);
	if (!SaveProgress->GetScenesProgress().IsEmpty())
	{
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
	//GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Orange, FString::Printf(TEXT("Scene : %s"), *pSceneName));
	// Load level if this is another then a current
	if (GetWorld()->GetName() != pLevelName)
	{
		UGameplayStatics::OpenLevel(this, FName(*pLevelName));
	}

	AActor* ActorSceneManager = UGameplayStatics::GetActorOfClass(this, ASceneManager::StaticClass());
	ASceneManager* SceneManager = Cast<ASceneManager>(ActorSceneManager);
	
	SceneManager->ChangeSceneByFName(FName(*pSceneName), false);
}
