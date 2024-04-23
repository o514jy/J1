#include "J1NormalAttack.h"
#include "Data/J1Data.h"

UJ1NormalAttack::UJ1NormalAttack()
{
}

UJ1NormalAttack::~UJ1NormalAttack()
{
}

void UJ1NormalAttack::SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId)
{
	Super::SetInfo(InOwner, InTemplateId);
}

void UJ1NormalAttack::DoSkill(const Protocol::S_SKILL& InSkillPkt)
{
	Super::DoSkill(InSkillPkt);
}

void UJ1NormalAttack::OnAttackEvent(int32 InTimeCount)
{
	Super::OnAttackEvent(InTimeCount);
}
