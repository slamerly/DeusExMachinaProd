#include "TranslationSupport.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DeusExMachina/Utils/SplineComponentPlus.h"
#include "Components/ArrowComponent.h"
#include "DeusExMachina/MovableSystem/MovableObjectComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Defines.h"


// ======================================================
//                    Constructor
// ======================================================

ATranslationSupport::ATranslationSupport()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SceneRootComponent->SetupAttachment(GetRootComponent());

	TranslationBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Translation Base"));
	TranslationBase->SetupAttachment(SceneRootComponent);

	TranslationSpline = CreateDefaultSubobject<USplineComponentPlus>(TEXT("Translation Spline"));
	TranslationSpline->SetupAttachment(SceneRootComponent);
	TranslationSpline->bAllowDiscontinuousSpline = true;
}


// ======================================================
//                    Begin Play
// ======================================================
void ATranslationSupport::BeginPlay()
{
	//  'EditorSplineIndex' serve as a begin play spline index, initialize 'InnerSplineIndex' with it
	InnerSplineIndex = EditorSplineIndex % TranslationSpline->GetNumberOfSplinePoints();
	ProgressToNextIndex = 0.0f;
	ComputeDistanceFromSplineOrigin();

	//  reset the relative location of rotation base to prevent unexpeced behavior in game
	TranslationBase->SetRelativeLocation(TranslationSpline->GetLocationAtSplinePoint(EditorSplineIndex, ESplineCoordinateSpace::Local));

	//  call parent begin play after reseting the translation base so that the computation of StartTransform is correct
	Super::BeginPlay();

	//  hide support and remove collision (if necessary)
	TranslationBase->SetHiddenInGame(bDisableSupportVisibility);
	TranslationBase->SetCollisionEnabled(bDisableSupportCollision ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	//  show spline points visual if needed
	UpdateSplinePointsVisual();
}


// ======================================================
//                        Tick
// ======================================================
void ATranslationSupport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// ======================================================
//                  Inner Translation
// ======================================================
float ATranslationSupport::GetDistanceFromSplineOrigin() const
{
	return DistanceFromSplineOrigin;
}

int ATranslationSupport::GetInnerSplineIndex() const
{
	return InnerSplineIndex;
}

float ATranslationSupport::GetProgressToNextIndex() const
{
	return ProgressToNextIndex;
}

void ATranslationSupport::AddTranslationAlongSpline(const float TranslationAdd)
{
	//  compute the distance on the spline from the spline origin
	DistanceFromSplineOrigin += TranslationAdd;
	const float SplineLength = TranslationSpline->GetSplineLength();
	if (DistanceFromSplineOrigin >= SplineLength)
	{
		DistanceFromSplineOrigin -= SplineLength;
	}
	else if (DistanceFromSplineOrigin < 0.0f)
	{
		DistanceFromSplineOrigin += SplineLength;
	}

	//  compute the support current spline index and progress to the next index
	ComputeInnerIndexAndProgress();


	//  apply the support movement
	ComputeInnerTransform();
}

void ATranslationSupport::ForcePositionOnSpline(const float DistanceFromOrigin)
{
	//  apply distance from origin
	DistanceFromSplineOrigin = DistanceFromOrigin;
	const float SplineLength = TranslationSpline->GetSplineLength();
	if (DistanceFromSplineOrigin >= SplineLength)
	{
		DistanceFromSplineOrigin -= SplineLength;
	}
	else if (DistanceFromSplineOrigin < 0.0f)
	{
		DistanceFromSplineOrigin += SplineLength;
	}

	//  compute the support current spline index and progress to the next index
	ComputeInnerIndexAndProgress();


	//  apply the support movement
	ComputeInnerTransform();
}

void ATranslationSupport::ForcePositionOnSpline(const int SplineIndex, const float ProgressToNextPoint)
{
	//  apply spline index
	InnerSplineIndex = SplineIndex;
	const int NumberSplinePoints = TranslationSpline->GetNumberOfSplinePoints();
	if (InnerSplineIndex >= NumberSplinePoints)
	{
		InnerSplineIndex %= NumberSplinePoints;
	}
	else if (InnerSplineIndex < 0)
	{
		InnerSplineIndex %= NumberSplinePoints;
		InnerSplineIndex += NumberSplinePoints; //  modulo does not get the number above 0
	}

	//  apply progress to next point
	ProgressToNextIndex = ProgressToNextPoint;
	if (ProgressToNextIndex >= 1.0f)
	{
		ProgressToNextIndex = UKismetMathLibrary::Percent_FloatFloat(ProgressToNextIndex, 1.0f);
	}
	else if (ProgressToNextIndex <= 0.0f)
	{
		ProgressToNextIndex = UKismetMathLibrary::Percent_FloatFloat(ProgressToNextIndex, 1.0f);
		ProgressToNextIndex += 1.0f; //  modulo does not get the number above 0.0f
	}

	//  compute the distance on the spline from the spline origin
	ComputeDistanceFromSplineOrigin();


	//  apply the support movement
	ComputeInnerTransform();
}

void ATranslationSupport::ComputeInnerTransform()
{
	TranslationBase->SetRelativeLocation(TranslationSpline->GetLocationAtDistanceAlongSpline(DistanceFromSplineOrigin, ESplineCoordinateSpace::Local));
}


// ======================================================
//                    Clamp Functions
// ======================================================
float ATranslationSupport::ClampMovementBetweenSplinePoints(const float Movement, const int SplineIndexA, const int SplineIndexB)
{
	const float DistanceOA = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(SplineIndexA);
	const float DistanceOB = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(SplineIndexB);
	const float PreAppliedMovement = DistanceFromSplineOrigin + Movement;

	if (SplineIndexA == SplineIndexB)
	{
		//  return a movement that will put the support on point A (and on point B since they're the same in this case)
		return DistanceOA - DistanceFromSplineOrigin;
	}
	else if (SplineIndexA < SplineIndexB)
	{
		//  the simple computation, no need to get around the spline origin
		if (PreAppliedMovement > DistanceOB)
		{
			return DistanceOB - DistanceFromSplineOrigin;
		}
		else if (PreAppliedMovement < DistanceOA)
		{
			return DistanceOA - DistanceFromSplineOrigin;
		}

		//  all good
		return Movement;
	}
	else if (SplineIndexA > SplineIndexB) //  it's just a else in this case, but for better understandability I let the if condition visible
	{
		//  the (slightly) harder computation, need to get around the spline origin
		const float DistanceOBAround = TranslationSpline->GetSplineLength() + DistanceOB;

		if (PreAppliedMovement > DistanceOBAround)
		{
			return DistanceOBAround - DistanceFromSplineOrigin;
		}
		else if (PreAppliedMovement < DistanceOA)
		{
			return DistanceOA - DistanceFromSplineOrigin;
		}

		//  all good
		return Movement;
	}

	return Movement;
}


// ======================================================
//                   Utility Functions
// ======================================================
float ATranslationSupport::GetSplineDistanceToNextSplinePoint()
{
	return GetSplineDistanceToPoint(GetNextSplineIndex(InnerSplineIndex));
}

float ATranslationSupport::GetSplineDistanceToPoint(const int SplineIndexA)
{
	const float DistanceOA = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(SplineIndexA);

	float DistanceIA = DistanceOA - DistanceFromSplineOrigin; //  I for inner
	if (DistanceIA < 0.0f) DistanceIA += TranslationSpline->GetSplineLength();

	return DistanceIA;
}

float ATranslationSupport::GetSplineDistanceAToB(const int SplineIndexA, const int SplineIndexB)
{
	const float DistanceOA = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(SplineIndexA);
	const float DistanceOB = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(SplineIndexB);

	float DistanceAB = DistanceOB - DistanceOA;
	if (DistanceAB < 0.0f) DistanceAB += TranslationSpline->GetSplineLength();

	return DistanceAB;
}

int ATranslationSupport::GetNextSplineIndex(const int SplineIndex)
{
	return (SplineIndex + 1) % TranslationSpline->GetNumberOfSplinePoints();
}

int ATranslationSupport::GetPrevSplineIndex(const int SplineIndex)
{
	int PrevSplinePoint = (SplineIndex - 1) % TranslationSpline->GetNumberOfSplinePoints();
	if (PrevSplinePoint < 0) PrevSplinePoint += TranslationSpline->GetNumberOfSplinePoints();
	return PrevSplinePoint;
}

int ATranslationSupport::GetNumberOfSplinePoints()
{
	return TranslationSpline->GetNumberOfSplinePoints();
}

void ATranslationSupport::ComputeDistanceFromSplineOrigin()
{
	//  compute distance from spline origin from inner spline index and progress to next index
	const float DistanceOriginToInner = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(InnerSplineIndex);
	const float DistanceInnerToNext = GetSplineDistanceAToB(InnerSplineIndex, GetNextSplineIndex(InnerSplineIndex));
	DistanceFromSplineOrigin = DistanceOriginToInner + (DistanceInnerToNext * ProgressToNextIndex);
}

void ATranslationSupport::ComputeInnerIndexAndProgress()
{
	//  compute inner spline index and progress to next index with distance from spline origin
	float EvaluateIndex = 0;
	while (DistanceFromSplineOrigin >= TranslationSpline->GetDistanceAlongSplineAtSplinePoint(EvaluateIndex))
	{
		EvaluateIndex++;
	}
	InnerSplineIndex = EvaluateIndex - 1;

	const float DistanceInnerToNext = GetSplineDistanceAToB(InnerSplineIndex, GetNextSplineIndex(InnerSplineIndex)); //  distance between the spline point at 'InnerSplineIndex' and the next spline point
	const float DistanceInnerToCurr = DistanceFromSplineOrigin - TranslationSpline->GetDistanceAlongSplineAtSplinePoint(InnerSplineIndex); //  distance between the spline point at 'InnerSplineIndex' and the real position of the support on the spline
	ProgressToNextIndex = DistanceInnerToCurr / DistanceInnerToNext;
}


// ======================================================
//                   Get Transform
// ======================================================
FTransform ATranslationSupport::GetObjectTransform()
{
	return TranslationBase->GetComponentTransform();
}

// ======================================================
//                  Utility (Moving Base)
// ======================================================
bool ATranslationSupport::IsCurrentlyMoving()
{
	return CurrentTranslationState != ETranslationState::NotMoving;
}

bool ATranslationSupport::GetPlayerInRange(FVector PlayerPosition)
{
	return false;
}



// ======================================================
//                     Level Editor
// ======================================================
void ATranslationSupport::ApplyEditorValues()
{
	EditorSplineIndex = EditorSplineIndex % TranslationSpline->GetNumberOfSplinePoints();

	ComputeChildrensOffsetEditor();

	TranslationBase->SetRelativeLocation(TranslationSpline->GetLocationAtSplinePoint(EditorSplineIndex, ESplineCoordinateSpace::Local));

	ApplyChildLevelEditor();
}

void ATranslationSupport::UpdateSplinePointsVisual()
{
	//  reset the spline point visual arrows array
	for (auto SplinePointsVisualArrow : SplinePointsVisualArrows)
	{
		if (!IsValid(SplinePointsVisualArrow)) continue;
		SplinePointsVisualArrow->DestroyComponent();
	}
	SplinePointsVisualArrows.Empty();

	//  check if the spline point visual arrow is needed
	if (TranslationSpline->GetNumberOfSplinePoints() <= 0) return;
	if (!bShowSplinePointsVisual) return;

	//  construct the new arrow components
	SplinePointsVisualArrows.Reserve(TranslationSpline->GetNumberOfSplinePoints());
	for (int i = 0; i < TranslationSpline->GetNumberOfSplinePoints(); i++)
	{
		SplinePointsVisualArrows.Add(Cast<UArrowComponent>(AddComponentByClass(UArrowComponent::StaticClass(), false, FTransform::Identity, false)));
		UArrowComponent* SplineArrowComp = SplinePointsVisualArrows[i];
		SplineArrowComp->SetupAttachment(SceneRootComponent);
		SplineArrowComp->SetHiddenInGame(false);

		SplineArrowComp->SetArrowColor(FColor::Blue);
		SplineArrowComp->SetWorldScale3D(FVector{ 0.4f, 0.8f, 0.8f });
		SplineArrowComp->SetWorldRotation(FRotator{ -90.0f, 0.0f, 0.0f });
		SplineArrowComp->SetWorldLocation(TranslationSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World) + (FVector::UpVector * 30.0f));
	}
}
