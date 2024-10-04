#include "MeasurementUtility.h"

bool UMeasurementUtility::IsInEditor(const UObject* WorldContectObject)
{
	if (!IsValid(WorldContectObject)) return true;

	return WorldContectObject->GetWorld()->WorldType == EWorldType::PIE;
}

void UMeasurementUtility::SaveStringAsFile(const FString& StringToFile, const FString& FileSavePath)
{

}