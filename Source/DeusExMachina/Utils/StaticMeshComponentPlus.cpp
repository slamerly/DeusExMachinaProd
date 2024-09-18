#include "StaticMeshComponentPlus.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationSupport.h"
#include "Defines.h"

void UStaticMeshComponentPlus::PostEditComponentMove(bool bFinished)
{
	if (bUsedInRotationSupport) BroadcastRotSupport();

	Super::PostEditComponentMove(bFinished);
}

void UStaticMeshComponentPlus::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (CheckPropertyChangedEventTransform(PropertyChangedEvent))
	{
		if (bUsedInRotationSupport) BroadcastRotSupport();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

bool UStaticMeshComponentPlus::CheckPropertyChangedEventTransform(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FString PropertyEventName = PropertyChangedEvent.GetMemberPropertyName().ToString();

	if (PropertyEventName == "RelativeLocation") return true;

	if (PropertyEventName == "RelativeRotation") return true;

	if (PropertyEventName == "RelativeScale3D") return true;

	return false;
}

void UStaticMeshComponentPlus::BroadcastRotSupport()
{
	ARotationSupport* OwnerRotSupport = Cast<ARotationSupport>(GetOwner());
	if (!IsValid(OwnerRotSupport)) return;

	OwnerRotSupport->OnRotationBaseMovedEditor();
}
