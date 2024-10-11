#include "TranslationSupport.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DeusExMachina/Utils/SplineComponentPlus.h"
#include "Components/ArrowComponent.h"
#include "DeusExMachina/MovableSystem/MovableObjectComponent.h"

#include "TranslationBehaviorAutomatic.h"
#include "TranslationBehaviorControlled.h"
#include "TranslationBehaviorStandard.h"

#if WITH_EDITOR
#include "LevelEditor.h"
#endif // WITH_EDITOR

#include "Kismet/KismetMathLibrary.h"
#include "Defines.h"


// ======================================================
//                    Constructor
// ======================================================
ATranslationSupport::ATranslationSupport()
{
	PrimaryActorTick.bCanEverTick = false;

	//  create default components

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SceneRootComponent->SetupAttachment(GetRootComponent());

	TranslationBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Translation Base"));
	TranslationBase->SetupAttachment(SceneRootComponent);

	TranslationSpline = CreateDefaultSubobject<USplineComponentPlus>(TEXT("Translation Spline"));
	TranslationSpline->SetupAttachment(SceneRootComponent);
}


// ======================================================
//                    Begin Play
// ======================================================
void ATranslationSupport::BeginPlay()
{
	//  call parent begin play to compute self movable
	Super::BeginPlay();

	//  'EditorSplineIndex' serve as a begin play spline index, initialize 'InnerSplineIndex' with it
	InnerSplineIndex = EditorSplineIndex % TranslationSpline->GetNumberOfSplinePoints();
	ProgressToNextIndex = 0.0f;
	ComputeDistanceFromSplineOrigin();

	//  reset the relative location of rotation base to prevent unexpected behavior in game
	TranslationBase->SetRelativeLocation(TranslationSpline->GetLocationAtSplinePoint(EditorSplineIndex, ESplineCoordinateSpace::Local));

	//  hide support and remove collision (if necessary)
	TranslationBase->SetHiddenInGame(bDisableSupportVisibility);
	TranslationBase->SetCollisionEnabled(bDisableSupportCollision ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	//  show spline points visual if needed
	UpdateSplinePointsVisual();
}



// ======================================================
//             Inner Translation Functions
// ======================================================
void ATranslationSupport::AddTranslationAlongSpline(const float TranslationAdd)
{
	const float IndexBefore = InnerSplineIndex;

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

	//  check the objectives for Event Dispatcher
	CheckSplinePointObjective(IndexBefore, ProgressToNextIndex, InnerSplineIndex);
}

void ATranslationSupport::ForcePositionOnSpline(const float DistanceFromOrigin)
{
	const float IndexBefore = InnerSplineIndex;

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

	//  check the objectives for Event Dispatcher
	CheckSplinePointObjective(IndexBefore, ProgressToNextIndex, InnerSplineIndex);
}

void ATranslationSupport::ForcePositionOnSpline(const int SplineIndex, const float ProgressToNextPoint)
{
	const float IndexBefore = InnerSplineIndex;

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
		ProgressToNextIndex = UKismetMathLibrary::GenericPercent_FloatFloat(ProgressToNextIndex, 1.0f);
	}
	else if (ProgressToNextIndex < 0.0f)
	{
		ProgressToNextIndex = UKismetMathLibrary::GenericPercent_FloatFloat(ProgressToNextIndex, 1.0f);
		ProgressToNextIndex += 1.0f; //  modulo does not get the number above 0.0f
	}

	//  compute the distance on the spline from the spline origin
	ComputeDistanceFromSplineOrigin();


	//  apply the support movement
	ComputeInnerTransform();

	//  check the objectives for Event Dispatcher
	CheckSplinePointObjective(IndexBefore, ProgressToNextIndex, InnerSplineIndex);
}



// ======================================================
//           Compute Inner Translation Helpers
// ======================================================
void ATranslationSupport::ComputeInnerTransform()
{
	OnTranslationSupportMoved.Broadcast(InnerSplineIndex, ProgressToNextIndex);

	//  simply set the relative location to the current location on the spline
	TranslationBase->SetRelativeLocation(TranslationSpline->GetLocationAtDistanceAlongSpline(DistanceFromSplineOrigin, ESplineCoordinateSpace::Local));
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
	ComputeInnerIndexAndProgress(DistanceFromSplineOrigin, InnerSplineIndex, ProgressToNextIndex);
}

