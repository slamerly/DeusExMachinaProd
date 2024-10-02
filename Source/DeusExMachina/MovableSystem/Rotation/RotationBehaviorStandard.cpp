#include "RotationBehaviorStandard.h"
#include "RotationSupport.h"


URotationBehaviorStandard::URotationBehaviorStandard()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void URotationBehaviorStandard::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	InitializeOwner(); //  RotationBehaviorBase
}


// ======================================================
//                        Tick
// ======================================================
void URotationBehaviorStandard::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bOwnerRotSupportValid) return;
	if (!bCurrentlyRotating) return;

	//  standard rotation
	RotationTimer += DeltaTime;
	if (RotationTimer >= RotationDuration)
	{
		OwnerRotSupport->ForceInnerRotation(DestinationAngle, true); //  reposition support to destination angle (security)

		if (bRunClamped) OwnerRotSupport->OnRotationSupportClamped.Broadcast(ClampAngle);

		//  timer finished, stop rotation
		CancelStandardRotation();
		return;
	}

	//  apply standard rotation
	const float DesiredAngle = RotationAngle * RotationCurve->GetFloatValue(RotationTimer / RotationDuration);
	OwnerRotSupport->AddInnerRotation(DesiredAngle - LastFrameRotAngle, true);
	
	LastFrameRotAngle = DesiredAngle;
}


// ======================================================
//              Control Standard Rotation
// ======================================================
void URotationBehaviorStandard::StartStandardRotation(FStandardRotationDatas Datas, bool bForceStart)
{
	//  check validities
	if (!bOwnerRotSupportValid) return;
	if (!IsStandardRotValid(Datas)) return;

	//  check already rotating
	if (bCurrentlyRotating && !bForceStart) return;

	//  set standard rot values
	RotationAngle = Datas.GetRotationAngle();
	RotationCurve = Datas.GetRotationCurve();
	RotationDuration = Datas.GetRotationDuration();
	RotationTimer = 0.0f;
	LastFrameRotAngle = 0.0f;

	//  check clamp on support
	float RotationAngleClamped = RotationAngle;
	if (OwnerRotSupport->SimulateRotationWithClamp(RotationAngle, RotationAngleClamped, ClampAngle))
	{
		RotationAngle = RotationAngleClamped;
		RotationDuration *= (RotationAngleClamped / RotationAngle); //  reduce the duration of the standard rotation proportionnaly to the reduction of angle rotated so the rotation keeps the same speed

		bRunClamped = true;
	}

	//  compute destination angle
	DestinationAngle = FMath::RoundToInt(OwnerRotSupport->GetInnerRotation() + RotationAngle);

	//  set rotating and tick for this component
	bCurrentlyRotating = true;
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerRotSupport->CurrentRotationState = ERotationState::StandardRotation;
	OwnerRotSupport->StartMovementOnChildrens();

	//  broadcast the OnStandardRotationStart event
	OnStandardRotationStart.Broadcast(OwnerRotSupport->GetInnerRotation() + RotationAngle, Datas);
}

void URotationBehaviorStandard::CancelStandardRotation()
{
	if (!bCurrentlyRotating) return;

	//  set rotating and tick for this component
	bCurrentlyRotating = false;
	SetComponentTickEnabled(false);

	//  set state on support and stop movement on childrens
	OwnerRotSupport->CurrentRotationState = ERotationState::NotRotating;
	OwnerRotSupport->StopMovementOnChildrens();

	//  broadcast the OnStandardRotationEnd event
	OnStandardRotationEnd.Broadcast(OwnerRotSupport->GetInnerRotation());
}


// ======================================================
//                   Helper Functions
// ======================================================
bool URotationBehaviorStandard::IsStandardRotValid(FStandardRotationDatas Datas)
{
	//  check validity of standard rotation values
	if (!Datas.IsDataValid()) return false;

	return Datas.GetRotationDuration() > 0.0f && IsValid(Datas.GetRotationCurve());
}

