#include "RotationSupport.h"
#include "Components/SceneComponent.h"
#include "DeusExMachina/Utils/StaticMeshComponentPlus.h"
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

	RotationBase = CreateDefaultSubobject<UStaticMeshComponentPlus>(TEXT("Rotation Base"));
	RotationBase->SetupAttachment(SceneRootComponent);
	RotationBase->bUsedInRotationSupport = true;

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



// ======================================================
//             Prevent Rotation Base Movement
// ======================================================
void ARotationSupport::OnRotationBaseMovedEditor()
{
	bool ComponentReset = false;

	if (RotationBase->GetRelativeLocation() != FVector::ZeroVector)
	{
		RotationBase->SetRelativeLocation(FVector::ZeroVector);
		ComponentReset = true;
	}

	const FRotator CompLocalRot = RotationBase->GetRelativeRotation();
	if (CompLocalRot.Pitch != 0.0f || CompLocalRot.Roll != 0.0f)
	{
		RotationBase->SetRelativeRotation(FRotator{ 0.0f, CompLocalRot.Yaw, 0.0f });
		ComponentReset = true;
	}

	if (ComponentReset)
	{
		const FName PrintKey = "Editor_RotationSupport_RotationBaseMoved_Warning";
		GEngine->AddOnScreenDebugMessage((int32)GetTypeHash(PrintKey), 5.0f, FColor::Orange, "Do NOT move the RotationBase component! Move the entire RotationSupport object instead.");
	}
}