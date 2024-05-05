#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/J1SkillBase.h"
#include "J1SpreadCloud.generated.h"

UCLASS()
class J1_API UJ1SpreadCloud : public UJ1SkillBase
{
	GENERATED_BODY()
public:
	UJ1SpreadCloud();
	~UJ1SpreadCloud();

public:
	/** initialize **/
	virtual void SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId) override;

public:
	/** Process Skill **/
	virtual void DoSkill(const Protocol::S_SKILL& InSkillPkt) override;

public:
	/** handle event **/
	virtual void OnAttackEvent(int32 InTimeCount) override;

};
