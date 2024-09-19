// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveProgress.h"

TMap<TSoftObjectPtr<UWorld>, bool> USaveProgress::GetScenesProgress()
{
	return ScenesProgress;
}
