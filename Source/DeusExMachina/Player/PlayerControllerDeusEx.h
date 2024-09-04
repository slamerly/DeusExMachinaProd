#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerDeusEx.generated.h"

class ACharacterBaseDeusEx;
class UInputMappingContext;
class UInputAction;


UCLASS()
class DEUSEXMACHINA_API APlayerControllerDeusEx : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void LateBeginPlay();
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs|Movements")
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs|TempCamera")
	UInputAction* LookInputAction;


protected:
	void Move(const struct FInputActionValue& value);
	void Look(const struct FInputActionValue& value);


public:
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* pawn) override;


protected:
	ACharacterBaseDeusEx* Character{ nullptr };
};
