#include "TranslationSupport.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DeusExMachina/Utils/SplineComponentPlus.h"
#include "Components/ArrowComponent.h"
#include "DeusExMachina/MovableSystem/MovableObjectComponent.h"
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
int ATranslationSupport::GetInnerSplineIndex()
{
	return InnerSplineIndex;
}

float ATranslationSupport::GetProgressToNextIndex()
{
	return ProgressToNextIndex;
}

bool ATranslationSupport::AddTranslationAlongSpline(const float TranslationAdd, const bool StopIfSplinePointReached)
{
	float RemainingTranslationAdd = FMath::Abs(TranslationAdd);
	bool SplinePointReached = false;

	if (TranslationAdd > 0.0f)
	{
		while (RemainingTranslationAdd > 0.0f)
		{
			const float DistanceToNextPoint = GetDistanceToNextSplinePoint();

			if (RemainingTranslationAdd >= DistanceToNextPoint)
			{
				InnerSplineIndex = GetNextSplineIndex(InnerSplineIndex);
				ProgressToNextIndex = 0.0f;
				RemainingTranslationAdd -= DistanceToNextPoint;

				SplinePointReached = true;
				if(StopIfSplinePointReached) RemainingTranslationAdd = 0.0f;
			}
			else
			{
				const float DistanceCurrentToNextPoint = GetDistanceFromToSplinePoint(InnerSplineIndex, GetNextSplineIndex(InnerSplineIndex));
				ProgressToNextIndex += (RemainingTranslationAdd / DistanceCurrentToNextPoint);
				RemainingTranslationAdd = 0.0f;
			}
		}
	}
	else if (TranslationAdd < 0.0f)
	{
		while (RemainingTranslationAdd > 0.0f)
		{
			const float DistanceToPrevPoint = GetDistanceToCurrentSplinePoint();

			if (RemainingTranslationAdd >= DistanceToPrevPoint)
			{
				InnerSplineIndex = GetPrevSplineIndex(InnerSplineIndex);
				ProgressToNextIndex = 1.0f;
				RemainingTranslationAdd -= DistanceToPrevPoint;

				SplinePointReached = true;
				if (StopIfSplinePointReached) RemainingTranslationAdd = 0.0f;
			}
			else
			{
				const float DistanceNextToCurrentPoint = GetDistanceFromToSplinePoint(GetNextSplineIndex(InnerSplineIndex), InnerSplineIndex);
				ProgressToNextIndex -= (RemainingTranslationAdd / DistanceNextToCurrentPoint);
				RemainingTranslationAdd = 0.0f;
			}
		}
	}

	ComputeInnerTransform();
	return SplinePointReached;
}

void ATranslationSupport::ForcePositionOnSpline(const int SplineIndex, const float ProgressToNextPoint)
{
	InnerSplineIndex = SplineIndex;
	ProgressToNextIndex = ProgressToNextPoint;

	ComputeInnerTransform();
}

void ATranslationSupport::ComputeInnerTransform()
{
	//  simply set the location value of InnerTransform to the delta location done since begin play
	FVector CurrentLocation = TranslationSpline->GetLocationAtSplinePoint(InnerSplineIndex, ESplineCoordinateSpace::World);

	const FVector ProgressDirection = GetDirectionFromToSplinePoint(InnerSplineIndex, GetNextSplineIndex(InnerSplineIndex));
	const float ProgressDistance = GetDistanceFromToSplinePoint(InnerSplineIndex, GetNextSplineIndex(InnerSplineIndex));
	CurrentLocation += (ProgressDirection * ProgressDistance * ProgressToNextIndex);

	InnerTransform.SetLocation((CurrentLocation - StartTransform.GetLocation()) / TranslationBase->GetComponentScale()); //  need to invert the effect of the scale on location in the apply inner movement

	ApplyInnerMovement(TranslationBase);
}


// ======================================================
//                   Utility Functions
// ======================================================
float ATranslationSupport::GetDistanceToNextSplinePoint()
{
	const int NextSplineIndex = GetNextSplineIndex(InnerSplineIndex);
	return FVector::Distance(TranslationBase->GetComponentLocation(), TranslationSpline->GetLocationAtSplinePoint(NextSplineIndex, ESplineCoordinateSpace::World));
}

float ATranslationSupport::GetDistanceToCurrentSplinePoint()
{
	return FVector::Distance(TranslationBase->GetComponentLocation(), TranslationSpline->GetLocationAtSplinePoint(InnerSplineIndex, ESplineCoordinateSpace::World));
}

FVector ATranslationSupport::GetDirectionFromToSplinePoint(const int SplineIndexA, const int SplineIndexB)
{
	const FVector FromSplinePoint = TranslationSpline->GetLocationAtSplinePoint(SplineIndexA, ESplineCoordinateSpace::World);
	const FVector ToSplinePoint = TranslationSpline->GetLocationAtSplinePoint(SplineIndexB, ESplineCoordinateSpace::World);
	return (ToSplinePoint - FromSplinePoint).GetSafeNormal();
}

float ATranslationSupport::GetDistanceFromToSplinePoint(const int SplineIndexA, const int SplineIndexB)
{
	const FVector FromSplinePoint = TranslationSpline->GetLocationAtSplinePoint(SplineIndexA, ESplineCoordinateSpace::World);
	const FVector ToSplinePoint = TranslationSpline->GetLocationAtSplinePoint(SplineIndexB, ESplineCoordinateSpace::World);
	return FVector::Distance(FromSplinePoint, ToSplinePoint);
}

int ATranslationSupport::GetNextSplineIndex(const int SplineIndex)
{
	return (SplineIndex + 1) % TranslationSpline->GetNumberOfSplinePoints();
}

int ATranslationSupport::GetPrevSplineIndex(const int SplineIndex)
{
	int PrevSplinePoint = SplineIndex - 1;
	if (PrevSplinePoint < 0) PrevSplinePoint += TranslationSpline->GetNumberOfSplinePoints();
	return PrevSplinePoint;
}


// ======================================================
//                   Get Transforms
// ======================================================
FTransform ATranslationSupport::GetObjectTransform()
{
	return TranslationBase->GetComponentTransform();
}

FTransform ATranslationSupport::GetObjectTransformRelative()
{
	return TranslationBase->GetRelativeTransform();
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