void ATranslationSupport::ComputeInnerIndexAndProgress(const float DistanceFromOrigin, int& OutIndex, float& OutProgress)
{
	const float DistanceOrigin = UKismetMathLibrary::GenericPercent_FloatFloat(DistanceFromOrigin, TranslationSpline->GetSplineLength());

	//  compute inner spline index and progress to next index with distance from spline origin, and return it
	float EvaluateIndex = 0;
	while (DistanceOrigin >= TranslationSpline->GetDistanceAlongSplineAtSplinePoint(EvaluateIndex))
	{
		EvaluateIndex++;
	}
	OutIndex = EvaluateIndex - 1;

	const float DistanceInnerToNext = GetSplineDistanceAToB(OutIndex, GetNextSplineIndex(OutIndex)); //  distance between the spline point at 'OutIndex' and the next spline point
	const float DistanceInnerToCurr = DistanceOrigin - TranslationSpline->GetDistanceAlongSplineAtSplinePoint(OutIndex); //  distance between the spline point at 'OutIndex' and the inputed position on the spline
	OutProgress = DistanceInnerToCurr / DistanceInnerToNext;
}



// ======================================================
//               Inner Translation Getters
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



// ======================================================
//                    Clamp Functions
// ======================================================
float ATranslationSupport::ClampMovementBetweenSplinePoints(const float Movement, const int SplineIndexA, const int SplineIndexB, int& ClampIndex)
{
	const float DistanceOA = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(SplineIndexA);
	const float DistanceOB = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(SplineIndexB);
	const float PreAppliedMovement = DistanceFromSplineOrigin + Movement;

	if (SplineIndexA == SplineIndexB)
	{
		//  return a movement that will put the support on point A (and on point B since they're the same in this case)
		ClampIndex = SplineIndexA;
		return DistanceOA - DistanceFromSplineOrigin;
	}
	else if (SplineIndexA < SplineIndexB)
	{
		//  the simple computation, no need to get around the spline origin
		if (PreAppliedMovement > DistanceOB)
		{
			ClampIndex = SplineIndexB;
			return DistanceOB - DistanceFromSplineOrigin;
		}
		else if (PreAppliedMovement < DistanceOA)
		{
			ClampIndex = SplineIndexA;
			return DistanceOA - DistanceFromSplineOrigin;
		}

		//  all good
		ClampIndex = InnerSplineIndex;
		return Movement;
	}
	else if (SplineIndexA > SplineIndexB) //  it's just a else in this case, but for better understandability I let the if condition visible
	{
		//  the (slightly) harder computation, need to get around the spline origin
		const float DistanceOBAround = TranslationSpline->GetSplineLength() + DistanceOB;

		if (PreAppliedMovement > DistanceOBAround)
		{
			ClampIndex = SplineIndexB;
			return DistanceOBAround - DistanceFromSplineOrigin;
		}
		else if (PreAppliedMovement < DistanceOA)
		{
			ClampIndex = SplineIndexA;
			return DistanceOA - DistanceFromSplineOrigin;
		}

		//  all good
		ClampIndex = InnerSplineIndex;
		return Movement;
	}

	ClampIndex = InnerSplineIndex;
	return Movement; //  for the compiler
}



// ======================================================
//                    Snap Functions
// ======================================================
int ATranslationSupport::SearchNearestSplinePointToSnap(const float InputDistanceOnSpline, const float SnapSearchAdvantage)
{
	//  same code base than 'ComputeInnerIndexAndProgress()' but different utility
	float EvaluateIndex = 0;
	while (InputDistanceOnSpline >= TranslationSpline->GetDistanceAlongSplineAtSplinePoint(EvaluateIndex))
	{
		EvaluateIndex++;
		if (EvaluateIndex >= TranslationSpline->GetNumberOfSplinePoints()) return 0; //  security
	}

	//  retrieve snap points that are around the 'InputDistanceOnSpline'
	const float LowerSnap = GetPrevSplineIndex(EvaluateIndex);
	const float HigherSnap = GetNextSplineIndex(LowerSnap);

	//  compute the progress of 'InputDistanceOnSpline' between found snap points 
	const float DistanceLowerToHigher = GetSplineDistanceAToB(LowerSnap, HigherSnap); //  distance between the lower snap point and the higher snap point
	const float DistanceLowerToInputDist = InputDistanceOnSpline - TranslationSpline->GetDistanceAlongSplineAtSplinePoint(LowerSnap); //  distance between the lower spline point and the inputed distance on spline
	const float SnapProgress = DistanceLowerToInputDist / DistanceLowerToHigher;


	//  compute the snap search advantage
	const float SearchAdvantage = FMath::Clamp(SnapSearchAdvantage, 0.0f, 1.0f);
	if (SearchAdvantage == 0.0f) return LowerSnap;
	if (SearchAdvantage == 1.0f) return HigherSnap;

	const float AdvantagedSnapProgress = SnapProgress + (SearchAdvantage - 0.5f);
	if (AdvantagedSnapProgress >= 0.5f)
	{
		return HigherSnap;
	}
	else
	{
		return LowerSnap;
	}
}



