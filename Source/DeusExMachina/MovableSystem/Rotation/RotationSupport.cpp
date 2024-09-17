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

	ClampVisualLow = CreateDefaultSubobject<UArrowComponent>(TEXT("Clamp Low"));
	ClampVisualLow->SetupAttachment(SceneRootComponent);

	ClampVisualHigh = CreateDefaultSubobject<UArrowComponent>(TEXT("Clamp High"));
	ClampVisualHigh->SetupAttachment(SceneRootComponent);


	//UpdateClampVisual();
	//UpdateSnapVisual();
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
	RotationBase->SetRelativeTransform(FTransform{ FRotator{0.0f, InnerRotation, 0.0f}, FVector::ZeroVector, RotationBaseScale3D });

	//  call parent begin play after reseting the rotation base so that the computation of StartTransform is correct
	Super::BeginPlay();


	//  hide support and remove collision (if necessary)
	SceneRootComponent->SetHiddenInGame(bDisableSupportVisibility, true);
	RotationBase->SetCollisionEnabled(bDisableSupportCollision ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);


	//  update clamp & snap visual
	UpdateClampVisual();
	UpdateSnapVisual();
}


// ======================================================
//                        Tick
// ======================================================
void ARotationSupport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// ======================================================
//              Auto update Clamp & Snap
// ======================================================
void ARotationSupport::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FString PropertyChangedName = PropertyChangedEvent.GetPropertyName().ToString();
	
	if (PropertyChangedName.Contains("Clamp"))
	{
		UpdateClampVisual();
	}

	UpdateSnapVisual(); //  need to reupdate everytime cause unreal kills the arrows component each time the actor has a modification
}

void ARotationSupport::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	UpdateSnapVisual(); //  need to reupdate everytime cause unreal kills the arrows component each time the actor has a modification
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

bool ARotationSupport::AddInnerRotation(float InnerRotAdd, bool TestClamp)
{
	bool TriggerClamp = false;

	if (TestClamp)
	{
		float InnerRotAddClamped = InnerRotAdd;
		TriggerClamp = SimulateRotationWithClamp(InnerRotAdd, InnerRotAddClamped);
		InnerRotation += InnerRotAddClamped;
	}
	else
	{
		InnerRotation += InnerRotAdd;
	}

	ComputeInnerTransform();
	return TriggerClamp;
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
//                  Clamp Functions
// ======================================================
bool ARotationSupport::UseValidClamp()
{
	if (!RotSupportValues.IsDataValid()) return false;

	if (!RotSupportValues.GetUseClamp()) return false;

	return RotSupportValues.GetClampHighValue() > RotSupportValues.GetClampLowValue();
}

void ARotationSupport::GetClampValues(float& ClampLow, float& ClampHigh)
{
	if (!UseValidClamp())
	{
		ClampLow = ClampHigh = 0.0f;
		return;
	}

	ClampLow = RotSupportValues.GetClampLowValue();
	ClampHigh = RotSupportValues.GetClampHighValue();
}

bool ARotationSupport::SimulateRotationWithClamp(const float InputRotationAngle, float& ClampedRotationAngle)
{
	if (!UseValidClamp())
	{
		ClampedRotationAngle = InputRotationAngle;
		return false;
	}

	if (InputRotationAngle >= 0.0f)
	{
		float ClampHigh = RotSupportValues.GetClampHighValue();
		if (InnerRotation + InputRotationAngle >= ClampHigh)
		{
			ClampedRotationAngle = ClampHigh - InnerRotation;
			return true;
		}
	}
	else
	{
		float ClampLow = RotSupportValues.GetClampLowValue();
		if (InnerRotation + InputRotationAngle <= ClampLow)
		{
			ClampedRotationAngle = ClampLow - InnerRotation;
			return true;
		}
	}

	ClampedRotationAngle = InputRotationAngle;
	return false;
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

	RotationBase->SetRelativeRotation(FRotator{ 0.0f, EditorAngle, 0.0f });

	ApplyChildLevelEditor();
}

void ARotationSupport::UpdateClampVisual()
{
	if (!RotSupportValues.IsDataValid() || bHideClampVisual || !RotSupportValues.GetUseClamp())
	{
		ClampVisualLow->SetVisibility(false);
		ClampVisualHigh->SetVisibility(false);
	}
	else
	{
		ClampVisualLow->SetVisibility(true);
		ClampVisualHigh->SetVisibility(true);

		const FVector BaseScale = RotationBase->GetComponentScale();

		const float LowClamp = RotSupportValues.GetClampLowValue();
		const float LowClampRad = FMath::DegreesToRadians(LowClamp);
		ClampVisualLow->SetWorldScale3D(BaseScale * 0.5f);
		ClampVisualLow->SetRelativeRotation(FRotator{ 0.0f, UAnglesUtils::ModuloAngle(LowClamp + 180.0f), 0.0f });
		ClampVisualLow->SetRelativeLocation(FVector{FMath::Cos(LowClampRad) * 140.0f, FMath::Sin(LowClampRad) * 140.0f, BaseScale.Z + 2.0f});

		const float HighClamp = RotSupportValues.GetClampHighValue();
		const float HighClampRad = FMath::DegreesToRadians(HighClamp);
		ClampVisualHigh->SetWorldScale3D(BaseScale * 0.5f);
		ClampVisualHigh->SetRelativeRotation(FRotator{ 0.0f, UAnglesUtils::ModuloAngle(HighClamp + 180.0f), 0.0f });
		ClampVisualHigh->SetRelativeLocation(FVector{ FMath::Cos(HighClampRad) * 140.0f, FMath::Sin(HighClampRad) * 140.0f, BaseScale.Z + 2.0f });
	}
}

void ARotationSupport::UpdateSnapVisual()
{
	//  reset the snap arrows array
	for (auto SnapArrow : SnapArrows)
	{
		if (!IsValid(SnapArrow)) continue;
		SnapArrow->DestroyComponent();
	}
	SnapArrows.Empty();

	//  check if the snap visual representation if needed
	if (!RotSupportValues.IsDataValid()) return;
	if (!RotSupportValues.GetUseSnap()) return;
	if (bHideSnapVisual) return;

	const TArray<FSnapValue> SnapValues = RotSupportValues.GetSnapValues();
	if (SnapValues.Num() <= 0) return;


	//  construct the new arrow components
	const FVector BaseScale = RotationBase->GetComponentScale();

	SnapArrows.Reserve(SnapValues.Num());
	for (int i = 0; i < SnapValues.Num(); i++)
	{
		const float SnapAngleRad = FMath::DegreesToRadians(SnapValues[i].SnapAngle);

		SnapArrows.Add(Cast<UArrowComponent>(AddComponentByClass(UArrowComponent::StaticClass(), false, FTransform::Identity, false)));
		UArrowComponent* SnapArrowComp = SnapArrows[i];
		SnapArrowComp->SetupAttachment(SceneRootComponent);

		SnapArrowComp->SetArrowColor(FLinearColor{ 0.89f, 0.09, 1.0f });
		SnapArrowComp->SetWorldScale3D(BaseScale * 0.2f);
		SnapArrowComp->SetRelativeRotation(FRotator{0.0f, UAnglesUtils::ModuloAngle(SnapValues[i].SnapAngle + 180.0f), 0.0f});
		SnapArrowComp->SetRelativeLocation(FVector{ FMath::Cos(SnapAngleRad) * 150.0f, FMath::Sin(SnapAngleRad) * 150.0f, BaseScale.Z + 2.5f });
	}
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