#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/J1SkillBase.h"
#include "J1AuroraQ.generated.h"

UCLASS()
class J1_API UJ1AuroraQ : public UJ1SkillBase
{
	GENERATED_BODY()
public:
	UJ1AuroraQ();
	~UJ1AuroraQ();

public:
	/** initialize **/
	virtual void SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId);

public:
	/** Process Skill **/
	virtual void DoSkill(const Protocol::S_SKILL& InSkillPkt) override;

public:
	/** handle event **/
	virtual void OnAttackEvent(int32 InTimeCount) override;
};
