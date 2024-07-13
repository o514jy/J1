#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/J1SkillBase.h"
#include "J1RangedNormalAttack.generated.h"

UCLASS()
class J1_API UJ1RangedNormalAttack : public UJ1SkillBase
{
	GENERATED_BODY()
public:
	UJ1RangedNormalAttack();
	~UJ1RangedNormalAttack();

public:
	/** initialize **/
	virtual void SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId);

public:
	/** Process Skill **/
	virtual void DoSkill(const Protocol::S_SKILL& InSkillPkt) override;

public:
	/** handle event **/
	virtual void OnAttackEvent(int32 InTimeCount) override;

public:
	/** information **/
	FVector DestPos;
	Protocol::ObjectInfo* ProjectileInfo;
};
