#include "pch.h"
#include "GimmickComponent.h"
#include "GimmickBase.h"
#include "Data.h"

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
