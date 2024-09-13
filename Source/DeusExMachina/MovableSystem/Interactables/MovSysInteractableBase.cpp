#include "MovSysInteractableBase.h"

AMovSysInteractableBase::AMovSysInteractableBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMovSysInteractableBase::BeginPlay()
{
	Super::BeginPlay();
}


void AMovSysInteractableBase::DisableInteractable()
{
	bDisabled = true;
}

void AMovSysInteractableBase::EnableInteractable()
{
	bDisabled = false;
}

bool AMovSysInteractableBase::IsInteractableDisabled()
{
	return bDisabled;
}

void AMovSysInteractableBase::ForceReleaseInteractable()
{
	//  made to be overriden
}
