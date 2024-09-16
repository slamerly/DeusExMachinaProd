#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovableObjectComponent.generated.h"

class AMovingSupportBase;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateChildrensSignature);

UCLASS( ClassGroup=(MovableSystem), meta=(BlueprintSpawnableComponent, DisplayName = "Movable Object", Tooltip = "Component to add to every object that should be moved by a moving support.\nCan also be added to a moving support."))
class DEUSEXMACHINA_API UMovableObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMovableObjectComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


// ======================================================
//                    Functions
// ======================================================
public:
	// ====================
	//      Movement
	// ====================
	void ApplyMovement();
	void SetObjectMoving(bool MovingValue);
	void RecomputeTransformOffset();

	// ====================
	//  Self Moving Support
	// ====================
	bool IsSelfSupport(AMovingSupportBase** MovingSupportSelf = 0);

	// ====================
	//     Level Editor
	// ====================
	void ApplyMovementEditor();



// ======================================================
//                  Helping Functions
// ======================================================
public:
	bool IsObjectMoving();
	bool IsSelfSupportMoving();

protected:
	bool CheckIsMovingFull(); //  check for IsObjectMoving or IsSelfSupportMoving
	void SetEnableTick(bool EnableTick); //  this function prevent tick to be disabled if this component is on a moving support



// ======================================================
//          Variables - Moving Support Parent
// ======================================================
public:
	// ====================
	//  Instance Editable
	// ====================
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (Tooltip = "Can be a TranslationSupport or a RotationSupport."))
	AMovingSupportBase* MovingSupport{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (Tooltip = "Should this object not inherit the rotation of its support ?\nIt will move following its support but won't rotate on itself.\n(Basically it will always face the same direction even if it is moved by its support.)"))
	bool bDoNotInheritRotation{ false };

protected:
	// ====================
	//      Internal
	// ====================
	bool bMovingSupportValid{ false };



// ======================================================
//                Variables - Others
// ======================================================
protected:
	// ====================
	//      Movement
	// ====================
	bool bMoving{ false };
	FTransform TransformOffset{ FTransform::Identity };

	// ====================
	//  Self Moving Support
	// ====================
	bool bIsMovingSupport{ false };
	bool bSelfMovingSupportComputed{ false };
	AMovingSupportBase* SelfMovingSupport{ nullptr };

public:
	// ====================
	//       Event
	// ====================
	FUpdateChildrensSignature OnUpdateEveryChildrens;
};
