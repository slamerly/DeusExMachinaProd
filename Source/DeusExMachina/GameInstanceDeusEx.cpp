// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceDeusEx.h"
#include <Kismet/GameplayStatics.h>
#include "ProgressSystem/SaveProgress.h"

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

USaveGame* UGameInstanceDeusEx::GetSaveProgressRef()
{
	return SaveRef;
}

void UGameInstanceDeusEx::SetSaveProgress(TSoftObjectPtr<UWorld> scene, bool bIsDone)
{
	
	USaveProgress* SaveProgress = Cast<USaveProgress>(SaveRef);
	SaveProgress->GetScenesProgress().Add(scene, bIsDone);
	UGameplayStatics::SaveGameToSlot(SaveProgress, "DEMSave", 0);
	GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Yellow, TEXT("Progress Saved."));
	
}

void UGameInstanceDeusEx::GetSaveMap()
{

}
