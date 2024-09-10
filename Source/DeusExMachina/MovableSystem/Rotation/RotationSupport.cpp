#include "RotationSupport.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "DeusExMachina/MovableSystem/MovableObjectComponent.h"
#include "AnglesUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Defines.h"


// ======================================================
//                    Constructor
// ======================================================
ARotationSupport::ARotationSupport()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SceneRootComponent->SetupAttachment(GetRootComponent());

	RotationBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rotation Base"));
	RotationBase->SetupAttachment(SceneRootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RotationBase);
}


// ======================================================
//                    Begin Play
// ======================================================

void ARotationSupport::BeginPlay()
{
	Super::BeginPlay();
}


// ======================================================
//                      Tick
// ======================================================
void ARotationSupport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// ======================================================
//                   Get Transforms
// ======================================================
FTransform ARotationSupport::GetObjectTransform()
{
	return RotationBase->GetComponentTransform();
}

FTransform ARotationSupport::GetObjectTransformRelative()
{
	return RotationBase->GetRelativeTransform();
}


// ======================================================
//                      Utility
// ======================================================
bool ARotationSupport::IsCurrentlyMoving()
{
	return false;
}

bool ARotationSupport::GetPlayerInRange(FVector PlayerPosition)
{
	return false;
}


// ======================================================
//                    Testing Editor
// ======================================================
void ARotationSupport::ApplyTestingValues()
{
	TestingAngle = UAnglesUtils::ModuloAngle(TestingAngle);
	
	ComputeChildrensOffsetEditor();

	const FRotator CurrentRot = RotationBase->GetRelativeRotation();
	RotationBase->SetRelativeRotation(FRotator{CurrentRot.Pitch, TestingAngle, CurrentRot.Roll});

	ApplyChildTestingEditor();
}

