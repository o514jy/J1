#include "pch.h"
#include "SafeZone.h"

SafeZone::SafeZone()
{
	_dir = Protocol::DIR_NONE;
}

SafeZone::~SafeZone()
{
}

void SafeZone::UpdateTick()
{
	__super::UpdateTick();
}

void SafeZone::SetInfo(BossRef owner, GimmickBaseRef ownerGimmick, int32 templateId)
{
	__super::SetInfo(owner, ownerGimmick, templateId);
}

void SafeZone::SetDir(Protocol::Direction dir)
{
	_dir = dir;
}

Protocol::Direction SafeZone::GetDir()
{
	return _dir;
}

void SafeZone::OnImpactEvent(int32 impactCount)
{
	__super::OnImpactEvent(impactCount);
}


