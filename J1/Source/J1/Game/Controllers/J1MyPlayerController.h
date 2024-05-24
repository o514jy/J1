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
class AJ1MyPlayer;
class USplineComponent;

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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

public:
	/** setter & getter **/
	TObjectPtr<AJ1MyPlayer> GetMyPlayer();
	

public:
	/** network **/

protected:
	UJ1NetworkManager* GetNetworkManager() const;

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	/** Input handlers for BaseAttack action. */
	void OnBaseAttackTriggered();

	/** Input handlers for Q action. */
	void OnQTriggered();

public:
	/** move **/
	void AutoRun();

private:
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	FVector CachedDestination;
	float FollowTime; // For how long it has been pressed
};