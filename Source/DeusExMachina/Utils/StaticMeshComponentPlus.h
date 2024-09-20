#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "StaticMeshComponentPlus.generated.h"


/**
 * A classic Static Mesh Component with minor additionnal functionnalities
 * Actually there is hardcoded system for Rotation Support because the event didn't worked properly.
 */
UCLASS(ClassGroup = MovSysCustomComponent, meta = (BlueprintSpawnableComponent), Blueprintable)
class DEUSEXMACHINA_API UStaticMeshComponentPlus : public UStaticMeshComponent
{
	GENERATED_BODY()


public:
#if WITH_EDITOR
	/** Called when this component is moved in the editor */
	virtual void PostEditComponentMove(bool bFinished) override;

	/** Called when a value is changed on this component */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR


private:
	/** Return true if the PropertyChangedEvent is a transform change of this component */
	bool CheckPropertyChangedEventTransform(FPropertyChangedEvent& PropertyChangedEvent);

	void BroadcastRotSupport();


public:
	bool bUsedInRotationSupport{ false };
};
