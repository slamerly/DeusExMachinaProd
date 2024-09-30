#include "MovableObjectComponent.h"
#include "MovingSupportBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Defines.h"

UMovableObjectComponent::UMovableObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// ======================================================
//                    Begin Play
// ======================================================
void UMovableObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false); //  tick disabled by default (it will be enabled again when the object this component is on will move)


	//  check moving support parent
	if (!IsValid(MovingSupport))
	{
		kPRINT_COLOR("Warning : " + UKismetSystemLibrary::GetDisplayName(GetOwner()) + " has no associated Moving Support!", FColor::Orange);
		return;
	}

	bMovingSupportValid = true;
	MovingSupport->AddMovableChild(this);


	//  compute offset from moving support parent
	RecomputeTransformOffset();


	//  check if self moving support
	bSelfMovingSupportComputed = true;
	SelfMovingSupport = Cast<AMovingSupportBase>(GetOwner());
	if (IsValid(SelfMovingSupport))
	{
		bIsMovingSupport = true;
	}

}

// ======================================================
//                       Tick
// ======================================================
void UMovableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//  auto update transform if the object this component is on is moving
	if (!CheckIsMovingFull()) return;
	ApplyMovement();
}


// ======================================================
//                     Movement
// ======================================================
void UMovableObjectComponent::ApplyMovement()
{
	//if (!bMovingSupportValid) return; //  prevent this to be called from the editor. need to find a solution

	const FTransform ComposedTransform = TransformOffset * MovingSupport->GetObjectTransform();

	GetOwner()->SetActorLocation(ComposedTransform.GetLocation());

	GetOwner()->SetActorRotation((bDoNotInheritRotation ? TransformOffset : ComposedTransform).GetRotation());
}

void UMovableObjectComponent::SetObjectMoving(bool MovingValue)
{
	if (!bMovingSupportValid) return;

	bMoving = MovingValue;
	SetEnableTick(MovingValue);

	if (!bIsMovingSupport) return; //  next part is useful only if this component is on a moving support

	if (MovingValue)
	{
		SelfMovingSupport->StartMovementOnChildrens();
	}
	else
	{
		SelfMovingSupport->StopMovementOnChildrens();
	}
}

void UMovableObjectComponent::RecomputeTransformOffset()
{
	if (!IsValid(MovingSupport)) return; //  can't use 'bMovingSupportValid' cause this can is called from the editor where begin play hasn't ran yet

	//  compute transform relative to owner
	const FTransform OwnerTransform = GetOwner()->GetActorTransform();
	const FTransform RelativeTransform = UKismetMathLibrary::MakeRelativeTransform(OwnerTransform, MovingSupport->GetObjectTransform());

	TransformOffset = FTransform
	{
		(bDoNotInheritRotation ? OwnerTransform : RelativeTransform).GetRotation(),
		RelativeTransform.GetLocation(),
		RelativeTransform.GetScale3D()
	}; //  rotation then location then scale when building transform this way
}


// ======================================================
//                  Self Moving Support
// ======================================================
bool UMovableObjectComponent::IsSelfSupport(AMovingSupportBase** MovingSupportSelf)
{
	if (bSelfMovingSupportComputed)
	{
		*MovingSupportSelf = SelfMovingSupport;
		return bIsMovingSupport;
	}

	//  if self moving support is not computed, it mean that this function is called from the 'TestingEditor'
	AMovingSupportBase* EditorSelfSupport = Cast<AMovingSupportBase>(GetOwner());
	if (IsValid(EditorSelfSupport))
	{
		*MovingSupportSelf = EditorSelfSupport;
		return true;
	}
	else
	{
		return false;
	}
}


// ======================================================
//                    Level Editor
// ======================================================
void UMovableObjectComponent::ApplyMovementEditor()
{
	ApplyMovement(); 
	
	//  actually there is no need for this function to exist, but it can be useful to track down a bug in the testing editor system
}


// ======================================================
//                   Is Object Moving
// ======================================================
bool UMovableObjectComponent::IsObjectMoving()
{
	return bMoving;
}

bool UMovableObjectComponent::IsSelfSupportMoving()
{
	if (!bIsMovingSupport) return false;

	return SelfMovingSupport->IsCurrentlyMoving();
}


// ======================================================
//                  Helping Functions
// ======================================================
bool UMovableObjectComponent::CheckIsMovingFull()
{
	return IsObjectMoving() || IsSelfSupportMoving();
}

void UMovableObjectComponent::SetEnableTick(bool EnableTick)
{
	if (bIsMovingSupport)
	{
		SetComponentTickEnabled(true);
	}
	else
	{
		SetComponentTickEnabled(EnableTick);
	}
}

