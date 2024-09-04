#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBaseDeusEx.generated.h"

UCLASS()
class DEUSEXMACHINA_API ACharacterBaseDeusEx : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBaseDeusEx();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
