#include "TranslationBehaviorControlled.h"
#include "TranslationSupport.h"
#include "Kismet/GameplayStatics.h"
#include "Defines.h"

UTranslationBehaviorControlled::UTranslationBehaviorControlled()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTranslationBehaviorControlled::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	InitializeOwner();
}


// ======================================================
//                     Tick (snap)
// ======================================================
void UTranslationBehaviorControlled::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


// ======================================================
//             Control Controlled Translation
// ======================================================
bool UTranslationBehaviorControlled::StartControlledTranslation(FControlledTranslationDatas Datas)
{
	//  check validities
	if (!bOwnerTransSupportValid) return false;
	if (!IsControlledTransValid(Datas)) return false;

	//  check already in control
	if (CurrentState != EControlledTranslationState::Inactive && CurrentState != EControlledTranslationState::Snap) return false;

	//  set controlled rot values
	CurrentDatas = Datas;
	TranslationSpeed = Datas.GetTranslationSpeed();

	//  set startup values & state
	if (IsStartupValid(Datas))
	{
		bUseStartup = true;
		StartupTimer = 0.0f;
		StartupDuration = Datas.GetStartupDuration();
		StartupCurve = Datas.GetStartupCurve();
		CurrentState = EControlledTranslationState::ControlStartup;
	}
	else
	{
		bUseStartup = false;
		CurrentState = EControlledTranslationState::Control;
	}

	//  enable tick on this component
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerTransSupport->CurrentTranslationState = ETranslationState::ControlledTranslation;
	OwnerTransSupport->StartMovementOnChildrens();

	return true;
}

void UTranslationBehaviorControlled::StopControlledTranslation(bool DontTriggerSnap)
{
	//  check validity
	if (!bOwnerTransSupportValid) return;

	//  check already in control
	if (CurrentState != EControlledTranslationState::Control && CurrentState != EControlledTranslationState::ControlStartup) return;

	/*if (snap)
	{

	}
	else*/
	{
		//  set state and disable tick for this component
		SetComponentTickEnabled(false);
		CurrentState = EControlledTranslationState::Inactive;

		//  set state on support and stop movement on childrens
		OwnerTransSupport->CurrentTranslationState = ETranslationState::NotMoving;
		OwnerTransSupport->StopMovementOnChildrens();
	}
}

bool UTranslationBehaviorControlled::UpdateControlledTranslation(float ControlValue)
{
	//  check validity
	if (!bOwnerTransSupportValid) return false;

	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	bool TriggeredClamp = false;
	float DesiredDistAdd = 0.0f;

	switch (CurrentState)
	{
	case EControlledTranslationState::ControlStartup:
		StartupTimer += DeltaTime;
		if (StartupTimer >= StartupDuration)
		{
			StartupTimer = StartupDuration;
			CurrentState = EControlledTranslationState::Control;
		}

		DesiredDistAdd = ControlValue * TranslationSpeed * DeltaTime * StartupCurve->GetFloatValue(StartupTimer / StartupDuration);
		break;

	case EControlledTranslationState::Control:
		DesiredDistAdd = ControlValue * TranslationSpeed * DeltaTime;
		break;
	}

	//  clamp 'DesiredDistAdd' between spline points 0 and 1
	DesiredDistAdd = OwnerTransSupport->ClampMovementBetweenSplinePoints(DesiredDistAdd, 0, 1);

	if (FMath::IsNearlyZero(DesiredDistAdd)) TriggeredClamp = true;


	OwnerTransSupport->AddTranslationAlongSpline(DesiredDistAdd);


	if (ControlValue == 0.0f) //  The player stopped the rotation (joystick) but kept the control (interaction button)
	{
		if (bUseStartup) //  reset the startup (if using startup)
		{
			StartupTimer = 0.0f;
			CurrentState = EControlledTranslationState::ControlStartup;
		}
	}

	return TriggeredClamp;
}


// ======================================================
//                   Helper Functions
// ======================================================
bool UTranslationBehaviorControlled::IsControlledTransValid(FControlledTranslationDatas Datas)
{
	if (!Datas.IsDataValid()) return false;

	return Datas.GetTranslationSpeed() != 0.0f;
}

bool UTranslationBehaviorControlled::IsStartupValid(FControlledTranslationDatas Datas)
{
	if (!Datas.IsDataValid()) return false;

	return Datas.GetStartupDuration() > 0.0f && IsValid(Datas.GetStartupCurve());
}

