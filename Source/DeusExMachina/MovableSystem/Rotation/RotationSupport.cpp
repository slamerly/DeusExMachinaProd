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
	//  'EditorAngle' serve as a begin play angle, initialize 'InnerRotation' with it
	InnerRotation = UAnglesUtils::ModuloAngle(EditorAngle);

	//  reset the relative transform of rotation base to prevent unexpected behavior in game
	const FVector RotationBaseScale3D = RotationBase->GetRelativeScale3D();
	RotationBase->SetRelativeTransform(FTransform{ FRotator{0.0f, InnerRotation, 0.0f}, FVector::ZeroVector, RotationBaseScale3D});

	//  call parent begin play after reseting the rotation base so that the computation of StartTransform is correct
	Super::BeginPlay();


	//  hide support and remove collision (if necessary)
	SceneRootComponent->SetHiddenInGame(bDisableSupportVisibility, true);
	RotationBase->SetCollisionEnabled(bDisableSupportCollision ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
}


// ======================================================
//                      Tick
// ======================================================
void ARotationSupport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// ======================================================
//                  Inner Rotation
// ======================================================
float ARotationSupport::GetInnerRotation()
{
	return InnerRotation;
}

float ARotationSupport::GetInnerRotationBase360()
{
	return UAnglesUtils::ModuloAngle(InnerRotation);
}

void ARotationSupport::AddInnerRotation(float InnerRotAdd)
{
	InnerRotation += InnerRotAdd;

	ComputeInnerTransform();
}

void ARotationSupport::ForceInnerRotation(float InnerRot, bool AbsoluteRotation)
{
	if (AbsoluteRotation)
	{
		InnerRotation = InnerRot;
	}
	else
	{
		//  compute the nearest rotation angle of current InnerRotation that is a modulo of inputed InnerRot

		const float CurrentInnerMod = UAnglesUtils::ModuloAngle(InnerRotation);
		const float CurrentInnerDiff = InnerRotation - CurrentInnerMod;
		float InputRotMod = UAnglesUtils::ModuloAngle(InnerRot);

		if (CurrentInnerMod - InputRotMod > 180.0f)
		{
			InputRotMod += 360.0f;
		}
		else if (CurrentInnerMod - InputRotMod < -180.0f)
		{
			InputRotMod -= 360.0f;
		}

		InnerRotation = InputRotMod + CurrentInnerDiff;
	}

	ComputeInnerTransform();
}

void ARotationSupport::ComputeInnerTransform()
{
	//  simply set the yaw rotation value of InnerTransform to the delta angle done since begin play
	FRotator CurrentInnerTransformRot = InnerTransform.GetRotation().Rotator();
	CurrentInnerTransformRot.Yaw = GetInnerRotationBase360() - EditorAngle;
	InnerTransform.SetRotation(CurrentInnerTransformRot.Quaternion());

	ApplyInnerMovement(RotationBase);
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
//                     Level Editor
// ======================================================
void ARotationSupport::ApplyEditorValues()
{
	EditorAngle = UAnglesUtils::ModuloAngle(EditorAngle);
	
	ComputeChildrensOffsetEditor();

	RotationBase->SetRelativeRotation(FRotator{0.0f, EditorAngle, 0.0f});

	ApplyChildLevelEditor();
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
		RotationBase->SetRelativeRotation(FRotator{ 0.0f, EditorAngle, 0.0f });
		ComponentReset = true;
	}

	if (ComponentReset)
	{
		const FName PrintKey = "Editor_RotationSupport_RotationBaseMoved_Warning";
		GEngine->AddOnScreenDebugMessage((int32)GetTypeHash(PrintKey), 5.0f, FColor::Orange, "Do NOT move the RotationBase component! Move the entire RotationSupport object instead.");
	}
}