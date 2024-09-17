#include "RotationBehaviorControlled.h"
#include "RotationSupport.h"
#include "ControlledRotationDatas.h"
#include "Kismet/GameplayStatics.h"
#include "Defines.h"

URotationBehaviorControlled::URotationBehaviorControlled()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URotationBehaviorControlled::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	InitializeOwner();
}


// ======================================================
//              Tick (only used for snap)
// ======================================================
void URotationBehaviorControlled::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


// ======================================================
//             Control Controlled Rotation
// ======================================================
bool URotationBehaviorControlled::StartControlledRotation(FControlledRotationDatas Datas)
{
	//  check validities
	if (!bOwnerRotSupportValid) return false;
	if (!IsControlledRotValid(Datas)) return false;

	//  check already in control
	if (CurrentState != EControlledRotationState::Inactive && CurrentState != EControlledRotationState::Snap) return false;

	//  set controlled rot values
	RotationSpeed = Datas.GetRotationSpeed();

	//  set startup values & state
	if (IsStartupValid(Datas))
	{
		bUseStartup = true;
		StartupTimer = 0.0f;
		StartupDuration = Datas.GetStartupDuration();
		StartupCurve = Datas.GetStartupCurve();
		CurrentState = EControlledRotationState::ControlStartup;
	}
	else
	{
		bUseStartup = false;
		CurrentState = EControlledRotationState::Control;
	}

	//  set state on support and start movement on childrens
	OwnerRotSupport->CurrentRotationState = ERotationState::ControlledRotation;
	OwnerRotSupport->StartMovementOnChildrens();

	return true;
}

void URotationBehaviorControlled::StopControlledRotation(bool DontTriggerSnap)
{
	//  check validity
	if (!bOwnerRotSupportValid) return;

	//  check already in control
	if (CurrentState != EControlledRotationState::Control && CurrentState != EControlledRotationState::ControlStartup) return;

	//  TODO: Implement snap (aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa)

	//  set state for this component
	CurrentState = EControlledRotationState::Inactive;

	//  set state on support and stop movement on childrens
	OwnerRotSupport->CurrentRotationState = ERotationState::NotRotating;
	OwnerRotSupport->StopMovementOnChildrens();
}

bool URotationBehaviorControlled::UpdateControlledRotation(float ControlValue)
{
	//  check validity
	if (!bOwnerRotSupportValid) return false;

	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	bool TriggeredClamp = false;
	float DesiredAngleAdd = 0.0f;

	switch (CurrentState)
	{
	case EControlledRotationState::ControlStartup:
		StartupTimer += DeltaTime;
		if (StartupTimer >= StartupDuration)
		{
			StartupTimer = StartupDuration;
			CurrentState = EControlledRotationState::Control;
		}

		DesiredAngleAdd = ControlValue * RotationSpeed * DeltaTime * StartupCurve->GetFloatValue(StartupTimer / StartupDuration);
		break;

	case EControlledRotationState::Control:
		DesiredAngleAdd = ControlValue * RotationSpeed * DeltaTime;
		break;
	}

	TriggeredClamp = OwnerRotSupport->AddInnerRotation(DesiredAngleAdd, true);


	if (ControlValue == 0.0f) //  The player stopped the rotation (joystick) but kept the control (interaction button)
	{
		if (bUseStartup) //  reset the startup (if using startup)
		{
			StartupTimer = 0.0f;
			CurrentState = EControlledRotationState::ControlStartup;
		}
	}

	return TriggeredClamp;
}


// ======================================================
//                   Helper Functions
// ======================================================
bool URotationBehaviorControlled::IsControlledRotValid(FControlledRotationDatas Datas)
{
	if (!Datas.IsDataValid()) return false;

	return Datas.GetRotationSpeed() != 0.0f;
}

bool URotationBehaviorControlled::IsStartupValid(FControlledRotationDatas Datas)
{
	return Datas.GetStartupDuration() > 0.0f && IsValid(Datas.GetStartupCurve());
}
