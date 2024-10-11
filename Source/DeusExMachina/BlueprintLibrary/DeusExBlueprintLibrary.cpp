#include "DeusExBlueprintLibrary.h"

#if WITH_EDITOR
#include "LevelEditor.h"
#endif // WITH_EDITOR

void UDeusExBlueprintLibrary::AddInstanceComponent(AActor* Actor, UActorComponent* Component)
{
	if (!IsValid(Actor) || !IsValid(Component)) return;

	Actor->AddInstanceComponent(Component);


#if WITH_EDITOR
	//  refresh detail panel to show created component
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.BroadcastComponentsEdited();
	LevelEditorModule.BroadcastRedrawViewports(false);
#endif // WITH_EDITOR
}
