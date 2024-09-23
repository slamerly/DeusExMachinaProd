// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveProgress.h"

TArray<FSceneProgress> USaveProgress::GetScenesProgress()
{
	return ScenesProgress;
}

void USaveProgress::SetScenesProgress(FString pSceneName, bool bIsDone, FString pLevelName)
{
	bool created = false;

	if (!ScenesProgress.IsEmpty())
	{
		for (int i = 0; i < ScenesProgress.Num(); i++)
		{
			if (ScenesProgress[i].SceneName == pSceneName)
			{
				if (ScenesProgress[i].bPuzzleDone == false)
				{
					ScenesProgress[i].bPuzzleDone = bIsDone;
				}
				created = true;
				return;
			}
		}

		if (!created)
		{
			ScenesProgress.Add(FSceneProgress(pSceneName, bIsDone, pLevelName));
		}
	}
	else
	{
		ScenesProgress.Add(FSceneProgress(pSceneName, bIsDone, pLevelName));
	}
}
