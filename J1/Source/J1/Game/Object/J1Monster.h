#pragma once

#include "CoreMinimal.h"
#include "Game/Object/J1Creature.h"
#include "J1Monster.generated.h"

class AJ1Player;

UCLASS()
class J1_API AJ1Monster : public AJ1Creature
{
	GENERATED_BODY()
public:
	
public:
	/** setter & getter **/
	TObjectPtr<UMonsterData> GetMonsterData() { return Cast<UMonsterData>(CreatureData); }

	void SetTargetPlayer(TObjectPtr<AJ1Player> InPlayer);
	TObjectPtr<AJ1Player> GetTargetPlayer() { return TargetPlayer; }

public:
	/** initialize info **/
	virtual void SetInfo(const Protocol::ObjectInfo& InObjectInfo) override;

public:
	/** information **/
	TObjectPtr<AJ1Player> TargetPlayer = nullptr;
};
