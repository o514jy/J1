#pragma once

#include "CoreMinimal.h"
#include "Game/Object/J1Monster.h"
#include "J1Boss.generated.h"

class UJ1GimmickComponent;

UCLASS()
class J1_API AJ1Boss : public AJ1Monster
{
	GENERATED_BODY()
public:

public:
	/** setter & getter **/
	TObjectPtr<UBossData> GetBossData() { return Cast<UBossData>(CreatureData); }

public:
	/** initialize info **/
	virtual void SetInfo(const Protocol::ObjectInfo& InObjectInfo) override;

public:
	/** network **/
	void ProcessGimmick(const Protocol::S_GIMMICK& GimmickPkt);

public:
	/** anim event **/
	virtual void HandleGameplayEvent(FGameplayTag EventTag) override;

public:
	/** component **/
	TObjectPtr<UJ1GimmickComponent> GimmickComponent;

public:
	/** state **/
	virtual void OnDead() override;
};
