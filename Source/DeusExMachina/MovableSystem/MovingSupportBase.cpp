#include "MovingSupportBase.h"
#include "MovableObjectComponent.h"
#include "Components/SceneComponent.h"
#include "DeusExMachina/BlueprintLibrary/LevelUtilitiesFunctions.h"
#include "Defines.h"

AMovingSupportBase::AMovingSupportBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

// ======================================================
//                    Begin Play
// ======================================================
void AMovingSupportBase::BeginPlay()
{
	Super::BeginPlay();
	
	//  check if this support is also a movable object
	SelfMovable = this->GetComponentByClass<UMovableObjectComponent>();
	if (IsValid(SelfMovable))
	{
		bIsSelfMovable = true;
		SelfMovable->OnUpdateEveryChildrens.AddUniqueDynamic(this, &AMovingSupportBase::UpdateEveryChildrens);
	}
}

void AMovingSupportBase::Tick(float DeltaTime)
{
	if (StopMovementPending <= 0) return;
	StopMovementPending--;
	if (StopMovementPending > 0) return;
	StopMovementOnChildrens(false);
}


// ======================================================
//            Movable Childrens Management
// ======================================================
void AMovingSupportBase::AddMovableChild(UMovableObjectComponent* MovableChild)
{
	MovableChildrens.AddUnique(MovableChild);
}

void AMovingSupportBase::StartMovementOnChildrens()
{
	for (auto ChildMovable : MovableChildrens)
	{
		ChildMovable->SetObjectMoving(true);
	}
}

void AMovingSupportBase::StopMovementOnChildrens(bool Delay)
{
	if (Delay)
	{
		StopMovementPending = 2;
		return;
	}

	if (IsCurrentlyMoving()) return;

	if (bIsSelfMovable && SelfMovable->IsObjectMoving()) return;

	for (auto ChildMovable : MovableChildrens)
	{
		ChildMovable->SetObjectMoving(false);
	}
}

void AMovingSupportBase::UpdateEveryChildrens()
{
	for (auto ChildMovable : MovableChildrens)
	{
		ChildMovable->ApplyMovement();
		ChildMovable->OnUpdateEveryChildrens.Broadcast();
	}
}


// ======================================================
//                   Get Transforms
// ======================================================
FTransform AMovingSupportBase::GetObjectTransform()
{
	return GetActorTransform();
}


// ======================================================
//                 Made to be overriden
// ======================================================
bool AMovingSupportBase::IsCurrentlyMoving()
{
	return false;
}

bool AMovingSupportBase::GetPlayerInRange(FVector PlayerPosition)
{
	return false;
}


// ======================================================
//                    Level Editor
// ======================================================
void AMovingSupportBase::ApplyEditorValues()
{
	//  made to be overriden
}

TArray<UMovableObjectComponent*> AMovingSupportBase::RetrieveChildrensEditor()
{
	//  this function allows this moving support to retrieve every movable object attached to it in the editor mode (without playing the game)

	ULevel* Level = GetLevel();

	TArray<AActor*> ActorsInLevel;
	ULevelUtilitiesFunctions::GetAllActorsOfClassInLevel(this, Level, AActor::StaticClass(), ActorsInLevel);

	TArray<UMovableObjectComponent*> ReturnList;

	for (auto Actor : ActorsInLevel)
	{
		UMovableObjectComponent* MovableObject = Actor->GetComponentByClass<UMovableObjectComponent>();
		if (!IsValid(MovableObject)) continue;

		if (MovableObject->MovingSupport != this) continue;

		ReturnList.AddUnique(MovableObject);
	}

	return ReturnList;
}

void AMovingSupportBase::ComputeChildrensOffsetEditor()
{
	TArray<UMovableObjectComponent*> ChildrensMovable = RetrieveChildrensEditor();

	for (auto ChildMovable : ChildrensMovable)
	{
		//  recursive if the child is also a support 
		AMovingSupportBase* ChildSupport{ nullptr };
		const bool IsChildASupport = ChildMovable->IsSelfSupport(&ChildSupport);
		if (IsChildASupport)
		{
			ChildSupport->ComputeChildrensOffsetEditor();
		}

		//  recompute child offset
		ChildMovable->RecomputeTransformOffset();
	}
}

void AMovingSupportBase::ApplyChildLevelEditor()
{
	TArray<UMovableObjectComponent*> ChildrensMovable = RetrieveChildrensEditor();

	for (auto ChildMovable : ChildrensMovable)
	{
		//  apply child editor movement
		ChildMovable->ApplyMovementEditor();

		//  recursive if the child is also a support
		AMovingSupportBase* ChildSupport{ nullptr };
		const bool IsChildASupport = ChildMovable->IsSelfSupport(&ChildSupport);
		if (IsChildASupport)
		{
			ChildSupport->ApplyChildLevelEditor();
		}
	}
}
