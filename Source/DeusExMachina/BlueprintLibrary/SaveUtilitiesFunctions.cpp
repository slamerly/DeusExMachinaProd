// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveUtilitiesFunctions.h"


void USaveUtilitiesFunctions::FindFilesInSaveDir(FString DirectoryName)
{
    FString MyProjectDirectoryTree = FPaths::ProjectSavedDir();
    MyProjectDirectoryTree.Append(DirectoryName);
    IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
    //Now we need to create a DirectoryVisitor 
    DirectoryVisitor Visitor;
    // The ItterateDirectory takes two arguments The directory and the Visitor we created above.
    if (FileManager.IterateDirectory(*MyProjectDirectoryTree, Visitor))
    {
        //Visitor.Visit(*MyConfigDirectoryTree, false);
        UE_LOG(LogTemp, Warning, TEXT("FilePaths: Directory's or files found"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("FilePaths: Directory did not exist or visitor returned false"));
    }
}

void USaveUtilitiesFunctions::DeleteSaveFiles()
{
    FString ProjectSavedDirPath = FPaths::ProjectSavedDir();
    ProjectSavedDirPath.Append(TEXT("SaveGames/"));
    IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

    // Delete all files and directory in the path give before in ProjectSavedDirPath
    if (FileManager.DeleteDirectoryRecursively(*ProjectSavedDirPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("FilePaths: Directory and sub-files and folders are Deleted"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("FilePaths: Directory and sub-files and folders are not Deleted"));
    }
}
