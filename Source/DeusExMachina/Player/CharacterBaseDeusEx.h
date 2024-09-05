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


	//  Move & Look from Third Person template for basic player movement
	virtual void Move(FVector2D MoveValue);
	virtual void LookCamera(FVector2D LookValue);


	//  Helper for interaction raycast
	void GetCapsuleSize(float& radius, float& halfHeight) const;
};
