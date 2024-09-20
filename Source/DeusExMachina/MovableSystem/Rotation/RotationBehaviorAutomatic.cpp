#include "RotationBehaviorAutomatic.h"
#include "RotationSupport.h"
#include "DeusExMachina/MovableSystem/Interactables/AutoRotInteractionDatas.h"


URotationBehaviorAutomatic::URotationBehaviorAutomatic()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// ======================================================
//                     Begin Play
// ======================================================
void URotationBehaviorAutomatic::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &URotationBehaviorAutomatic::LateBeginPlay);

	SetComponentTickEnabled(false);

	InitializeOwner();
}

void URotationBehaviorAutomatic::LateBeginPlay()
{
	if (!bOwnerRotSupportValid) return;
	if (!AutomaticRotationValues.IsDataValid()) return;

	if (AutomaticRotationValues.GetStartAutomatic())
	{
		StartAutomaticRotation();
	}
}



// ======================================================
//                        Tick
// ======================================================
void URotationBehaviorAutomatic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bOwnerRotSupportValid) return;

	switch (CurrentState)
	{
	//  compute the normal phase of the automatic rotation
	case EAutoRotationState::AutomaticRotation:
		OwnerRotSupport->AddInnerRotation(AutomaticRotationSpeed * DeltaTime, false);
		break;

	//  compute the start phase of the automatic rotation
	case EAutoRotationState::StartPhase:
		PhaseTimer += DeltaTime;
		if (PhaseTimer >= PhaseTime)
		{
			PhaseTimer = PhaseTime;
			CurrentState = EAutoRotationState::AutomaticRotation;
		}
		OwnerRotSupport->AddInnerRotation(AutomaticRotationSpeed * DeltaTime * PhaseCurve->GetFloatValue(PhaseTimer / PhaseTime), false);
		break;

	//  compute the end phase of the automatic rotation
	case EAutoRotationState::EndPhase:
		PhaseTimer += DeltaTime;
		if (PhaseTimer >= PhaseTime)
		{
			CancelAutomaticRotation();
			break;
		}
		OwnerRotSupport->AddInnerRotation(AutomaticRotationSpeed * DeltaTime * PhaseCurve->GetFloatValue(PhaseTimer / PhaseTime), false);
		break;
	}
}



// ======================================================
//              Control Automatic Rotation
// ======================================================
void URotationBehaviorAutomatic::StartAutomaticRotation(bool bForceNoStartPhase)
{
	//  check validities
	if (!bOwnerRotSupportValid) return;
	if (!AutomaticRotationValues.IsDataValid()) return;

	//  set rotation speed
	AutomaticRotationSpeed = AutomaticRotationValues.GetRotationSpeed();
	if (bRunningReverse) AutomaticRotationSpeed *= -1.0f;

	//  set start phase values
	PhaseTimer = 0.0f;
	PhaseTime = bForceNoStartPhase ? 0.0f : AutomaticRotationValues.GetStartDuration();
	PhaseCurve = AutomaticRotationValues.GetStartCurve();

	//  set state and tick for this component
	CurrentState = (bForceNoStartPhase || !IsStartPhaseValid()) ? EAutoRotationState::AutomaticRotation : EAutoRotationState::StartPhase;
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerRotSupport->CurrentRotationState = ERotationState::AutomaticRotation;
	OwnerRotSupport->StartMovementOnChildrens();
}

void URotationBehaviorAutomatic::StopAutomaticRotation(bool bForceNoEndPhase)
{
	//  check validities
	if (!bOwnerRotSupportValid) return;
	if (!AutomaticRotationValues.IsDataValid()) return;

	if (bForceNoEndPhase || !IsEndPhaseValid())
	{
		//  stop instantly if there is no end phase
		CancelAutomaticRotation();
		return;
	}

	//  set rotation speed
	AutomaticRotationSpeed = AutomaticRotationValues.GetRotationSpeed();
	if (bRunningReverse) AutomaticRotationSpeed *= -1.0f;

	//  set end phase values
	PhaseTimer = 0.0f;
	PhaseTime = AutomaticRotationValues.GetEndDuration();
	PhaseCurve = AutomaticRotationValues.GetEndCurve();

	//  set state and tick for this component
	SetComponentTickEnabled(true);
	CurrentState = EAutoRotationState::EndPhase;

	//  set state on support and start movement on childrens (should not be useful but we never know)
	OwnerRotSupport->CurrentRotationState = ERotationState::AutomaticRotation;
	OwnerRotSupport->StartMovementOnChildrens();
}

void URotationBehaviorAutomatic::CancelAutomaticRotation()
{
	SetComponentTickEnabled(false);
	CurrentState = EAutoRotationState::Inactive;

	OwnerRotSupport->CurrentRotationState = ERotationState::NotRotating;
	OwnerRotSupport->StopMovementOnChildrens();
}



// ======================================================
//                     Interaction
// ======================================================
void URotationBehaviorAutomatic::TriggerAutoRotInteraction(FAutoRotInteractionDatas Datas)
{
	if (!bOwnerRotSupportValid) return;

	if (Datas.GetStartStop())
	{
		if (CurrentState == EAutoRotationState::AutomaticRotation || CurrentState == EAutoRotationState::StartPhase)
		{
			StopAutomaticRotation();
		}
		else
		{
			if (Datas.GetReverse())
			{
				bRunningReverse = !bRunningReverse;
			}
			StartAutomaticRotation();
		}
	}
	else
	{
		if (Datas.GetReverse())
		{
			bRunningReverse = !bRunningReverse;
			if (CurrentState == EAutoRotationState::AutomaticRotation || CurrentState == EAutoRotationState::StartPhase)
			{
				StartAutomaticRotation();
			}
		}
	}
}



// ======================================================
//                   Helper Functions
// ======================================================
bool URotationBehaviorAutomatic::IsStartPhaseValid()
{
	if (!AutomaticRotationValues.IsDataValid()) return false;

	return AutomaticRotationValues.GetStartDuration() > 0.0f && IsValid(AutomaticRotationValues.GetStartCurve());
}

bool URotationBehaviorAutomatic::IsEndPhaseValid()
{
	if (!AutomaticRotationValues.IsDataValid()) return false;

	return AutomaticRotationValues.GetEndDuration() > 0.0f && IsValid(AutomaticRotationValues.GetEndCurve());
}
