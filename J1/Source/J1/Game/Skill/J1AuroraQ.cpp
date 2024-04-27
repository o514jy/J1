#include "J1AuroraQ.h"

UJ1AuroraQ::UJ1AuroraQ()
{
}

UJ1AuroraQ::~UJ1AuroraQ()
{
}

void UJ1AuroraQ::SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId)
{
	Super::SetInfo(InOwner, InTemplateId);
}

void UJ1AuroraQ::DoSkill(const Protocol::S_SKILL& InSkillPkt)
{
	Super::DoSkill(InSkillPkt);
}

void UJ1AuroraQ::OnAttackEvent(int32 InTimeCount)
{
	Super::OnAttackEvent(InTimeCount);
}
