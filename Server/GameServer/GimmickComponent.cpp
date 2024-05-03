#include "pch.h"
#include "GimmickComponent.h"
#include "GimmickBase.h"
#include "Data.h"
#include "SkillManager.h"

GimmickComponent::GimmickComponent()
{
	_owner = nullptr;
	_activeGimmick = nullptr;
}

GimmickComponent::~GimmickComponent()
{
	_owner = nullptr;
	_activeGimmick = nullptr;
}

void GimmickComponent::SetInfo(BossRef owner)
{
	_owner = owner;

	_canActiveGimmickList.push_back(GSkillManager->GenerateGimmickById(FIND_SAFE_ZONE_DATA_ID));
}

void GimmickComponent::DoGimmick(int32 gimmickId)
{
	for (GimmickBaseRef gimmick : _canActiveGimmickList)
	{
		if (gimmick->_gimmickData->DataId == gimmickId)
		{
			gimmick->DoGimmick();

			// 패킷 보내기

			return;
		}
	}
}

void GimmickComponent::SetActiveGimmick(GimmickBaseRef gimmick)
{
	_activeGimmick = gimmick;
}

GimmickBaseRef GimmickComponent::GetActiveGimmick()
{
	return _activeGimmick;
}
