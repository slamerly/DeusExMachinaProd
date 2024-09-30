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
	PrimaryActorTick.bCanEverTick = false;

	//  create default components

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
}



// ======================================================
//                    Begin Play
// ======================================================
void ARotationSupport::BeginPlay()
{
	//  call parent begin play to compute self movable
	Super::BeginPlay();

	//  'EditorAngle' serve as a begin play angle, initialize 'InnerRotation' with it
	InnerRotation = UAnglesUtils::ModuloAngle(EditorAngle);

	//  reset the relative transform of rotation base to prevent unexpected behavior in game
	const FVector RotationBaseScale3D = RotationBase->GetRelativeScale3D();
	RotationBase->SetRelativeTransform(FTransform{ FRotator{0.0f, InnerRotation, 0.0f}, FVector::ZeroVector, RotationBaseScale3D });

	//  hide support and remove collision (if necessary)
	RotationBase->SetHiddenInGame(bDisableSupportVisibility);
	Arrow->SetHiddenInGame(bHideAngleArrowVisual);
	RotationBase->SetCollisionEnabled(bDisableSupportCollision ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	//  update clamp & snap visual
	UpdateClampVisual();
	UpdateSnapVisual();
}



// ======================================================
//               Inner Rotation Functions
// ======================================================
bool ARotationSupport::AddInnerRotation(float InnerRotAdd, bool TestClamp)
{
	bool TriggerClamp = false;

	if (TestClamp)
	{
		//  simulate the add rotation with the clamp to add a clamped rotation
		float InnerRotAddClamped = InnerRotAdd;
		TriggerClamp = SimulateRotationWithClamp(InnerRotAdd, InnerRotAddClamped);
		InnerRotation += InnerRotAddClamped;
	}
	else
	{
		//  simply add the rotation
		InnerRotation += InnerRotAdd;
	}

	ComputeInnerTransform();
	return TriggerClamp;
}

void ARotationSupport::ForceInnerRotation(int InnerRot, bool AbsoluteRotation)
{
	if (AbsoluteRotation)
	{
		//  simply set the inner rotation to the inputed one
		InnerRotation = InnerRot;
	}
	else
	{
		//  compute the nearest rotation angle of current InnerRotation that is a modulo of inputed InnerRot

		const int InnerRotInt = FMath::RoundToInt(InnerRotation);
		const int CurrentInnerMod = UAnglesUtils::ModuloAngleInt(InnerRotInt); //  retrieve the inner rotation in base 360
		const int CurrentInnerDiff = InnerRotInt - CurrentInnerMod; //  compute the offset between base 360 inner rot and real inner rot
		int InputRotMod = UAnglesUtils::ModuloAngleInt(InnerRot); //  retrieve the inputed inner rot in base 360

		if (CurrentInnerMod - InputRotMod > 180)
		{
			//  the closest angle of input rot from current innner rot is superior to current inner rot
			InputRotMod += 360;
		}
		else if (CurrentInnerMod - InputRotMod < -180)
		{
			//  the closest angle of input rot from current inner rot is inferior to current inner rot
			InputRotMod -= 360;
		}

		InnerRotation = InputRotMod + CurrentInnerDiff; //  return the computed inputed inner rot with the offset to go back to the range of the inner rot at the beginnning of this function
	}

	ComputeInnerTransform();
}



// ======================================================
//             Compute Inner Rotation Helper
// ======================================================
void ARotationSupport::ComputeInnerTransform()
{
	//  simply set the relative rotation to the current inner rotation
	RotationBase->SetRelativeRotation(FRotator{ 0.0f, InnerRotation, 0.0f });
}



// ======================================================
//                Inner Rotation Getters
// ======================================================
float ARotationSupport::GetInnerRotation() const
{
	return InnerRotation;
}

float ARotationSupport::GetInnerRotationBase360() const
{
	return UAnglesUtils::ModuloAngle(InnerRotation);
}

const FRotSupportValues ARotationSupport::GetSupportValues() const
{
	return RotSupportValues;
}



// ======================================================
//                  Clamp Functions
// ======================================================
bool ARotationSupport::UseValidClamp()
{
	//  check if clamp values are valid
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
		//  clamp invalid = no clamp
		ClampedRotationAngle = InputRotationAngle;
		return false;
	}

	if (InputRotationAngle >= 0.0f) //  check clamp high
	{
		const int ClampHigh = RotSupportValues.GetClampHighValue();
		if (InnerRotation + InputRotationAngle >= ClampHigh) //  check trigger clamp high
		{
			ClampedRotationAngle = ClampHigh - InnerRotation;
			return true;
		}
	}
	else //  check clamp low
	{
		const int ClampLow = RotSupportValues.GetClampLowValue();
		if (InnerRotation + InputRotationAngle <= ClampLow) //  check trigger clamp low
		{
			ClampedRotationAngle = ClampLow - InnerRotation;
			return true;
		}
	}

	//  clamp was not triggered
	ClampedRotationAngle = InputRotationAngle;
	return false;
}



