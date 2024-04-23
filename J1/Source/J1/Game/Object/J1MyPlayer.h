// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "J1Player.h"
#include "J1MyPlayer.generated.h"

class UJ1NetworkManager;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class J1_API AJ1MyPlayer : public AJ1Player
{
public:
	GENERATED_BODY()

public:
	AJ1MyPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Returns DefaultCameraComponent subobject **/
	FORCEINLINE UCameraComponent* GetDefaultCameraComponent() const { return DefaultCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


public:
	/** network **/
	void CheckAndRegisterMove(float DeltaTime);
	virtual void ProcessMove(const Protocol::PosInfo& Info) override;
	virtual void ProcessSkill(const Protocol::S_SKILL& InSkillPkt) override;

	bool CompareNowPosAndDestPos();

protected:
	UJ1NetworkManager* GetNetworkManager() const;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* DefaultCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:
	const float MOVE_PACKET_SEND_DELAY = 0.2f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;
};
