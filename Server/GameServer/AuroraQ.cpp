#include "pch.h"
#include "AuroraQ.h"

AuroraQ::AuroraQ()
{
}

AuroraQ::~AuroraQ()
{
}

void AuroraQ::SetInfo(CreatureRef owner, int32 templateId)
{
	__super::SetInfo(owner, templateId);
}

void AuroraQ::OnAttackEvent(int32 timeCount)
{
	__super::OnAttackEvent(timeCount);
}

void AuroraQ::DoSkill(const Protocol::C_SKILL& skillPkt)
{
	__super::DoSkill(skillPkt);
}
