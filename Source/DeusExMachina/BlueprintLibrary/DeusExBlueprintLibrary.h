#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DeusExBlueprintLibrary.generated.h"


UCLASS()
class DEUSEXMACHINA_API UDeusExBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	

	/**
	* Add a component to the actor component instance list (the permanent component list of this actor), and refresh this list in the detail panel.
	* @param	Actor		The actor you want to instanciate the component on.
	* @param	Component	The component to instanciate on this actor.
	*/
	UFUNCTION(BlueprintCallable, Category = "Deus Ex Utility")
	static void AddInstanceComponent(AActor* Actor, UActorComponent* Component);
};
