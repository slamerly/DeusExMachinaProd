#include "TranslationBehaviorAutomatic.h"
#include "TranslationSupport.h"
#include "Defines.h"

UTranslationBehaviorAutomatic::UTranslationBehaviorAutomatic()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// ======================================================
//                     Begin Play
// ======================================================
void UTranslationBehaviorAutomatic::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &UTranslationBehaviorAutomatic::LateBeginPlay);

	SetComponentTickEnabled(false);

	InitializeOwner();
}

void UTranslationBehaviorAutomatic::LateBeginPlay()
{
	if (!bOwnerTransSupportValid) return;
	if (!AutomaticTranslationValues.IsDataValid()) return;

	if (AutomaticTranslationValues.GetStartAutomatic())
	{
		StartAutomaticTranslation();
	}
}



// ======================================================
//                        Tick
// ======================================================
void UTranslationBehaviorAutomatic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bOwnerTransSupportValid) return;

	switch (CurrentState)
	{
	//  compute the normal phase of the automatic translation
	case EAutoTranslationState::AutomaticTranslation:
		OwnerTransSupport->AddTranslationAlongSpline(AutomaticTranslationSpeed * DeltaTime);
		break;

	//  compute the start phase of the automatic translation
	case EAutoTranslationState::StartPhase:
		PhaseTimer += DeltaTime;
		if (PhaseTimer >= PhaseTime)
		{
			PhaseTimer = PhaseTime;
			CurrentState = EAutoTranslationState::AutomaticTranslation;
		}
		OwnerTransSupport->AddTranslationAlongSpline(AutomaticTranslationSpeed * DeltaTime * PhaseCurve->GetFloatValue(PhaseTimer / PhaseTime));
		break;

	//  compute the end phase of the automatic translation
	case EAutoTranslationState::EndPhase:
		PhaseTimer += DeltaTime;
		if (PhaseTimer >= PhaseTime)
		{
			CancelAutomaticTranslation();
			break;
		}
		OwnerTransSupport->AddTranslationAlongSpline(AutomaticTranslationSpeed * DeltaTime * PhaseCurve->GetFloatValue(PhaseTimer / PhaseTime));
		break;
	}
}



// ======================================================
//             Control Automatic Translation
// ======================================================
void UTranslationBehaviorAutomatic::StartAutomaticTranslation(bool bForceNoStartPhase)
{
	//  check validities
	if (!bOwnerTransSupportValid) return;
	if (!AutomaticTranslationValues.IsDataValid()) return;

	//  set translation speed
	AutomaticTranslationSpeed = AutomaticTranslationValues.GetTranslationSpeed();
	if (bRunningReverse) AutomaticTranslationSpeed *= -1.0f;

	//  set start phase values
	PhaseTimer = 0.0f;
	PhaseTime = bForceNoStartPhase ? 0.0f : AutomaticTranslationValues.GetStartDuration();
	PhaseCurve = AutomaticTranslationValues.GetStartCurve();

	//  set state and tick for this component
	CurrentState = (bForceNoStartPhase || !IsStartPhaseValid()) ? EAutoTranslationState::AutomaticTranslation : EAutoTranslationState::StartPhase;
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerTransSupport->CurrentTranslationState = ETranslationState::AutomaticTranslation;
	OwnerTransSupport->StartMovementOnChildrens();
}

void UTranslationBehaviorAutomatic::StopAutomaticTranslation(bool bForceNoEndPhase)
{
	//  check validities
	if (!bOwnerTransSupportValid) return;
	if (!AutomaticTranslationValues.IsDataValid()) return;

	if (bForceNoEndPhase || !IsEndPhaseValid())
	{
		//  stop instantly if there is no end phase
		CancelAutomaticTranslation();
		return;
	}

	//  set translation speed
	AutomaticTranslationSpeed = AutomaticTranslationValues.GetTranslationSpeed();
	if (bRunningReverse) AutomaticTranslationSpeed *= -1.0f;

	//  set end phase values
	PhaseTimer = 0.0f;
	PhaseTime = AutomaticTranslationValues.GetEndDuration();
	PhaseCurve = AutomaticTranslationValues.GetEndCurve();

	//  set state and tick for this component
	SetComponentTickEnabled(true);
	CurrentState = EAutoTranslationState::EndPhase;

	//  set state on support and start movement on childrens (should not be useful but we never know)
	OwnerTransSupport->CurrentTranslationState = ETranslationState::AutomaticTranslation;
	OwnerTransSupport->StartMovementOnChildrens();
}

void UTranslationBehaviorAutomatic::CancelAutomaticTranslation()
{
	SetComponentTickEnabled(false);
	CurrentState = EAutoTranslationState::Inactive;

	OwnerTransSupport->CurrentTranslationState = ETranslationState::NotMoving;
	OwnerTransSupport->StopMovementOnChildrens();
}



// ======================================================
//                   Helper Functions
// ======================================================
bool UTranslationBehaviorAutomatic::IsStartPhaseValid()
{
	if (!AutomaticTranslationValues.IsDataValid()) return false;

	return AutomaticTranslationValues.GetStartDuration() > 0.0f && IsValid(AutomaticTranslationValues.GetStartCurve());
}

bool UTranslationBehaviorAutomatic::IsEndPhaseValid()
{
	if (!AutomaticTranslationValues.IsDataValid()) return false;

	return AutomaticTranslationValues.GetEndDuration() > 0.0f && IsValid(AutomaticTranslationValues.GetEndCurve());
}
