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
	RotationBase->SetHiddenInGame(bDisableSupportVisibility);
	Arrow->SetHiddenInGame(bHideAngleArrowVisual);
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

	//UpdateSnapVisual(); //  need to reupdate everytime cause unreal kills the arrows component each time the actor has a modification
	//  called from blueprint construction script
}

void ARotationSupport::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	//UpdateSnapVisual(); //  need to reupdate everytime cause unreal kills the arrows component each time the actor has a modification
	//  called from blueprint construction script
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

void ARotationSupport::GetClampValues(int& ClampLow, int& ClampHigh)
{
	if (!UseValidClamp())
	{
		ClampLow = ClampHigh = 0;
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
		const int ClampHigh = RotSupportValues.GetClampHighValue();
		if (InnerRotation + InputRotationAngle >= ClampHigh)
		{
			ClampedRotationAngle = ClampHigh - InnerRotation;
			return true;
		}
	}
	else
	{
		const int ClampLow = RotSupportValues.GetClampLowValue();
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
//                   Snap Functions
// ======================================================
bool ARotationSupport::UseValidSnap()
{
	if (!RotSupportValues.IsDataValid()) return false;

	if (!RotSupportValues.GetUseSnap()) return false;

	if (!IsValid(RotSupportValues.GetSnapCurveContinue()) || !IsValid(RotSupportValues.GetSnapCurveNeutralReverse())) return false;

	return RotSupportValues.GetSnapValues().Num() > 0;
}

bool ARotationSupport::SearchSnapAngle(const float InputRotationAngle, float& SnapAngle, const float SnapSearchAdvantage, const bool IgnoreClamp, const bool IgnoreSnapRanges)
{
	if (!UseValidSnap())
	{
		SnapAngle = InputRotationAngle;
		return false;
	}


	// Preparation
	// ------------------------------------------

	const int InputMod = UAnglesUtils::ModuloAngleInt(FMath::RoundToInt(InputRotationAngle)); //  'InputRotationAngle' modulated between 0 and 360
	const int InputDiff = FMath::RoundToInt(InputRotationAngle) - InputMod; //  The value to add to an angle base 360 for it to become in the range of 'InputRotationAngle'
	const int InputDiff180 = InputMod - 180; //  The value to "convert" snap values to the search range of input (useful for query highers and lowers)

	const bool UsesClamp = UseValidClamp() && !IgnoreClamp; //  Do it once here to avoir unecessary computations
	const int ClampHigh = RotSupportValues.GetClampHighValue();
	const int ClampLow = RotSupportValues.GetClampLowValue();
	const TArray<FSnapValue> EverySnapValues = RotSupportValues.GetSnapValues();



	// Query higher and lower snaps
	// ------------------------------------------

	//  query the nearest suitable higher snap value
	int NearestHigherSnap = 1000; //  arbitrary value (higher than any value that can be found by this query)
	for (FSnapValue SnapValue : EverySnapValues)
	{
		const int SnapValueConverted = UAnglesUtils::ModuloAngleInt(SnapValue.SnapAngle - InputDiff180);
		if (SnapValueConverted < 180) continue; //  This snap value is for the lower query

		//  check nearest
		if (SnapValueConverted - 180 >= NearestHigherSnap) continue; //  This snap value is not the nearest

		//  check snap value range
		if (SnapValue.bLimitedRangeSnap && !IgnoreSnapRanges)
		{
			if (SnapValueConverted - SnapValue.SnapRangeLower > 180) continue; //  The input angle is outside the range of this snap value
		}

		//  check clamp
		if (UsesClamp)
		{
			const int SnapValueRanged = SnapValue.SnapAngle + InputDiff;
			if (SnapValueRanged > ClampHigh) continue; //  This snap value is outside the clamping range
		}

		//  if the snap value survived through all this, it is valid! congratulations
		NearestHigherSnap = SnapValueConverted - 180;
	}


	//  query the nearest suitable lower snap value
	int NearestLowerSnap = 1000; //  arbitrary value (higher than any value that can be found by this query)
	for (FSnapValue SnapValue : EverySnapValues)
	{
		const int SnapValueConverted = UAnglesUtils::ModuloAngleInt(SnapValue.SnapAngle - InputDiff180);
		if (SnapValueConverted > 180) continue; //  This snap value is for the higher query

		//  check nearest
		if (180 - SnapValueConverted >= NearestLowerSnap) continue; //  This snap value is not the nearest

		//  check snap value range
		if (SnapValue.bLimitedRangeSnap && !IgnoreSnapRanges)
		{
			if (SnapValueConverted + SnapValue.SnapRangeUpper < 180) continue; //  The input angle is outside the range of this snap value
		}

		//  check clamp
		if (UsesClamp)
		{
			const int SnapValueRanged = SnapValue.SnapAngle + InputDiff;
			if (SnapValueRanged < ClampLow) continue; //  This snap value is outside the clamping range
		}

		//  if the snap value survived through all this, it is valid! congratulations
		NearestLowerSnap = 180 - SnapValueConverted;
	}



	// Choose best snap with query results
	// ------------------------------------------

	//  no valid snap value found
	if (NearestHigherSnap >= 1000 && NearestLowerSnap >= 1000)
	{
		SnapAngle = InputRotationAngle;
		return false;
	}

	//  only the higher snap value is valid
	if (NearestHigherSnap < 1000 && NearestLowerSnap >= 1000)
	{
		SnapAngle = FMath::RoundToInt(InputRotationAngle) + NearestHigherSnap;
		return true;
	}

	//  only the lower snap value is valid
	if (NearestHigherSnap >= 1000 && NearestLowerSnap < 1000)
	{
		SnapAngle = FMath::RoundToInt(InputRotationAngle) - NearestLowerSnap;
		return true;
	}

	//  both higher and lower snap values are valid
	if (NearestHigherSnap < 1000 && NearestLowerSnap < 1000)
	{
		const float SearchAdvantage = FMath::Clamp(SnapSearchAdvantage, -1.0f, 1.0f);
		if (SearchAdvantage == -1.0f)
		{
			SnapAngle = FMath::RoundToInt(InputRotationAngle) - NearestLowerSnap;
			return true;
		}
		if (SearchAdvantage == 1.0f)
		{
			SnapAngle = FMath::RoundToInt(InputRotationAngle) + NearestHigherSnap;
			return true;
		}

		const float LowerAdvantaged = FMath::Abs(NearestLowerSnap * (-1.0f - SnapSearchAdvantage));
		const float HigherAdvantaged = FMath::Abs(NearestHigherSnap * (1.0f - SnapSearchAdvantage));

		if (LowerAdvantaged < HigherAdvantaged)
		{
			SnapAngle = FMath::RoundToInt(InputRotationAngle) - NearestLowerSnap;
			return true;
		}
		else
		{
			SnapAngle = FMath::RoundToInt(InputRotationAngle) + NearestHigherSnap;
			return true;
		}

		return true;
	}

	//  if the code reach here (it shouldn't) return false
	SnapAngle = InputRotationAngle;
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
	return CurrentRotationState != ERotationState::NotRotating;
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

	//  check if the snap visual representation is needed
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
		SnapArrowComp->SetHiddenInGame(false);

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