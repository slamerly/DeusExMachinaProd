#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TranslationBehaviorBase.generated.h"

class ATranslationSupport;


UCLASS( Abstract )
class DEUSEXMACHINA_API UTranslationBehaviorBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UTranslationBehaviorBase();

	/** Called when a component is created (not loaded). This can happen in the editor or during gameplay */
	virtual void OnComponentCreated() override;


	// ======================================================
	//              Owner-related functions
	// ======================================================
protected:
	bool InitializeOwner();


	// ======================================================
	//              Owner-related variables
	// ======================================================
protected:
	ATranslationSupport* OwnerTransSupport{ nullptr };
	bool bOwnerTransSupportValid{ false };
};
