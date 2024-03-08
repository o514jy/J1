#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Protocol.pb.h"
#include "J1Player.generated.h"

UCLASS()
class J1_API AJ1Player : public ACharacter
{
	GENERATED_BODY()

public:
	AJ1Player();
	virtual ~AJ1Player();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

public:
	bool IsMyPlayer();

	Protocol::MoveState GetMoveState() { return PlayerInfo->state(); }
	void SetMoveState(Protocol::MoveState State);

public:
	void SetPlayerInfo(const Protocol::PosInfo& Info);
	void SetDestInfo(const Protocol::PosInfo& Info);
	Protocol::PosInfo* GetPlayerInfo() { return PlayerInfo; }

protected:
	class Protocol::PosInfo* PlayerInfo; // 현재 위치
	class Protocol::PosInfo* DestInfo; // 목적지
};