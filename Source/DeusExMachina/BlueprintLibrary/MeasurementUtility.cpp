#include "MeasurementUtility.h"
#include "Misc/FileHelper.h"

bool UMeasurementUtility::IsInEditor()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif // WITH_EDITOR
}

bool UMeasurementUtility::SaveStringAsFile(const FString& StringToFile, const FString& FileSavePath)
{
	return FFileHelper::SaveStringToFile(StringToFile, *FileSavePath);
}

bool UMeasurementUtility::LoadFileAsString(const FString& FileSavePath, FString& StringFromFile)
{
	return FFileHelper::LoadFileToString(StringFromFile, *FileSavePath);
}
