#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "J1MyPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UJ1NetworkManager;
class Protocol::PosInfo;

UCLASS()
class AJ1MyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AJ1MyPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** Time Threshold to know when Controller send packet */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float sendMovePacketThreshold;

	/** FX Class that we will spawn when clicking */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	//UNiagaraSystem* FXCursor;

	/** MappingContext */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* SetDestinationClickAction;

public:
	/** network **/
	void RegisterMove(FVector location);
	void ProcessMove(const Protocol::PosInfo& posInfo);

protected:
	UJ1NetworkManager* GetNetworkManager() const;

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	/** Input handlers for BaseAttack action. */
	void OnBaseAttackTriggered();
private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};