// ======================================================
//                Spline Distance Functions
// ======================================================
float ATranslationSupport::GetSplineDistanceToNextSplinePoint()
{
	return GetSplineDistanceToPoint(GetNextSplineIndex(InnerSplineIndex));
}

float ATranslationSupport::GetSplineDistanceToPoint(const int SplineIndexA)
{
	//  compute distance from inner spline position to spline index A
	const float DistanceOA = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(SplineIndexA);
	float DistanceIA = DistanceOA - DistanceFromSplineOrigin; //  I for inner

	//  if spline index A was before inner spline position in the spline direction, return the distance including the loop of the spline
	if (DistanceIA < 0.0f) DistanceIA += TranslationSpline->GetSplineLength();
	return DistanceIA;
}

float ATranslationSupport::GetSplineDistanceToPointReversed(const int SplineIndexA)
{
	//  compute distance from spline index A to inner spline position 
	//  (gives the distance from inner spline pos to index A but going in the reverse direction of the spline)
	const float DistanceOA = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(SplineIndexA);
	float DistanceAI = DistanceFromSplineOrigin - DistanceOA; //  I for inner

	//  if inner spline position was before spline index A in the spline direction, return the distance including the loop of the spline
	if (DistanceAI < 0.0f) DistanceAI += TranslationSpline->GetSplineLength();
	return DistanceAI;
}

float ATranslationSupport::GetSplineDistanceAToB(const int SplineIndexA, const int SplineIndexB)
{
	//  compute distance from spline index A to spline index B
	const float DistanceOA = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(SplineIndexA);
	const float DistanceOB = TranslationSpline->GetDistanceAlongSplineAtSplinePoint(SplineIndexB);
	float DistanceAB = DistanceOB - DistanceOA;

	//  if spline index B was before spline index A in the spline direction, return the distance including the loop of the spline
	if (DistanceAB < 0.0f) DistanceAB += TranslationSpline->GetSplineLength();
	return DistanceAB;
}



// ======================================================
//                     Spline Getters
// ======================================================
int ATranslationSupport::GetNextSplineIndex(const int SplineIndex)
{
	return (SplineIndex + 1) % TranslationSpline->GetNumberOfSplinePoints();
}

int ATranslationSupport::GetPrevSplineIndex(const int SplineIndex)
{
	int PrevSplinePoint = (SplineIndex - 1) % TranslationSpline->GetNumberOfSplinePoints();
	if (PrevSplinePoint < 0) PrevSplinePoint += TranslationSpline->GetNumberOfSplinePoints(); //  modulo does not goes back above 0
	return PrevSplinePoint;
}

int ATranslationSupport::GetNumberOfSplinePoints()
{
	return TranslationSpline->GetNumberOfSplinePoints();
}

float ATranslationSupport::GetFullSplineLength()
{
	return TranslationSpline->GetSplineLength();
}



// ======================================================
//            Moving Support Base Functions
// ======================================================
FTransform ATranslationSupport::GetObjectTransform()
{
	return TranslationBase->GetComponentTransform();
}

bool ATranslationSupport::IsCurrentlyMoving()
{
	return CurrentTranslationState != ETranslationState::NotMoving;
}



// ======================================================
//                     Spline Visual
// ======================================================
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



