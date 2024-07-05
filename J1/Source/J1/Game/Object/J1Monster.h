#pragma once

#include "CoreMinimal.h"
#include "Game/Object/J1Creature.h"
#include "J1Monster.generated.h"

UCLASS()
class J1_API AJ1Monster : public AJ1Creature
{
	GENERATED_BODY()
public:
	
public:
	/** setter & getter **/
	TObjectPtr<UMonsterData> GetMonsterData() { return Cast<UMonsterData>(CreatureData); }

public:
	/** initialize info **/
	virtual void SetInfo(const Protocol::ObjectInfo& InObjectInfo) override;
};