// ======================================================
//                   Snap Functions
// ======================================================
bool ARotationSupport::UseValidSnap()
{
	//  check if snap values are valid
	if (!RotSupportValues.IsDataValid()) return false;
	if (!RotSupportValues.GetUseSnap()) return false;

	if (!IsValid(RotSupportValues.GetSnapCurveContinue()) || !IsValid(RotSupportValues.GetSnapCurveNeutralReverse())) return false;
	return RotSupportValues.GetSnapValues().Num() > 0;
}

bool ARotationSupport::SearchSnapAngle(const float InputRotationAngle, float& SnapAngle, const float SnapSearchAdvantage, const bool IgnoreClamp, const bool IgnoreSnapRanges)
{
	if (!UseValidSnap())
	{
		//  invalid snap = no snap
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
		//  compute snap search advantage
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

		//  compute query results with snap search advantage
		const float LowerAdvantaged = FMath::Abs(NearestLowerSnap * (-1.0f - SearchAdvantage));
		const float HigherAdvantaged = FMath::Abs(NearestHigherSnap * (1.0f - SearchAdvantage));

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
//             Moving Support Base Functions
// ======================================================
FTransform ARotationSupport::GetObjectTransform()
{
	return RotationBase->GetComponentTransform();
}

bool ARotationSupport::IsCurrentlyMoving()
{
	return CurrentRotationState != ERotationState::NotRotating;
}



// ======================================================
//             Clamp & Snap Visual Functions
// ======================================================
void ARotationSupport::UpdateClampVisual()
{
	//  check if show clamp visual is needed
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

		//  set clamp low arrow values
		const float LowClamp = RotSupportValues.GetClampLowValue();
		const float LowClampRad = FMath::DegreesToRadians(LowClamp);
		ClampVisualLow->SetWorldScale3D(BaseScale * 0.5f);
		ClampVisualLow->SetRelativeRotation(FRotator{ 0.0f, UAnglesUtils::ModuloAngle(LowClamp + 180.0f), 0.0f });
		ClampVisualLow->SetRelativeLocation(FVector{ FMath::Cos(LowClampRad) * 140.0f, FMath::Sin(LowClampRad) * 140.0f, BaseScale.Z + 2.0f });

		//  set clamp high arrow values
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

		//  construct new arrow component
		SnapArrows.Add(Cast<UArrowComponent>(AddComponentByClass(UArrowComponent::StaticClass(), false, FTransform::Identity, false)));
		UArrowComponent* SnapArrowComp = SnapArrows[i];
		SnapArrowComp->SetupAttachment(SceneRootComponent);
		SnapArrowComp->SetHiddenInGame(false);

		//  set constructed arrow values
		SnapArrowComp->SetArrowColor(FLinearColor{ 0.89f, 0.09, 1.0f });
		SnapArrowComp->SetWorldScale3D(BaseScale * 0.2f);
		SnapArrowComp->SetRelativeRotation(FRotator{ 0.0f, UAnglesUtils::ModuloAngle(SnapValues[i].SnapAngle + 180.0f), 0.0f });
		SnapArrowComp->SetRelativeLocation(FVector{ FMath::Cos(SnapAngleRad) * 150.0f, FMath::Sin(SnapAngleRad) * 150.0f, BaseScale.Z + 2.5f });
	}
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



// ======================================================
//             Prevent Rotation Base Movement
// ======================================================
void ARotationSupport::OnRotationBaseMovedEditor()
{
	bool ComponentReset = false;

	if (RotationBase->GetRelativeLocation() != FVector::ZeroVector)
	{
		//  check rotation base location
		RotationBase->SetRelativeLocation(FVector::ZeroVector);
		ComponentReset = true;
	}

	const FRotator CompLocalRot = RotationBase->GetRelativeRotation();
	if (CompLocalRot.Pitch != 0.0f || CompLocalRot.Roll != 0.0f)
	{
		//  check rotation base rotation
		RotationBase->SetRelativeRotation(FRotator{ 0.0f, EditorAngle, 0.0f });
		ComponentReset = true;
	}

	if (ComponentReset)
	{
		//  print warning if detected a location or rotation change on rotation base
		const FName PrintKey = "Editor_RotationSupport_RotationBaseMoved_Warning";
		GEngine->AddOnScreenDebugMessage((int32)GetTypeHash(PrintKey), 5.0f, FColor::Orange, "Do NOT move the RotationBase component! Move the entire RotationSupport object instead.");
	}
}