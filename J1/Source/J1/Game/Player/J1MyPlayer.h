// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "J1Player.h"
#include "InputActionValue.h"
#include "J1MyPlayer.generated.h"

class UJ1NetworkManager;
class UJ1CameraComponent;

UCLASS()
class J1_API AJ1MyPlayer : public AJ1Player
{
	GENERATED_BODY()

public:
	AJ1MyPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

public:

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "JY|Character")
	TObjectPtr<UJ1CameraComponent> CameraComponent;

protected:
	UJ1NetworkManager* GetNetworkManager() const;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

protected:

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	const float MOVE_PACKET_SEND_DELAY = 0.2f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

	// Cache
	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;

	// Dirty Flag Test
	FVector2D LastDesiredInput;
};
