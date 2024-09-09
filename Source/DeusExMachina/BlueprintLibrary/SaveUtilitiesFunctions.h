// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveUtilitiesFunctions.generated.h"

struct DirectoryVisitor : public IPlatformFile::FDirectoryVisitor
{
    //This function is called for every file or directory it finds.
    bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
    {
        // did we find a Directory or a file?
        if (bIsDirectory)
        {
            UE_LOG(LogTemp, Warning, TEXT("FilePaths: Directory found: %s"), FilenameOrDirectory);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("FilePaths: File Found: %s"), FilenameOrDirectory);
        }
        return true;
    }
};

/**
 * 
 */
UCLASS()
class DEUSEXMACHINA_API USaveUtilitiesFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	/**
	 *	Find all files in the Save direcotry with specified folder.
	 *	@param  FString		Folder name you want to check.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Utility")
	static void FindFilesInSaveDir(FString DirectoryName);

	/**
     *   Delete all save files in SaveGames folder.
     */
	UFUNCTION(BlueprintCallable, Category = "Save Utility")
	static void DeleteSaveFiles();

};
