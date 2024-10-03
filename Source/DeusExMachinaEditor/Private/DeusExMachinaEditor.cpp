#include "DeusExMachinaEditor.h"

#include "PropertyEditorModule.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationSupport.h"
#include "RotationSupportDetails.h"

void FDeusExMachinaEditorModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("DeusExMachina Editor loaded."));

	FPropertyEditorModule& PropertyEdModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEdModule.RegisterCustomClassLayout(
		ARotationSupport::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FRotationSupportDetails::MakeInstance)
	);
}

void FDeusExMachinaEditorModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("DeusExMachina Editor shut down."));
}

IMPLEMENT_MODULE(FDeusExMachinaEditorModule, DeusExMachinaEditor);