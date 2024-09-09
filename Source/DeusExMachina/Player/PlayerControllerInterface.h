#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerControllerEnums.h"
#include "PlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable) //  'NotBlueprintable' here means that the interface cannot be implemented by a blueprint
class UPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class DEUSEXMACHINA_API IPlayerControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, Category = "PlayerInputMode")
	virtual void SetPlayerInputMode(EPlayerInputMode InputMode) = 0;

	UFUNCTION(BlueprintCallable, Category = "PlayerInputMode")
	virtual EPlayerInputMode GetPlayerInputMode() = 0;


	UFUNCTION(BlueprintCallable, Category = "BlockPlayerInputs")
	virtual void BlockPlayerInputs(EBlockPlayerCause Cause) = 0;

	UFUNCTION(BlueprintCallable, Category = "BlockPlayerInputs")
	virtual void UnblockPlayerInputs(EBlockPlayerCause Cause) = 0;

	UFUNCTION(BlueprintCallable, Category = "BlockPlayerInputs")
	virtual bool IsPlayerBlocked() = 0;


	UFUNCTION(BlueprintCallable, Category = "Pause")
	virtual void SetInPause(bool bInPauseValue) = 0;

	UFUNCTION(BlueprintCallable, Category = "Pause")
	virtual bool GetInPause() = 0;
};
