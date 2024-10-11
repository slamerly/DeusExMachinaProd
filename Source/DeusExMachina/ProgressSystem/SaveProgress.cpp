// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveProgress.h"

TArray<FPuzzleProgress> USaveProgress::GetScenesProgress()
{
	return ScenesProgress;
}

void USaveProgress::SetProgress(FString pSceneName, bool bIsDone)
{
	bool created = false; // to know if the if the scene is already in the array

	// if the array is not empty
	if (!ScenesProgress.IsEmpty())
	{
		// search if there is already the scene in the array
		for (int i = 0; i < ScenesProgress.Num(); i++)
		{
			if (ScenesProgress[i].SceneName == pSceneName)
			{
				// modify only the value
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
			ScenesProgress.Add(FPuzzleProgress(pSceneName, bIsDone));
		}
	}
	else
	{
		ScenesProgress.Add(FPuzzleProgress(pSceneName, bIsDone));
	}
}