// ======================================================
//            Translation Behaviors Components
// ======================================================
void ATranslationSupport::SetupTranslationBehaviors()
{
	//  process for each behavior type

	//  automatic behavior
	if (TranslationBehaviorFlags & (uint8)ETranslationBehaviorFlags::Automatic)
	{
		//  check if the component already exists (no need to create it)
		TInlineComponentArray<UTranslationBehaviorAutomatic*> AutomaticComps(this);
		GetComponents<UTranslationBehaviorAutomatic*>(AutomaticComps);

		if (AutomaticComps.Num() == 0)
		{
			//  no automatic behavior component, create one
			UActorComponent* AutomaticBehaviorComp = AddComponentByClass(UTranslationBehaviorAutomatic::StaticClass(), false, FTransform::Identity, false);
			AddInstanceComponent(AutomaticBehaviorComp);
		}
	}
	else
	{
		//  check if the component exists (need to remove it)
		TInlineComponentArray<UTranslationBehaviorAutomatic*> AutomaticComps(this);
		GetComponents<UTranslationBehaviorAutomatic*>(AutomaticComps);

		for (auto AutomaticComponent : AutomaticComps)
		{
			AutomaticComponent->DestroyComponent();
		}
	}

	//  controlled behavior
	if (TranslationBehaviorFlags & (uint8)ETranslationBehaviorFlags::Controlled)
	{
		//  check if the component already exists (no need to create it)
		TInlineComponentArray<UTranslationBehaviorControlled*> ControlledComps(this);
		GetComponents<UTranslationBehaviorControlled*>(ControlledComps);

		if (ControlledComps.Num() == 0)
		{
			//  no controlled behavior component, create one
			UActorComponent* ControlledBehaviorComp = AddComponentByClass(UTranslationBehaviorControlled::StaticClass(), false, FTransform::Identity, false);
			AddInstanceComponent(ControlledBehaviorComp);
		}
	}
	else
	{
		//  check if the component exists (need to remove it)
		TInlineComponentArray<UTranslationBehaviorControlled*> ControlledComps(this);
		GetComponents<UTranslationBehaviorControlled*>(ControlledComps);

		for (auto ControlledComponent : ControlledComps)
		{
			ControlledComponent->DestroyComponent();
		}
	}

	//  standard behavior
	if (TranslationBehaviorFlags & (uint8)ETranslationBehaviorFlags::Standard)
	{
		//  check if the component already exists (no need to create it)
		TInlineComponentArray<UTranslationBehaviorStandard*> StandardComps(this);
		GetComponents<UTranslationBehaviorStandard*>(StandardComps);

		if (StandardComps.Num() == 0)
		{
			//  no standard behavior component, create one
			UActorComponent* StandardBehaviorComp = AddComponentByClass(UTranslationBehaviorStandard::StaticClass(), false, FTransform::Identity, false);
			AddInstanceComponent(StandardBehaviorComp);
		}
	}
	else
	{
		//  check if the component exists (need to remove it)
		TInlineComponentArray<UTranslationBehaviorStandard*> StandardComps(this);
		GetComponents<UTranslationBehaviorStandard*>(StandardComps);

		for (auto StandardComponent : StandardComps)
		{
			StandardComponent->DestroyComponent();
		}
	}


#if WITH_EDITOR
	//  refresh detail panel to show created components
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.BroadcastComponentsEdited();
	LevelEditorModule.BroadcastRedrawViewports(false);
#endif // WITH_EDITOR
}



// ======================================================
//              Objective Event Dispatcher
// ======================================================
void ATranslationSupport::CheckSplinePointObjective(const int IndexBefore, const float ProgressAfter, const int IndexAfter)
{
	//  check index after if progress after is zero

	if (ProgressAfter == 0.0f)
	{
		//  check loop preparation
		bool bObjectiveValid{ false };
		FGameplayTag EventTag;

		//  check loop
		for (auto& SplinePointObjective : SplinePointObjectives)
		{
			if (IndexAfter == SplinePointObjective.ObjectiveSplinePoint)
			{
				bObjectiveValid = true;
				EventTag = SplinePointObjective.ObjectiveEventTag;
			}
		}

		//  call event if found (then return cause objective is reached)
		if (bObjectiveValid)
		{
			CallEventDispatcherHub(EventTag);
			return;
		}
	}


	//  check if the movement has passed by a spline point
	if (IndexBefore == IndexAfter) return;


	//  check sweep

	//  check loop preparation

	int SplineIndexCheck{ 0 };
	if (GetPrevSplineIndex(IndexAfter) == IndexBefore)
	{
		SplineIndexCheck = IndexAfter;
	}
	else if (GetNextSplineIndex(IndexAfter) == IndexBefore)
	{
		SplineIndexCheck = IndexBefore;
	}
	else
	{
		//  there is two spline points (or more) between the movement, consider that to be a teleportation, eg. do not check sweep
		return;
	}

	bool bObjectiveValid{ false };
	FGameplayTag EventTag;

	//  check loop
	for (auto& SplinePointObjective : SplinePointObjectives)
	{
		if (SplineIndexCheck == SplinePointObjective.ObjectiveSplinePoint)
		{
			bObjectiveValid = true;
			EventTag = SplinePointObjective.ObjectiveEventTag;
		}
	}

	//  call event if found
	if (bObjectiveValid)
	{
		CallEventDispatcherHub(EventTag);
	}
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