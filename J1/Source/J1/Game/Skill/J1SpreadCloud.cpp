#include "J1SpreadCloud.h"

UJ1SpreadCloud::UJ1SpreadCloud()
{
}

UJ1SpreadCloud::~UJ1SpreadCloud()
{
}

void UJ1SpreadCloud::SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId)
{
	Super::SetInfo(InOwner, InTemplateId);
}

void UJ1SpreadCloud::DoSkill(const Protocol::S_SKILL& InSkillPkt)
{
	Super::DoSkill(InSkillPkt);
}

void UJ1SpreadCloud::OnAttackEvent(int32 InTimeCount)
{
	Super::OnAttackEvent(InTimeCount);
}
